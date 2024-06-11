#include "TexturesManager.h"
#include "graphicsD3D.h"
namespace TexturesManager
{
	std::unordered_map<uint32_t, std::shared_ptr<eopTexture>> textures;
	std::shared_ptr<eopTexture> LoadTexture(uint32_t id, const string& relativePath)
	{
		try
		{
			auto tex = std::make_shared<eopTexture>(globals::dataS.modPath + relativePath);
			textures[id] = tex;
			return tex;
		}
		catch (int ex)
		{
			//cannot load, ignore
		}

		return nullptr;
	}
	void UnloadTexture(uint32_t id)
	{
		textures.erase(id);
	}
	LPDIRECT3DTEXTURE9 GetImage(uint32_t id)
	{
		try
		{
			auto tex = textures.at(id);
			return tex->getImage();
		}
		catch (...)
		{
			//cannot get, ignore
		}
		return nullptr;
	}

	eopTexture::eopTexture(const std::string& path)
	{
		Tex = graphicsExport::loadTexture(path.c_str(), &XSize, &YSize);
		if (Tex == nullptr)
		{
			throw 1;
		}
	}
	eopTexture::~eopTexture()
	{
		graphicsExport::unloadTexture(Tex);
	}

	LPDIRECT3DTEXTURE9 eopTexture::getImage()
	{
		return Tex;
	}
}