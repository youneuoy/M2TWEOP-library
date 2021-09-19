#include "singleFbxMesh.h"
#include "assert.h"
#include <fstream>
#include "tAutodeskMemoryStream.h"
#

template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
   assert(!(hi < lo));
   return (v < lo) ? lo : (hi < v) ? hi : v;
}

void writeToLogFile(
   const std::string& fileName,
   const std::string& text)
{
   FILE* fp;

   fopen_s(&fp, fileName.c_str(), "a");
   if (nullptr == fp)
   {
      return;
   }

   fprintf(fp, "%s\n", text.c_str());
   fclose(fp);
}

SingleFbxMesh::SingleFbxMesh()
   :m_sdkManagerPtr(nullptr)
   , m_devicePtr(nullptr)
   , m_filename()
   , m_scenePtr(nullptr)
   , m_majorFileVersion(0)
   , m_minorFileVersion(0)
   , m_revisionFileVersion(0)
   , m_modelVector()
   , m_textureName()
   , m_boneVector()
   , m_allByControlPoint(true)
   , m_boneMatrixVectorSize(0)
   , m_boneMatrixVector()
   , m_initialAnimationDurationInMs(0)
   , m_texturePtr(nullptr)
   , m_effectPtr(nullptr)
   , m_skinnedMeshVertexDeclarationPtr(nullptr)
{
//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
   m_sdkManagerPtr = FbxManager::Create();
   assert(nullptr != m_sdkManagerPtr);

// Create an IOSettings object. This object holds all import settings.
   fbxsdk::FbxIOSettings* ioSettingsPtr = FbxIOSettings::Create(m_sdkManagerPtr, IOSROOT);
   m_sdkManagerPtr->SetIOSettings(ioSettingsPtr);

// I probably could have made more things false for this example. You might want
// to revisit this.
   m_sdkManagerPtr->GetIOSettings()->SetBoolProp(IMP_FBX_CONSTRAINT, false);
   m_sdkManagerPtr->GetIOSettings()->SetBoolProp(IMP_FBX_CONSTRAINT_COUNT, false);
   m_sdkManagerPtr->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, true);
   m_sdkManagerPtr->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, true);
   m_sdkManagerPtr->GetIOSettings()->SetBoolProp(IMP_FBX_LINK, true);
   m_sdkManagerPtr->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, true);
   m_sdkManagerPtr->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO, false);
   m_sdkManagerPtr->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, true);
   m_sdkManagerPtr->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
}

SingleFbxMesh::~SingleFbxMesh()
{
// Unloads the model.
   _releaseFile();

   if (nullptr != m_sdkManagerPtr)
   {
// Destroys all objects assocated with the manager.
      m_sdkManagerPtr->Destroy();
      m_sdkManagerPtr = nullptr;
   }
}

void SingleFbxMesh::load(
   LPDIRECT3DDEVICE9 devicePtr,
   const char* meshName,
   const char* textureName,
   unsigned long boneMatrixVectorSize)
{
// Generally, you want this to be around 30-50. Going below 12
// could cause a failure condition where we couldn't properly render
// many of the vertices. This is because each triangle has up to 12
// bone influences. In practice, most triangles have 1-3 influences?
   assert(boneMatrixVectorSize >= kTriangleVertexCount * kBoneInfluencesPerVertice);
   assert(devicePtr != nullptr);

   m_devicePtr = devicePtr;
   m_filename = meshName;
   m_textureName = textureName;
   m_boneMatrixVectorSize = boneMatrixVectorSize;

// This is safe to do before we have the device created.
   _loadModel();

// This can't be called until the device is created.
   _loadMeshBuffers();

// We just need the duration of the first animation track.
   m_initialAnimationDurationInMs = _getAnimationDuration();

   _loadModelTexture();
   _loadModelEffect();
   _loadSkinnedMeshVertexDeclaration();
}

void SingleFbxMesh::release()
{
   _releaseMeshBuffers();
}

void SingleFbxMesh::advanceTime()
{
   fbxsdk::FbxTime fbxFrameTime;
   unsigned long long localAnimationTime = GetTickCount64() % m_initialAnimationDurationInMs;

   fbxFrameTime.SetMilliSeconds(localAnimationTime);

   _buildMatrices(fbxFrameTime);
}

void SingleFbxMesh::render(
   const D3DXMATRIX& worldViewProj)
{
   m_effectPtr->SetMatrix("gWorldViewProj", &worldViewProj);
   m_effectPtr->SetTexture("gBaseTexture", m_texturePtr);
   m_effectPtr->SetTechnique("minimalSkinnedTechnique");

   for (size_t modelIndex = 0; modelIndex < m_modelVector.size(); ++modelIndex)
   {
      m_devicePtr->SetIndices(m_modelVector[modelIndex].indexBufferPtr);
      m_devicePtr->SetStreamSource(
         0,
         m_modelVector[modelIndex].vertexBufferPtr,
         0,
         sizeof(tSkinnedVertice));

      _loadBoneMatriceVector();

      m_devicePtr->SetVertexDeclaration(m_skinnedMeshVertexDeclarationPtr);

      // run through each pass and draw
      UINT numPasses;
      UINT numberOfTriangles = static_cast<UINT>(m_modelVector[modelIndex].indexVector.size()) / kTriangleVertexCount;
      UINT numberOfVertices = static_cast<UINT>(m_modelVector[modelIndex].verticeVector.size());

      m_effectPtr->Begin(&numPasses, D3DXFX_DONOTSAVESTATE);
      for (UINT i = 0; i < numPasses; i++)
      {
         m_effectPtr->BeginPass(i);

         m_devicePtr->DrawIndexedPrimitive(
            D3DPT_TRIANGLELIST,
            0,
            0,
            numberOfVertices,
            0,
            numberOfTriangles);

         m_effectPtr->EndPass();
      }

      m_effectPtr->End();
   }
}

void SingleFbxMesh::_loadModelTexture()
{
// I tend to load things out of memory buffers
// instead of files.
   std::ifstream file(m_textureName.c_str(), std::ios::binary | std::ios::ate);
   size_t fileSize = (size_t)file.tellg();
   file.seekg(0, std::ios::beg);

   std::vector<unsigned char> buffer;

   buffer.resize(size_t(fileSize), 0);
   file.read((char*)buffer.data(), fileSize);


   D3DXIMAGE_INFO imageInfo;

   // Inspect the texture file to determine the texture type.
   const long imageInfoStatus = D3DXGetImageInfoFromFileInMemory(
      buffer.data(),
      buffer.size(),
      &imageInfo);

   assert(imageInfoStatus == 0);

   const long createStatus = D3DXCreateTextureFromFileInMemoryEx(
      m_devicePtr,
      buffer.data(),
      buffer.size(),
      imageInfo.Width, // width
      imageInfo.Height, // height
      imageInfo.MipLevels, // mip levels
      0, // usage
      imageInfo.Format, // format
      D3DPOOL_MANAGED, // memory pool
      D3DX_FILTER_POINT, // filter
      D3DX_FILTER_POINT, // MIP filter
      0, // color key - disabled
      &imageInfo, // source image file information
      nullptr, // palette entry
      (IDirect3DTexture9**)&m_texturePtr);

   assert(createStatus == 0);
   assert(nullptr != m_texturePtr);
}

void SingleFbxMesh::_loadModelEffect()
{
   LPD3DXBUFFER errorBufferPtr = nullptr;

   if (0 != D3DXCreateEffectFromFileA(
      m_devicePtr,
      "skinned.fx",
      nullptr, // CONST D3DXMACRO* pDefines
      nullptr, // LPD3DXINCLUDE pInclude
      0, // shader flags
      nullptr,
      &m_effectPtr,
      &errorBufferPtr))
   {
      LPVOID pCompileErrors = errorBufferPtr->GetBufferPointer();

      MessageBoxA(
         nullptr,
         (const char*)pCompileErrors, "Compile Error",
         MB_OK | MB_ICONEXCLAMATION);

      assert(0);
   }
}

void SingleFbxMesh::_loadSkinnedMeshVertexDeclaration()
{
   const D3DVERTEXELEMENT9 vertexElementsSkinned[] =
   {
      { 0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0 },
      { 0, 12, D3DDECLTYPE_UBYTE4N,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,  0 },
      { 0, 16, D3DDECLTYPE_UBYTE4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
      { 0, 20, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,       0 },
      { 0, 32, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     0 },
      D3DDECL_END()
   };

   if (nullptr == m_skinnedMeshVertexDeclarationPtr)
   {
      m_devicePtr->CreateVertexDeclaration(
         vertexElementsSkinned,
         &m_skinnedMeshVertexDeclarationPtr);
   }
}

void SingleFbxMesh::_buildMatrices(
   const fbxsdk::FbxTime& fbxFrameTime)
{
   if (m_boneVector.empty())
   {
      return;
   }

// Here's most of the matrix bone operations from start to finish.
// The exception is the offset matrix calculation that happens once
// when the bone is created.

// Set the matrices that change by time and animation keys.
   _loadNodeLocalTransformMatrices(fbxFrameTime);

   // Propagate the local transform matrices from parent to child.
   _calculateCombinedTransforms();

   // This is the final pass that prepends the offset matrix.
   _calculatePaletteMatrices();
}

void SingleFbxMesh::_loadModel()
{
// I tend to load things out of memory buffers
// instead of files.
   std::ifstream file(m_filename.c_str(), std::ios::binary | std::ios::ate);
   size_t fileSize = (size_t)file.tellg();
   file.seekg(0, std::ios::beg);

   std::vector<unsigned char> buffer;
   
   buffer.resize(size_t(fileSize), 0);
   file.read((char*)buffer.data(), fileSize);

//Create an FBX scene. This object holds the object imported from a file.
// I'm current assuming we should have a scene per file.
   m_scenePtr = FbxScene::Create(m_sdkManagerPtr, m_filename.c_str());



   FbxIOPluginRegistry* fbxIoPluginRegistry = m_sdkManagerPtr->GetIOPluginRegistry();
   tAutodeskMemoryStream stream = tAutodeskMemoryStream((char*)buffer.data(), fileSize, fbxIoPluginRegistry->FindReaderIDByExtension("fbx"));
   fbxsdk::FbxImporter* importerPtr = FbxImporter::Create(m_sdkManagerPtr, "");


   importerPtr->Initialize(&stream, nullptr, -1, m_sdkManagerPtr->GetIOSettings());
   importerPtr->Import(m_scenePtr);


// Non-vital info, but FYI!
   importerPtr->GetFileVersion(m_majorFileVersion, m_minorFileVersion, m_revisionFileVersion);



   FbxNode* rootNodePtr = m_scenePtr->GetRootNode();



   FbxAxisSystem SceneAxisSystem = m_scenePtr->GetGlobalSettings().GetAxisSystem();
   FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
   if (SceneAxisSystem != OurAxisSystem)
   {
      OurAxisSystem.ConvertScene(m_scenePtr);
   }

// Convert Unit System to what is used in this example, if needed
// causes memory leaks
   FbxSystemUnit SceneSystemUnit = m_scenePtr->GetGlobalSettings().GetSystemUnit();
   if (SceneSystemUnit.GetScaleFactor() != 1.0)
   {
      FbxSystemUnit OurSystemUnit(1.0);
      OurSystemUnit.ConvertScene(m_scenePtr);
   }

// Convert mesh, NURBS and patch into triangle mesh
// DirectX desires triangles.
   {
      FbxGeometryConverter geometryConverter(m_sdkManagerPtr);
      geometryConverter.Triangulate(
          m_scenePtr, // converts the entire scene, but we have one mesh per scene
                      // maybe revisit this to use the node once I know more?
          true,// replace with triangles
          false);

   }

   _loadTextureNames();

   _loadBones(m_scenePtr->GetRootNode(), kInvalidBoneIndex);
   _loadMeshes(m_scenePtr->GetRootNode());

   importerPtr->Destroy();
   importerPtr = nullptr;
}

void SingleFbxMesh::_loadBones(
   FbxNode* nodePtr,
   long parentBoneIndex)
{
   FbxNodeAttribute* pNodeAttribute = nodePtr->GetNodeAttribute();
   long childCount = nodePtr->GetChildCount();

   if ((nullptr != pNodeAttribute)
      && (pNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton))
   {
      _loadBone(
         nodePtr,
         parentBoneIndex);

      parentBoneIndex = static_cast<long>(m_boneVector.size()) - 1;
   }

   for (long lChildIndex = 0; lChildIndex < childCount; ++lChildIndex)
   {
      _loadBones(nodePtr->GetChild(lChildIndex), parentBoneIndex);
   }
}

void SingleFbxMesh::_loadBone(
   FbxNode* nodePtr,
   long parentBoneIndex)
{
   tBone dummy;

   m_boneVector.push_back(dummy);

   tBone& back = m_boneVector.back();

   back.boneNodePtr = nodePtr;
   back.fbxSkeletonPtr = (FbxSkeleton*)nodePtr->GetNodeAttribute();
   back.fbxClusterPtr = nullptr; // fix this up later
   back.name = back.fbxSkeletonPtr->GetName();
   back.parentIndex = parentBoneIndex;

   // We aren't ready to load these, so set them to identity.
   D3DXMatrixIdentity(&back.offset);
   D3DXMatrixIdentity(&back.boneMatrice);
   D3DXMatrixIdentity(&back.combinedTransform);

   // Get our local transform at time 0.
   _getNodeLocalTransform(nodePtr, back.nodeLocalTransform);

   if (parentBoneIndex != kInvalidBoneIndex)
   {
      m_boneVector[parentBoneIndex].childIndexes.push_back(static_cast<long>(m_boneVector.size() - 1));
   }
}

void SingleFbxMesh::_loadMeshes(
   FbxNode* nodePtr)
{
   FbxNodeAttribute* pNodeAttribute = nodePtr->GetNodeAttribute();
   long childCount = nodePtr->GetChildCount();

   if ((nullptr != pNodeAttribute)
      && (pNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh))
   {
      _loadMesh(nodePtr);
   }

   for (long lChildIndex = 0; lChildIndex < childCount; ++lChildIndex)
   {
      _loadMeshes(nodePtr->GetChild(lChildIndex));
   }
}

void SingleFbxMesh::_loadMesh(
   FbxNode* nodePtr)
{
   // Bake material and hook as user data.
   const long materialCount = nodePtr->GetMaterialCount();
   FbxNodeAttribute* nodeAttributePtr = nodePtr->GetNodeAttribute();

   {// I'm old school.  I should do an stl swap instead.
      tModelRec emptyModelRec;
      m_modelVector.push_back(emptyModelRec);
   }
   tModelRec& modelRec = m_modelVector.back();

   modelRec.indexBufferPtr = nullptr;
   modelRec.vertexBufferPtr = nullptr;

// This paragraph of code isn't necessary, but it's nice to see.
   for (long lMaterialIndex = 0; lMaterialIndex < materialCount; ++lMaterialIndex)
   {
      FbxSurfaceMaterial* lMaterial = nodePtr->GetMaterial(lMaterialIndex);
      if (lMaterial && !lMaterial->GetUserDataPtr())
      {
         modelRec.materialName = lMaterial->GetName();
      }
   }

   if (nullptr != nodeAttributePtr)
   {
      // Bake mesh as VBO(vertex buffer object) into GPU.
      if (nodeAttributePtr->GetAttributeType() == FbxNodeAttribute::eMesh)
      {
         FbxMesh* meshPtr = nodePtr->GetMesh();

         if (nullptr != meshPtr)
         {
            long boneCount = _getBoneCount(meshPtr);

// This line of code isn't necessary.
            modelRec.meshName = meshPtr->GetName();

            _loadMeshPositionNormalUV(nodePtr, modelRec);

            assert(_isMeshSkinned(meshPtr));

// Ensure our bone matrix vector size is large enough.
            assert(m_boneMatrixVectorSize >= (unsigned int)boneCount);

            _loadMeshBoneWeightsAndIndices(nodePtr, modelRec);
            _normalizeBoneWeights(modelRec);
            _compressSkinnedVertices(modelRec);
         }
      }
   }
}

void SingleFbxMesh::_loadMeshPositionNormalUV(
   FbxNode* nodePtr,
   tModelRec& modelRec)
{
// Wow, I wish I could say I totally understood or tested this code. It mostly
// came out of Autodesk's only example where GL displays an .fbx mesh using
// software vertice processing.
   FbxMesh* meshPtr = nodePtr->GetMesh();

   assert(nullptr != meshPtr->GetElementMaterial());
   assert(meshPtr->GetElementNormalCount() > 0);
   assert(meshPtr->GetElementUVCount() > 0);
   assert(0 == meshPtr->GetDeformerCount(FbxDeformer::eBlendShape));
   assert(0 == meshPtr->GetDeformerCount(FbxDeformer::eVertexCache));
   assert(1 <= meshPtr->GetDeformerCount(FbxDeformer::eSkin));

   FbxGeometryElement::EMappingMode normalMappingMode = meshPtr->GetElementNormal(0)->GetMappingMode();
   FbxGeometryElement::EMappingMode uvMappingMode = meshPtr->GetElementUV(0)->GetMappingMode();
   FbxLayerElementArrayTemplate<int>* materialIndice = &meshPtr->GetElementMaterial()->GetIndexArray();

   assert(normalMappingMode != FbxGeometryElement::eNone);
   assert(uvMappingMode != FbxGeometryElement::eNone);
   assert(nullptr != materialIndice);

   const long polygonCount = meshPtr->GetPolygonCount();
   // Count the polygon count of each material
   FbxGeometryElement::EMappingMode lMaterialMappingMode = meshPtr->GetElementMaterial()->GetMappingMode();
   tSkinnedVertice emptyVertice;

   ZeroMemory(&emptyVertice, sizeof(emptyVertice));

   // Congregate all the data of a mesh to be cached in VBOs.
   // If normal or UV is by polygon vertex, record all vertex attributes by polygon vertex.
   if (normalMappingMode != FbxGeometryElement::eByControlPoint)
   {
      m_allByControlPoint = false;
   }

   if (uvMappingMode != FbxGeometryElement::eByControlPoint)
   {
      m_allByControlPoint = false;
   }

   // Allocate the array memory, by control point or by polygon vertex.
   long polygonVertexCount = meshPtr->GetControlPointsCount();

   if (!m_allByControlPoint)
   {
      polygonVertexCount = polygonCount * kTriangleVertexCount;
   }

   modelRec.indexVector.resize(polygonCount * kTriangleVertexCount);
   modelRec.verticeVector.resize(polygonVertexCount, emptyVertice);

   FbxStringList uvNames;
   meshPtr->GetUVSetNames(uvNames);
   const char* uvName = NULL;

   if (uvNames.GetCount() != 0)
   {
      uvName = uvNames[0];
   }

   // Populate the array with vertex attribute, if by control point.
   const FbxVector4* controlPoints = meshPtr->GetControlPoints();
   FbxVector4 currentVertex;
   FbxVector4 currentNormal;
   FbxVector2 currentUV;
   D3DXMATRIX geometryTransformMatrix;

   _getGeometryTransformMatrix(nodePtr, geometryTransformMatrix);

   if (m_allByControlPoint)
   {
      const FbxGeometryElementNormal* lNormalElement = meshPtr->GetElementNormal(0);;
      const FbxGeometryElementUV* lUVElement = meshPtr->GetElementUV(0);

      for (long index = 0; index < polygonVertexCount; ++index)
      {
         // Save the vertex position.
         {
            currentVertex = controlPoints[index];
            D3DXVECTOR3 dxVertex = D3DXVECTOR3(
               static_cast<float>(currentVertex[0]),
               static_cast<float>(currentVertex[1]),
               static_cast<float>(currentVertex[2]));
            D3DXVec3TransformCoord(&modelRec.verticeVector[index].point, &dxVertex, &geometryTransformMatrix);
         }

         // Save the normal.
         {
            long lNormalIndex = index;

            if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
            {
               lNormalIndex = lNormalElement->GetIndexArray().GetAt(index);
            }

            currentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
            D3DXVECTOR3 dxNormal = D3DXVECTOR3(
               static_cast<float>(currentNormal[0]),
               static_cast<float>(currentNormal[1]),
               static_cast<float>(currentNormal[2]));
            D3DXVec3TransformNormal(&modelRec.verticeVector[index].normal, &dxNormal, &geometryTransformMatrix);
         }

         // Save the UV.
         {
            long uvIndex = index;

            if (lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
            {
               uvIndex = lUVElement->GetIndexArray().GetAt(index);
            }

            currentUV = lUVElement->GetDirectArray().GetAt(uvIndex);
            modelRec.verticeVector[index].u = static_cast<float>(currentUV[0]);
            modelRec.verticeVector[index].v = static_cast<float>(currentUV[1]);
         }
      }
   }

   long lVertexCount = 0;
   long numberOfTriangles = 0;

   for (long lPolygonIndex = 0; lPolygonIndex < polygonCount; ++lPolygonIndex)
   {
      // The material for current face.
      long materialIndex = 0;
      if (materialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
      {
         materialIndex = materialIndice->GetAt(lPolygonIndex);
      }

      // Where should I save the vertex attribute index, according to the material
      const long indexOffset = numberOfTriangles * 3;

      for (long verticeIndex = 0; verticeIndex < kTriangleVertexCount; ++verticeIndex)
      {
         const long controlPointIndex = meshPtr->GetPolygonVertex(lPolygonIndex, verticeIndex);

         if (m_allByControlPoint)
         {
            modelRec.indexVector[indexOffset + verticeIndex] = static_cast<unsigned short>(controlPointIndex);
         }
         // Populate the array with vertex attribute, if by polygon vertex.
         else
         {
            modelRec.indexVector[indexOffset + verticeIndex] = static_cast<unsigned short>(lVertexCount);

            currentVertex = controlPoints[controlPointIndex];
            D3DXVECTOR3 dxVertex = D3DXVECTOR3(
               static_cast<float>(currentVertex[0]),
               static_cast<float>(currentVertex[1]),
               static_cast<float>(currentVertex[2]));
            D3DXVec3TransformCoord(&modelRec.verticeVector[lVertexCount].point, &dxVertex, &geometryTransformMatrix);

            meshPtr->GetPolygonVertexNormal(lPolygonIndex, verticeIndex, currentNormal);
            D3DXVECTOR3 dxNormal = D3DXVECTOR3(
               static_cast<float>(currentNormal[0]),
               static_cast<float>(currentNormal[1]),
               static_cast<float>(currentNormal[2]));
            D3DXVec3TransformNormal(&modelRec.verticeVector[lVertexCount].normal, &dxNormal, &geometryTransformMatrix);

            {
               bool unmappedUV;

               meshPtr->GetPolygonVertexUV(lPolygonIndex, verticeIndex, uvName, currentUV, unmappedUV);
               modelRec.verticeVector[lVertexCount].u = static_cast<float>(currentUV[0]);
               modelRec.verticeVector[lVertexCount].v = 1.0f - static_cast<float>(currentUV[1]);
            }
         }

         ++lVertexCount;
      }

      numberOfTriangles += 1;
   }
}

// Here's a sad sad story:
// My meshes came out with fully elaborated vertex buffers.
// Ex. a cube would have 36 vertices instead of 8
// This function re-indexes the vertex buffer and makes it smaller.
// Wasn't that sad?  Almost 3 sads, but I cut it off at two.
// I'll let you figure out the third sad.
void SingleFbxMesh::_compressSkinnedVertices(
   SingleFbxMesh::tModelRec& modelRec)
{
   tSkinnedVerticeVector newVertices;
   unsigned short foundIndice;

   for (auto& i : modelRec.indexVector)
   {
      tSkinnedVertice currentSkinnedVertice = modelRec.verticeVector[i];

      foundIndice = _findSkinnedVertice(newVertices, currentSkinnedVertice);

      if (foundIndice == 0xFFFF) // not found
      {
         i = static_cast<unsigned short>(newVertices.size());
         newVertices.push_back(currentSkinnedVertice);
      }
      else
      {
         i = foundIndice;
      }
   }

   modelRec.verticeVector.swap(newVertices);
}

unsigned short  SingleFbxMesh::_findSkinnedVertice(
   const  SingleFbxMesh::tSkinnedVerticeVector& skinnedVerticeVector,
   const  SingleFbxMesh::tSkinnedVertice& skinnedVertice)
{
   // I'm doing an old fashion sequential search. I figure it's not
   // too awful with processor memory caches.
   // Also, the code is easier to read.
   for (size_t i = 0; i < skinnedVerticeVector.size(); ++i)
   {
      if ((skinnedVertice.point == skinnedVerticeVector[i].point)
         && (skinnedVertice.normal == skinnedVerticeVector[i].normal)
         && (skinnedVertice.u == skinnedVerticeVector[i].u)
         && (skinnedVertice.v == skinnedVerticeVector[i].v))
      {
         return static_cast<unsigned short>(i);
      }
   }

   return 0xFFFF;
}

void SingleFbxMesh::_loadMeshBoneWeightsAndIndices(
   FbxNode* nodePtr,
   tModelRec& modelRec)
{
   FbxMesh* meshPtr = nodePtr->GetMesh();
   D3DXMATRIX geometryTransform;
   tControlPointRemap controlPointRemap;

   // maps control points to vertex indexes
   _loadControlPointRemap(meshPtr, controlPointRemap);

   // takes into account an offsetted model.
   _getGeometryTransformMatrix(nodePtr, geometryTransform);

   // A deformer is a FBX thing, which contains some clusters
   // A cluster contains a link, which is basically a joint
   // Normally, there is only one deformer in a mesh
   // There are many types of deformers in Maya,
   // We are using only skins, so we see if this is a skin
   FbxSkin* currSkin = (FbxSkin*)(meshPtr->GetDeformer(0, FbxDeformer::eSkin));

   long numberOfClusters = currSkin->GetClusterCount();
   for (long clusterIndex = 0; clusterIndex < numberOfClusters; ++clusterIndex)
   {
      FbxCluster* clusterPtr = currSkin->GetCluster(clusterIndex);
      long numOfIndices = clusterPtr->GetControlPointIndicesCount();
      double* weightPtr = clusterPtr->GetControlPointWeights();
      int* indicePtr = clusterPtr->GetControlPointIndices();
      std::string currJointName = clusterPtr->GetLink()->GetName();
      std::string secondName = clusterPtr->GetName();
      unsigned long boneIndex = _boneNameToindex(currJointName);
      FbxAMatrix transformMatrix;
      FbxAMatrix transformLinkMatrix;
      FbxAMatrix globalBindposeInverseMatrix;

      // Now that we have the clusterPtr, let's calculate the inverse bind pose matrix.
      clusterPtr->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
      clusterPtr->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
      globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix;// *geometryTransform; // find out if we need geometryTransform
      _fbxToMatrix(globalBindposeInverseMatrix, m_boneVector[boneIndex].offset);

      // Update the information in mSkeleton 
      m_boneVector[boneIndex].fbxClusterPtr = clusterPtr;

      if (nullptr == clusterPtr->GetLink())
      {
         continue;
      }

      // Associate each joint with the control points it affects
      for (long i = 0; i < numOfIndices; ++i)
      {
         double weight = weightPtr[i];

         if (weight == 0.0)
         {
            continue;
         }

         // all the points the control word is mapped to
         tControlPointIndexes& controlPointIndexes = controlPointRemap[indicePtr[i]];

         for (auto const& i : controlPointIndexes)
         {
            // Change the index vector offset, to a vertex offset.
            long vertexIndex = modelRec.indexVector[i];

            _addBoneInfluence(modelRec.verticeVector, vertexIndex, boneIndex, weight);
         }
      }
   }
}

void SingleFbxMesh::_normalizeBoneWeights(
   tModelRec& modelRec)
{
   for (auto& vertice : modelRec.verticeVector)
   {
      tPackedInt packedWeights;

      packedWeights.number = vertice.boneWeights;

      {
         unsigned long totalWeight =
            packedWeights.bytes[0]
            + packedWeights.bytes[1]
            + packedWeights.bytes[2]
            + packedWeights.bytes[3];

         assert(totalWeight != 0);

         if ((totalWeight >= 254)
            && (totalWeight <= 256))
         { // no need to normalize
            continue;
         }
      }

      float weights[4];
      float calculation[4];

      weights[0] = static_cast<float>(packedWeights.bytes[0]);
      weights[1] = static_cast<float>(packedWeights.bytes[1]);
      weights[2] = static_cast<float>(packedWeights.bytes[2]);
      weights[3] = static_cast<float>(packedWeights.bytes[3]);

      float totalWeight = weights[0] + weights[1] + weights[2] + weights[3];

      calculation[0] = 255.0f * (weights[0] / totalWeight) + 0.5f;
      calculation[1] = 255.0f * (weights[1] / totalWeight) + 0.5f;
      calculation[2] = 255.0f * (weights[2] / totalWeight) + 0.5f;
      calculation[3] = 255.0f * (weights[3] / totalWeight) + 0.5f;

      packedWeights.bytes[0] = static_cast<unsigned char>(calculation[0]);
      packedWeights.bytes[1] = static_cast<unsigned char>(calculation[1]);
      packedWeights.bytes[2] = static_cast<unsigned char>(calculation[2]);
      packedWeights.bytes[3] = static_cast<unsigned char>(calculation[3]);

      vertice.boneWeights = packedWeights.number;
   }
}

void SingleFbxMesh::_loadTextureNames()
{
// We aren't using the embedded texture name, but this is here for convience.
   const long lTextureCount = m_scenePtr->GetTextureCount();
   for (long lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
   {
      FbxTexture* lTexture = m_scenePtr->GetTexture(lTextureIndex);
      FbxFileTexture* lFileTexture = FbxCast<FbxFileTexture>(lTexture);
      if (lFileTexture && !lFileTexture->GetUserDataPtr())
      {
         std::string fileName = lFileTexture->GetFileName();
      }
   }
}

void SingleFbxMesh::_releaseFile()
{
   if (nullptr != m_skinnedMeshVertexDeclarationPtr)
   {
      m_skinnedMeshVertexDeclarationPtr->Release();
      m_skinnedMeshVertexDeclarationPtr = nullptr;
   }

   if (nullptr != m_texturePtr)
   {
      m_texturePtr->Release();
      m_texturePtr = nullptr;
   }

   if (nullptr != m_effectPtr)
   {
      m_effectPtr->Release();
      m_effectPtr = nullptr;
   }

   _releaseMeshBuffers();

   if (nullptr != m_scenePtr)
   {
      m_scenePtr->Destroy();
      m_scenePtr = nullptr;
   }
}

void SingleFbxMesh::_loadMeshBuffers()
{
   for (auto& model : m_modelVector)
   {
      unsigned long indexBufferSize;
      unsigned short* indicesPtr;
      unsigned long vertexBufferSize;
      void* verticesPtr;

      indexBufferSize = model.indexVector.size() * sizeof(unsigned short);
      vertexBufferSize = model.verticeVector.size() * sizeof(tSkinnedVertice);

      m_devicePtr->CreateIndexBuffer(
         indexBufferSize,
         D3DUSAGE_WRITEONLY,
         D3DFMT_INDEX16,
         D3DPOOL_MANAGED,
         &(model.indexBufferPtr),
         NULL);
      m_devicePtr->CreateVertexBuffer(
         vertexBufferSize,
         D3DUSAGE_WRITEONLY,
         0,
         D3DPOOL_MANAGED,
         &(model.vertexBufferPtr),
         NULL);
      model.indexBufferPtr->Lock(
         0,
         indexBufferSize,
         (void**)&indicesPtr,
         0);
      model.vertexBufferPtr->Lock(
         0,
         vertexBufferSize,
         (void**)&verticesPtr,
         0);

      memcpy((char*)indicesPtr, model.indexVector.data(), indexBufferSize);
      memcpy((char*)verticesPtr, model.verticeVector.data(), vertexBufferSize);

      model.vertexBufferPtr->Unlock();
      model.indexBufferPtr->Unlock();
   }
}

void SingleFbxMesh::_releaseMeshBuffers()
{
   for (auto& model : m_modelVector)
   {
      if (NULL != model.indexBufferPtr)
      {
         model.indexBufferPtr->Release();
         model.indexBufferPtr = NULL;
      }

      if (NULL != model.vertexBufferPtr)
      {
         model.vertexBufferPtr->Release();
         model.vertexBufferPtr = NULL;
      }
   }

   m_modelVector.clear();
}

bool SingleFbxMesh::_isMeshSkinned(
   FbxMesh* meshPtr)
{
   long boneCount = _getBoneCount(meshPtr);

   return boneCount > 0;
}

long SingleFbxMesh::_getBoneCount(
   FbxMesh* meshPtr)
{
   if (meshPtr->GetDeformerCount(FbxDeformer::eSkin) == 0)
   {
      return 0;
   }

   return ((FbxSkin*)(meshPtr->GetDeformer(0, FbxDeformer::eSkin)))->GetClusterCount();
}

void SingleFbxMesh::_getGeometryTransformMatrix(
   FbxNode* nodePtr,
   D3DXMATRIX& geometryOffsetMatrix)
{
   // Get the geometry offset to a node. It is never inherited by the children.
   const FbxVector4 lT = nodePtr->GetGeometricTranslation(FbxNode::eSourcePivot);
   const FbxVector4 lR = nodePtr->GetGeometricRotation(FbxNode::eSourcePivot);
   const FbxVector4 lS = nodePtr->GetGeometricScaling(FbxNode::eSourcePivot);

   FbxAMatrix fbxMatrix = FbxAMatrix(lT, lR, lS);

   _fbxToMatrix(fbxMatrix, geometryOffsetMatrix);
}

void SingleFbxMesh::_getNodeLocalTransform(
   FbxNode* nodePtr,
   D3DXMATRIX& matrix)
{
   FbxAMatrix fbxMatrix = m_scenePtr->GetAnimationEvaluator()->GetNodeLocalTransform(nodePtr);

   _fbxToMatrix(fbxMatrix, matrix);
}

void SingleFbxMesh::_getNodeLocalTransform(
   FbxNode* nodePtr,
   const FbxTime& fbxTime,
   D3DXMATRIX& matrix)
{
   FbxAMatrix fbxMatrix = m_scenePtr->GetAnimationEvaluator()->GetNodeLocalTransform(nodePtr, fbxTime);

   _fbxToMatrix(fbxMatrix, matrix);
}

void SingleFbxMesh::_fbxToMatrix(
   const FbxAMatrix& fbxMatrix,
   D3DXMATRIX& matrix)
{
   for (unsigned long i = 0; i < 4; ++i)
   {
      matrix(i, 0) = (float)fbxMatrix.Get(i, 0);
      matrix(i, 1) = (float)fbxMatrix.Get(i, 1);
      matrix(i, 2) = (float)fbxMatrix.Get(i, 2);
      matrix(i, 3) = (float)fbxMatrix.Get(i, 3);
   }
}

void SingleFbxMesh::_calculateCombinedTransforms()
{
   if (m_boneVector.empty())
   {
      return;
   }

   if (m_boneVector.size() == 1)
   {
      return;
   }

   for (auto& bone : m_boneVector)
   {
      if (bone.parentIndex != kInvalidBoneIndex)
      {
         bone.combinedTransform = bone.nodeLocalTransform * m_boneVector[bone.parentIndex].combinedTransform;
      }
      else
      {
         bone.combinedTransform = bone.nodeLocalTransform;
      }
   }
}

long SingleFbxMesh::_boneNameToindex(
   const std::string& boneName)
{
   long index = 0;

   for (auto const& i : m_boneVector)
   {
      if (i.name == boneName)
      {
         return index;
      }

      ++index;
   }

   assert(false);
   return kInvalidBoneIndex;
}

void SingleFbxMesh::_loadControlPointRemap(
   FbxMesh* meshPtr,
   SingleFbxMesh::tControlPointRemap& controlPointRemap)
{
   const long lPolygonCount = meshPtr->GetPolygonCount();

   controlPointRemap.resize(lPolygonCount);

   for (long lPolygonIndex = 0; lPolygonIndex < lPolygonCount; lPolygonIndex++)
   {
      long lPolygonSize = meshPtr->GetPolygonSize(lPolygonIndex);

      for (long lVertexIndex = 0; lVertexIndex < lPolygonSize; lVertexIndex++)
      {
         long lControlPointIndex = meshPtr->GetPolygonVertex(lPolygonIndex, lVertexIndex);

         controlPointRemap[lControlPointIndex].push_back(lPolygonIndex * kTriangleVertexCount + lVertexIndex);
      }
   }
}

void SingleFbxMesh::_addBoneInfluence(
   SingleFbxMesh::tSkinnedVerticeVector& skinnedVerticeVector,
   long vertexIndex,
   long boneIndex,
   double boneWeight)
{
   unsigned long integerWeight = clamp(static_cast<unsigned long>(boneWeight * 255.0 + 0.5), unsigned long(0), unsigned long(255));

   if (0 == integerWeight)
   {
      return;
   }

   tPackedInt packedIndices;
   tPackedInt packedWeights;
   unsigned long smallestWeightIndex = 0xFFFFFFFF;
   unsigned long smallestWeight = 0xFFFFFFFF;

   packedIndices.number = skinnedVerticeVector[vertexIndex].boneIndices;
   packedWeights.number = skinnedVerticeVector[vertexIndex].boneWeights;

   for (unsigned long i = 0; i < 4; ++i)
   {
      unsigned long packedWeight = packedWeights.bytes[i];

      if (packedWeight == 0)
      {
         // Easy case! We found an empty slot.
         smallestWeightIndex = i;
         smallestWeight = 0;
         // We found an empty slot, but we keep looking in case of a duplicate.
      }
      else if ((packedWeight < boneWeight)
         && (packedWeight < smallestWeight))
      {
         // This slot is taken, but remember it incase we are full, and it's smaller
         // than the new weight.
         smallestWeightIndex = i;
         smallestWeight = packedWeight;
      }

      if (packedIndices.bytes[i] == boneIndex)
      { // This is a duplicate!?!
         return;
      }
   }

   if (smallestWeightIndex == 0xFFFFFFFF)
   {
      // We had more than four weights, and this weight is the smallest.
      return;
   }

   packedIndices.bytes[smallestWeightIndex] = static_cast<unsigned char>(boneIndex);
   packedWeights.bytes[smallestWeightIndex] = static_cast<unsigned char>(integerWeight);
   skinnedVerticeVector[vertexIndex].boneIndices = packedIndices.number;
   skinnedVerticeVector[vertexIndex].boneWeights = packedWeights.number;
}

void SingleFbxMesh::_calculatePaletteMatrices()
{
   for (auto& bone : m_boneVector)
   {
      bone.boneMatrice = bone.offset * bone.combinedTransform;
   }
}

void SingleFbxMesh::_loadNodeLocalTransformMatrices(
   const FbxTime& fbxTime)
{
   for (unsigned long i = 0; i < m_boneVector.size(); ++i)
   {
      _getNodeLocalTransform(m_boneVector[i].boneNodePtr, fbxTime, m_boneVector[i].nodeLocalTransform);
   }
}

void SingleFbxMesh::_loadBoneMatriceVector()
{
   m_boneMatrixVector.clear();

   for (const auto& bone : m_boneVector)
   {
      m_boneMatrixVector.push_back(bone.boneMatrice);
   }

   m_effectPtr->SetMatrixArray(
      "gBoneMatrices",
      (D3DXMATRIX const*)m_boneMatrixVector.data(),
      (UINT)m_boneMatrixVector.size());
}

unsigned long long SingleFbxMesh::_getAnimationDuration()
{
   FbxArray<FbxString*> animStackNameArray;
   FbxAnimStack* fbxAnimStackPtr = m_scenePtr->GetSrcObject<FbxAnimStack>(0);
   if (fbxAnimStackPtr== nullptr)
   {

   }
   assert(nullptr != fbxAnimStackPtr);

   m_scenePtr->FillAnimStackNameArray(animStackNameArray);

   int animStackNameArraySize = animStackNameArray.GetCount();
   if (animStackNameArraySize < 1)
   {

   }
   assert(animStackNameArraySize >= 1);

   FbxTakeInfo* currentTakeInfoPtr = m_scenePtr->GetTakeInfo(*animStackNameArray[0]);
   
   assert(nullptr != currentTakeInfoPtr);

   return currentTakeInfoPtr->mLocalTimeSpan.GetStop().GetMilliSeconds()
      - currentTakeInfoPtr->mLocalTimeSpan.GetStart().GetMilliSeconds();
}
