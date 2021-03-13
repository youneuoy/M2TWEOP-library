#include "helpers.h"
#include "dataG.h"
void helpers::loadTexture(d3dImage* image)
{
	HRESULT res = D3DXCreateTextureFromFile(dataG::data.d3d.g_pd3dDevice, image->path.c_str(), &image->image);
	string t = std::system_category().message(res);
	if (res != D3D_OK)
	{
		MessageBoxA(NULL, t.c_str(), "Loading texture err!", MB_OK | MB_ICONASTERISK);
	}


	D3DSURFACE_DESC my_image_desc;
	image->image->GetLevelDesc(0, &my_image_desc);

	image->xSize = (int)my_image_desc.Width;
	image->ySize = (int)my_image_desc.Height;
}

void helpers::updateMetrics()
{
	dataG::data.screen.screenSize.x = GetSystemMetrics(SM_CXSCREEN);
	dataG::data.screen.screenSize.y = GetSystemMetrics(SM_CYSCREEN);

	dataG::data.screen.screenHalfSize.x = dataG::data.screen.screenSize.x / 2;
	dataG::data.screen.screenHalfSize.y = dataG::data.screen.screenSize.y / 2;
}

screenS& helpers::getScreen()
{
	return dataG::data.screen;
}

d3dImage* helpers::findImage(const char* name, int nameLen)
{
	for (d3dImage* img: dataG::data.staticImagesCollection)
	{
		if (img->path.size() != nameLen)continue;

		if (strncmp(name, img->path.c_str(), nameLen) == 0)
		{
			return img;
		}
	}
	return findImage("empty",5);
}

ImFont* helpers::findFont(const char* name)
{
	for (fontS* fnt : dataG::data.staticFontsCollection)
	{

		if (strcmp(name, fnt->name.c_str()) == 0)
		{
			return fnt->font;
		}
	}
	return findFont("mainFont");
}
