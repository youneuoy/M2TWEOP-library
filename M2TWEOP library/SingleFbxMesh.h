// This is a super lazy way of displaying an .fbx mesh.
// If I were doing this properly, I'd use an object to
// encapsulate the Autodesk SDK, another object to
// represent an autodesk model, and a third object to
// represent an autodesk model instance.  You'd need
// even more objects to handle the animation transitions. I've
// written all this code in the past, but for a demo, I want to
// keep it simple. I despise complex demos.

#pragma once

#include <fbxsdk.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include <vector>
#include <string>


#include "fbxHelper.h"
class SingleFbxMesh
{
public:
    SingleFbxMesh(int modID);
    SingleFbxMesh()=delete;
    ~SingleFbxMesh();

    void load(
        LPDIRECT3DDEVICE9 devicePtr,
        const char* meshName,
        const char* textureName,
        unsigned long boneMatrixVectorSize = 50);
    void release();
    void advanceTime(unsigned long long timeMod=1);
    void render(
        const D3DXMATRIX& worldViewProj);
    int modelID;
private:
    // All the necessary pices to keep the Autodesk SDK loaded
    fbxsdk::FbxManager* m_sdkManagerPtr;
    LPDIRECT3DDEVICE9 m_devicePtr;

    // All the necessary pieces to keep an .fbx model loaded

    enum
    {
        kInvalidBoneIndex = -1,
        kTriangleVertexCount = 3,
        kBoneInfluencesPerVertice = 4,
    };

    // This is a stupidly simple way to slice and dice bytes
    // and words. This is great for grabbing color channels
    // and bone influences.
    typedef union
    {
        unsigned char bytes[4];
        unsigned long number;
    } tPackedInt;

    // Skinned mesh definition, we are being lazy and converting
    // everything to a skinned mesh. If we were doing this properly,
    // we would recognized that a mesh has a single bone influence
    // and render it as a static mesh and use the bone position as
    // the world matrice transform.
    typedef struct
    {
        D3DXVECTOR3 point;
        unsigned long boneWeights;
        unsigned long boneIndices;
        D3DXVECTOR3 normal;
        float u;
        float v;
    } tSkinnedVertice;
    typedef std::vector<tSkinnedVertice> tSkinnedVerticeVector;
    typedef tSkinnedVerticeVector::iterator tSkinnedVerticeIterator;
    typedef tSkinnedVerticeVector::const_iterator tSkinnedVerticeConstIterator;

    typedef std::vector<unsigned short> tVertexIndexVector;
    typedef tVertexIndexVector::iterator tVertexIndexIterator;
    typedef tVertexIndexVector::const_iterator tVertexIndexConstIterator;

    typedef std::vector<unsigned char> tBoneIndexVector;
    typedef tBoneIndexVector::iterator tBoneIndexIterator;
    typedef tBoneIndexVector::const_iterator tBoneIndexConstIterator;
    typedef struct
    {
        tBoneIndexVector boneIndexVector;
        tSkinnedVerticeVector subverticeVector;
        tVertexIndexVector indexVector;
        IDirect3DIndexBuffer9* indexBufferPtr;
        IDirect3DVertexBuffer9* vertexBufferPtr;
    } tMeshRec;

    typedef std::vector<tMeshRec> tMeshVector;
    typedef tMeshVector::iterator tMeshIterator;
    typedef tMeshVector::const_iterator tMeshConstIterator;

    typedef struct
    {
        std::string materialName;
        std::string meshName;
        tSkinnedVerticeVector verticeVector;
        tVertexIndexVector indexVector;
        tMeshVector meshVector;
        //IDirect3DIndexBuffer9* indexBufferPtr;
        //IDirect3DVertexBuffer9* vertexBufferPtr;
    } tModelRec;
    typedef std::vector<tModelRec> tModelVector;
    typedef tModelVector::iterator tModelIterator;
    typedef tModelVector::const_iterator tModelConstIterator;

    typedef struct
    {
        std::string name;

        // calculates the locals together depth first
        // equals localTransform * parentMatrix
        D3DXMATRIX combinedTransform;

        // updated for animation
        D3DXMATRIX nodeLocalTransform;

        // This is an inverse of the parent to child matrix,
        // or you can call it the inverse bind pose matrix.
        // It's set at mesh load time and doesn't change.
        D3DXMATRIX offset;

        // Ready for a skinned mesh shader as the palette matrix.
        // equals offset * combinedTransform
        D3DXMATRIX palette;
        fbxsdk::FbxNode* boneNodePtr;

        // Relationship Stuff:
        std::vector<int> childIndexes;
        int parentIndex;
    } tBone;

    // Used to do a reverse lookup.
    typedef std::vector<int> tControlPointIndexes;
    typedef std::vector<tControlPointIndexes> tControlPointRemap;

    typedef std::vector<tBone> tBoneVector;
    typedef tBoneVector::iterator tBoneIterator;
    typedef tBoneVector::const_iterator tBoneConstIterator;

    typedef std::vector<D3DXMATRIX> tMatrixVector;
    typedef tMatrixVector::iterator tMatrixIterator;
    typedef tMatrixVector::const_iterator tMatrixConstIterator;

    std::string m_filename;
    fbxsdk::FbxScene* m_scenePtr;
    int m_majorFileVersion;
    int m_minorFileVersion;
    int m_revisionFileVersion;
    tModelVector m_modelVector;
    std::string m_textureName;
    tBoneVector m_boneVector;
    D3DXMATRIX m_identityMatrix;
    bool m_allByControlPoint;
    unsigned int m_boneMatrixVectorSize;
    tMatrixVector m_boneMatrixVector;
    unsigned long long m_initialAnimationDurationInMs;
    IDirect3DTexture9* m_texturePtr;
    ID3DXEffect* m_effectPtr;
    IDirect3DVertexDeclaration9* m_verticeVectorVertexDeclarationPtr;

    void _loadModelTexture();
    void _loadModelEffect();
    void _loadverticeVectorVertexDeclaration();
    void _getMatrices(
        tMatrixVector& paletteMatrixVector);

    void _buildMatrices(
        const fbxsdk::FbxTime& time);

    void _loadModel();
    void _loadBones(
        fbxsdk::FbxNode* nodePtr,
        long parentBoneIndex);
    void _loadBone(
        fbxsdk::FbxNode* nodePtr,
        long parentBoneIndex);
    void _loadMeshes(
        fbxsdk::FbxNode* nodePtr);
    void _loadMesh(
        fbxsdk::FbxNode* nodePtr);
    void _loadMeshPositionNormalUV(
        fbxsdk::FbxNode* nodePtr,
        tModelRec& meshRec);
    void _compressSkinnedVertices(
        tModelRec& modelRec);
    unsigned short _findSkinnedVertice(
        const tSkinnedVerticeVector& skinnedVerticeVector,
        const tSkinnedVertice& skinnedVertice);
    void _loadMeshBoneWeightsAndIndices(
        fbxsdk::FbxNode* nodePtr,
        tModelRec& modelRec);
    void _normalizeBoneWeights(
        tModelRec& modelRec);
    void _breakModelAlongBoneMatrixArraySize(
        tModelRec& modelRec);
    bool _addTriangleToMeshVector(
        tModelRec& modelRec,
        unsigned long triangleIndex);
    bool _addTriangleToMesh(
        tModelRec& modelRec,
        tMeshRec& meshRec,
        unsigned long triangleIndex);
    bool _addBoneToPalette(
        tMeshRec& meshRec,
        unsigned char& boneIndex); // in/out parameter
    void _loadTextureNames();

    void _releaseFile();

    void _loadMeshBuffers();
    void _releaseMeshBuffers();

    bool _isMeshSkinned(
        fbxsdk::FbxMesh* meshPtr);
    long _getBoneCount(
        fbxsdk::FbxMesh* meshPtr);
    void _getGeometryTransformMatrix(
        fbxsdk::FbxNode* nodePtr,
        D3DXMATRIX& geometryOffsetMatrix);
    void _getNodeLocalTransform(
        fbxsdk::FbxNode* nodePtr,
        D3DXMATRIX& matrix);
    void _getNodeLocalTransform(
        fbxsdk::FbxNode* nodePtr,
        const fbxsdk::FbxTime& fbxTime,
        D3DXMATRIX& matrix);
    void _fbxToMatrix(
        const fbxsdk::FbxAMatrix& fbxMatrix,
        D3DXMATRIX& matrix);
    void _calculateCombinedTransforms();
    long _boneNameToindex(
        const std::string& name);
    void _loadControlPointRemap(
        fbxsdk::FbxMesh* meshPtr,
        tControlPointRemap& controlPointRemap);
    void _addBoneInfluence(
        tSkinnedVerticeVector& skinnedVerticeVector,
        long vertexIndex,
        long boneIndex,
        double boneWeight);

    void _calculatePaletteMatrices();
    void _loadNodeLocalTransformMatrices(
        const fbxsdk::FbxTime& fbxTime);
    void _loadBoneMatriceVector(
        const SingleFbxMesh::tMeshRec& meshRec);

    unsigned long long _getAnimationDuration();
};
