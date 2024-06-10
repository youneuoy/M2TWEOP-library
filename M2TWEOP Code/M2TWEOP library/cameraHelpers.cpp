#include "cameraHelpers.h"
#include "plugData.h"
namespace cameraHelpers
{
	void moveStratCamera(int x, int y)
	{
		(*(*plugData::data.funcs.moveStratCameraSlow))(x, y);
	}
	void snapStratCamera(int x, int y)
	{
		(*(*plugData::data.funcs.moveStratCameraFast))(x, y);
	}
	void zoomStratCamera(float zoom)
	{
		(*(*plugData::data.funcs.zoomStratCamera))(zoom);
	}
}

