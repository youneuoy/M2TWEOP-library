#include "fbxModels.h"
#include "SingleFbxMesh.h"
#include "fbxSdk.h"


namespace fbxModels
{
	class fbxObject
	{
	public:
		fbxObject(SingleFbxMesh* model, int objectID);
		int ID;
		SingleFbxMesh* mesh;
		void draw(D3DXMATRIX& worldViewProj);

		void setCoords(float* newCoords) 
		{
			coords[0] = newCoords[0];
			coords[1]= newCoords[1];
			coords[2]= newCoords[2];
		}
		float* getCoords() { return coords; }

		void setSize(float newSize) {size = newSize;}
		float getSize() {return size;}

		void setDrawState(bool newState) { isDraw = newState; }
		bool getDrawState() { return isDraw; }

		void setAnimate(bool newAnimate) { isAnimate = newAnimate; }
		bool getAnimate() { return isAnimate; }

		void setType(int newType) { objectType = newType; }
		int getType() {return objectType; }
	private:

		//1-stratmap
		//2-tactmap
		//0-both
		int objectType = 1;
		bool isDraw = true;
		bool isAnimate = true;
		float coords[3];
		float size = 0.001f;

	};
	struct fbxModelsDataS
	{
		LPDIRECT3DDEVICE9 currDevice = nullptr;
		vector<SingleFbxMesh*>models;
		vector<fbxObject*>objects;
	}data;

	void draw(int drawType, int gameVer)
	{
		// Set up world matrix
		D3DXMATRIXA16 matWorld;
		//D3DXMatrixRotationY(&matWorld, 1);
		D3DXMatrixIdentity(&matWorld);


		D3DXMATRIXA16 matView;

		float camCoords[3];
		float camPitch;
		if (gameVer == 2)//steam
		{
			techFuncs::Read(0x0193D538, &camCoords, 4 * 3);
			techFuncs::Read(0x0193D628, &camPitch, 4);
		}
		else
		{
			techFuncs::Read(0x01986698, &camCoords, 4 * 3);
			techFuncs::Read(0x01986778, &camPitch, 4);
		}
	//	D3DXMATRIXA16 camTransMat;

		//D3DXMatrixTranslation(&camTransMat, -camCoords[0], -camCoords[1], -camCoords[2]);



		//D3DXMATRIXA16 camRotMat;
		//D3DXMatrixRotationX(&camRotMat, -camPitch / 2 * D3DX_PI);
		//D3DXMatrixMultiply(&matView, &camRotMat, &camTransMat);
		if (gameVer == 2)//steam
		{
			techFuncs::Read(0x0193D604, &matView, 16 * 4);
		}
		else
		{
			techFuncs::Read(0x01986754, &matView, 16 * 4);
		}
		D3DXMATRIXA16 matProj;
		//D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		if (gameVer == 2)//steam
		{
			techFuncs::Read(0x02C9E0F8, &matProj, 16 * 4);

		}
		else
		{
			techFuncs::Read(0x02ce7098, &matProj, 16 * 4);
		}


		D3DXMATRIXA16 worldViewProj;

		D3DXMATRIX matScale;
		for (fbxObject* obj : data.objects)
		{
			if (obj->getDrawState() == false)
			{
				continue;
			}
			int objType = obj->getType();
			if (objType != 0 && objType != drawType)
			{
				continue;
			}
			float scale = obj->getSize();
			D3DXMatrixScaling(&matScale, scale, scale, scale);

			float* coords = obj->getCoords();

			//model moving
			/*float xCoord = coords[0];
			if (xCoord > 50)
			{
				xCoord = 0;
			}
			else
			{
				xCoord += 0.01;
			}
			coords[0] = xCoord;*/
			matWorld[12] = coords[0];
			matWorld[13] = coords[1];
			matWorld[14] = coords[2];

			worldViewProj = matScale * matWorld * matView * matProj;
			obj->draw(worldViewProj);
		}
	}
	void set3dDevice(LPDIRECT3DDEVICE9 device)
	{
		data.currDevice = device;
	}

	NOINLINE EOP_EXPORT bool addFbxModel(const char* path, const char* texture, int modelID)
	{
		std::string fPath = globals::dataS.modPatch;
		fPath += "\\eopData\\shaders\\skinned.fx";


		data.models.push_back(new SingleFbxMesh(modelID));

		SingleFbxMesh* newModel = (SingleFbxMesh*)getFbxModel(modelID);
		newModel->load(data.currDevice, path, texture, fPath.c_str(), 50);
		SingleFbxMesh* newModel2 = (SingleFbxMesh*)getFbxModel(modelID);
		return true;
	}
	
	NOINLINE EOP_EXPORT void* getFbxModel(int modelID)
	{
		for (SingleFbxMesh* mod : data.models)
		{
			if (mod->modelID == modelID)
			{
				return (void*)mod;
			}
		}

		return nullptr;
	}

	NOINLINE EOP_EXPORT bool deleteFbxModel(int modelID)
	{
		for (auto& object : data.objects)
		{
			if (object->mesh->modelID == modelID)
			{
				delete object;
				object = nullptr;
			}
		}
		data.objects.erase(std::remove(data.objects.begin(), data.objects.end(), nullptr), data.objects.end());



		for (auto& model : data.models)
		{
			if (model->modelID == modelID)
			{
				delete model;
				model = nullptr;
			}
		}
		data.models.erase(std::remove(data.models.begin(), data.models.end(), nullptr), data.models.end());

		return true;
	}

	NOINLINE EOP_EXPORT void* addFbxObject(int modelID, int objectID)
	{
		SingleFbxMesh* model = (SingleFbxMesh*)getFbxModel(modelID);


		if (model == nullptr)
		{
			return nullptr;
		}



		data.objects.push_back(new fbxObject(model, objectID));


		return getFbxObject(objectID);
	}

	NOINLINE EOP_EXPORT bool deleteFbxObject(void* object)
	{
		for (auto& objectT : data.objects)
		{
			if (objectT == object)
			{
				delete objectT;
				objectT = nullptr;
			}
		}
		data.objects.erase(std::remove(data.objects.begin(), data.objects.end(), nullptr), data.objects.end());

		return true;
	}

	NOINLINE EOP_EXPORT void* getFbxObject(int objectID)
	{
		for (fbxObject* obj : data.objects)
		{
			if (obj->ID == objectID)
			{
				return (void*)obj;
			}
		}

		return nullptr;
	}

	NOINLINE EOP_EXPORT void setFbxObjectCoords(void* object, float* coords)
	{
		fbxObject* currObject = reinterpret_cast<fbxObject*>(object);
		currObject->setCoords(coords);
	}

	NOINLINE EOP_EXPORT float* getFbxObjectCoords(void* object)
	{
		fbxObject* currObject = reinterpret_cast<fbxObject*>(object);

		return currObject->getCoords();
	}

	NOINLINE EOP_EXPORT void setFbxObjectSize(void* object, float size)
	{
		fbxObject* currObject = reinterpret_cast<fbxObject*>(object);

		currObject->setSize(size);
	}

	NOINLINE EOP_EXPORT float getFbxObjectSize(void* object)
	{
		fbxObject* currObject = reinterpret_cast<fbxObject*>(object);

		return currObject->getSize();
	}

	NOINLINE EOP_EXPORT void setFbxObjectDrawState(void* object, bool isDrawNeed)
	{
		fbxObject* currObject = reinterpret_cast<fbxObject*>(object);

		currObject->setDrawState(isDrawNeed);
	}

	NOINLINE EOP_EXPORT bool getFbxObjectDrawState(void* object)
	{
		fbxObject* currObject = reinterpret_cast<fbxObject*>(object);

		return currObject->getDrawState();
	}

	NOINLINE EOP_EXPORT void setFbxObjectAnimState(void* object, bool isAnimate)
	{
		fbxObject* currObject = reinterpret_cast<fbxObject*>(object);

		currObject->setAnimate(isAnimate);
	}

	NOINLINE EOP_EXPORT bool getFbxObjectAnimState(void* object)
	{
		fbxObject* currObject = reinterpret_cast<fbxObject*>(object);

		return currObject->getAnimate();
	}




	fbxObject::fbxObject(SingleFbxMesh* model, int objectID)
		:mesh(model)
		,ID(objectID)
	{
	}

	void fbxObject::draw(D3DXMATRIX& worldViewProj)
	{
		mesh->advanceTime(isAnimate);
		mesh->render(worldViewProj);
	}

};

