#pragma once
#include "headersSTD.h"
namespace TexturesManager
{
	class eopTexture
	{
	public:
		eopTexture() = delete;
		eopTexture(const std::string& path);
		~eopTexture();
		LPDIRECT3DTEXTURE9 getImage();
	private:
		LPDIRECT3DTEXTURE9 Tex = nullptr;
		int XSize = 0;
		int YSize = 0;
	};
	std::shared_ptr<eopTexture> LoadTexture(uint32_t id, const string& relativePath);
	void UnloadTexture(uint32_t id);
	LPDIRECT3DTEXTURE9 GetImage(uint32_t id);
};

