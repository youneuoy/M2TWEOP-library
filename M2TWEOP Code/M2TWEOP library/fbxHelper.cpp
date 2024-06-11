#include "fbxHelper.h"
namespace fbxHelper
{
    struct
    {
        FbxManager* sdkManager=nullptr;
    }data;
    static  FbxManager* initFbxManager()
    {
        FbxManager*fbxMan= FbxManager::Create();

        // Create an IOSettings object. This object holds all import settings.
        FbxIOSettings* ioSettingsPtr = FbxIOSettings::Create(fbxMan, IOSROOT);
        fbxMan->SetIOSettings(ioSettingsPtr);

        // I probably could have made more things false for this example. You might want
        // to revisit this.
        fbxMan->GetIOSettings()->SetBoolProp(IMP_FBX_CONSTRAINT, false);
        fbxMan->GetIOSettings()->SetBoolProp(IMP_FBX_CONSTRAINT_COUNT, false);
        fbxMan->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, true);
        fbxMan->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, true);
        fbxMan->GetIOSettings()->SetBoolProp(IMP_FBX_LINK, true);
        fbxMan->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, true);
        fbxMan->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO, false);
        fbxMan->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, true);
        fbxMan->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

        return fbxMan;
    }
    FbxManager* getFbxManager()
    {
        if (data.sdkManager == nullptr)
        {
            data.sdkManager = initFbxManager();
        }

        return  data.sdkManager;
    }
};