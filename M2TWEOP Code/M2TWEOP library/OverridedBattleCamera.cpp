#include "OverridedBattleCamera.h"
#include "jsonManager.h"
#include <chrono>
#define _USE_MATH_DEFINES
#include <math.h>


void OverridedBattleCamera::UpdateState(bool isDisable)
{
	Patcher.UpdateState(isDisable);
}

void OverridedBattleCamera::OnWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CurrentWindow = hWnd;
	if (IsFeatureEnabled == false)
	{
		return;
	}

	if (uMsg != WM_MOUSEWHEEL)
	{
		return;
	}
	if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
	{
		TmpData.scrollPos += 1;
	}
	else if(GET_WHEEL_DELTA_WPARAM(wParam) < 0)
	{
		TmpData.scrollPos += -1;
	}
}

void OverridedBattleCamera::Tick()
{
	if (IsPressed(Config.EXIT))
	{
		IsFeatureEnabled = !IsFeatureEnabled;
		UpdateState(IsFeatureEnabled);
		UpdateTmp();
		return;
	}
	if (IsPressed(Config.PAUSE))
	{
		IsFeatureEnabled = !IsFeatureEnabled;
		UpdateState(IsFeatureEnabled);
		UpdateTmp();
		return;
	}
	if (IsFeatureEnabled == false)
	{
		UpdateTmp();
		return;
	}
	if (*Variables.BATTLE == 0)
	{
		UpdateState(true);
		UpdateTmp();
		return;
	}
	else
	{
		UpdateState(false);
	}
	POINT point = { 0,0 };
	GetCursorPos(&point);
	RECT window;
	GetWindowRect(CurrentWindow, &window);

	bool fast_press = IsPressed(Config.FAST);
	bool slow_press = IsPressed(Config.SLOW);
	float horizontal_speed = Config.HORIZONTAL_SPEED;
	float vertical_speed = Config.VERTICAL_SPEED;

	if (fast_press && slow_press)
		horizontal_speed *= Config.BOTH_MOD, vertical_speed *= Config.BOTH_MOD;
	else if (fast_press)
		horizontal_speed *= Config.FAST_MOD, vertical_speed *= Config.FAST_MOD;
	else if (slow_press)
		horizontal_speed *= Config.SLOW_MOD, vertical_speed *= Config.SLOW_MOD;


	float x_acc = 0;
	float y_acc = 0;
	float z_acc = 0;

	float pitch_acc = 0;
	float yaw_acc = 0;

	//################# VANILLA FUNCTION RETENTION ####################
	//Double click detection
	if (IsPressed(VK_LBUTTON))
	{
		if (TmpData.lButtonHeld == false)
		{
			TmpData.oldClickTime = TmpData.clickTime;
			TmpData.clickTime = chrono::duration_cast<chrono::milliseconds>(chrono::time_point_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()).time_since_epoch()).count();
			if (TmpData.clickTime - TmpData.oldClickTime < GetDoubleClickTime() && abs(TmpData.oldClickPoint.x - point.x) < 2 && abs(TmpData.oldClickPoint.y - point.y) < 2)
			{
				UpdateState(true);
			}
			TmpData.oldClickPoint.x = point.x;
			TmpData.oldClickPoint.y = point.y;
		}
		TmpData.lButtonHeld = true;
	}
	else
	{
		TmpData.lButtonHeld = false;
	}


	//################# CAMERA POSITION ####################
	if (IsPressed(Config.MOVE_FORWARD))
	{
		y_acc += sin(0 + TmpData.yaw), x_acc += cos(0 + TmpData.yaw);
		UpdateState(false);
	}

	if (IsPressed(Config.MOVE_BACKWARD))
	{
		y_acc += sin(M_PI + TmpData.yaw), x_acc += cos(M_PI + TmpData.yaw);
		UpdateState(false);
	}

	if (IsPressed(Config.MOVE_LEFT))
	{
		y_acc += sin(M_PI / 2 + TmpData.yaw), x_acc += cos(M_PI / 2 + TmpData.yaw);
		UpdateState(false);
	}

	if (IsPressed(Config.MOVE_RIGHT))
	{
		y_acc += sin(3 * (M_PI / 2) + TmpData.yaw), x_acc += cos(3 * (M_PI / 2) + TmpData.yaw);
		UpdateState(false);
	}

	//Up and down
	float diff = (TmpData.scrollPos - TmpData.oldScrollPos);
	diff = Config.INVERT_SCROLL ? -diff : diff;
	int isNegative = (diff != 0 ? (abs(diff) / diff) : 1);
	TmpData.z_vel += (pow(diff, 2) * isNegative) * vertical_speed / 10.0f;  //Kinda ugly

	if (IsPressed(Config.MOVE_UP))
	{
		z_acc -= 1;
		UpdateState(false);
	}

	if (IsPressed(Config.MOVE_DOWN))
	{
		z_acc -= -1;
		UpdateState(false);
	}
	//################# CAMERA ORIENTATION ####################
	//FREEEECAAAAAAAAM
	float adjusted_sensitivty = (Config.SENSITIVITY * (1 - Config.CAMERA_SMOOTHENING));
	if (IsPressed(Config.FREECAM))
	{
		float invert = Config.INVERT_MOUSE ? -1.0 : 1.0;
		pitch_acc -= (float)(invert * (point.y - TmpData.oldPoint.y)) / (500.0f) * adjusted_sensitivty;
		yaw_acc -= (float)(invert * (point.x - TmpData.oldPoint.x)) / (500.0f) * adjusted_sensitivty;
		UpdateState(false);
	}

	//Edge Detection
	float adjusted_panSpeed = (Config.PAN_SPEED * (1 - Config.CAMERA_SMOOTHENING));
	if (Config.ENABLE_EDGEPAN)
	{
		if (point.x <= window.left + Config.EDGEPAN_MARGIN)
			yaw_acc += (3.0f / 100.0f) * adjusted_panSpeed,
			UpdateState(false);

		else if (point.x >= window.right - Config.EDGEPAN_MARGIN)
			yaw_acc -= (3.0f / 100.0f) * adjusted_panSpeed,
			UpdateState(false);

		if (point.y <= window.top + Config.EDGEPAN_MARGIN)
			y_acc += sin(0 + TmpData.yaw), x_acc += cos(0 + TmpData.yaw),
			UpdateState(false);

		else if (point.y >= window.bottom - Config.EDGEPAN_MARGIN)
			y_acc += sin(M_PI + TmpData.yaw), x_acc += cos(M_PI + TmpData.yaw),
			UpdateState(false);
	}


	//Keycontrolled orientation
	if (Config.ROTATION_CONTROLS)
	{
		if (IsPressed(Config.ROTATE_UP))
			pitch_acc += (3.0f / 100.0f) * adjusted_panSpeed,
			UpdateState(false);

		if (IsPressed(Config.ROTATE_DOWN))
			pitch_acc -= (3.0f / 100.0f) * adjusted_panSpeed,
			UpdateState(false);

		if (IsPressed(Config.ROTATE_LEFT))
			yaw_acc += (3.0f / 100.0f) * adjusted_panSpeed,
			UpdateState(false);

		if (IsPressed(Config.ROTATE_RIGHT))
			yaw_acc -= (3.0f / 100.0f) * adjusted_panSpeed,
			UpdateState(false);
	}


	if (IsPressed(VK_MBUTTON))
		UpdateState(true);


	//################# ETC ####################
	float length = sqrt(pow(x_acc, 2) + pow(y_acc, 2) + pow(z_acc, 2));
	if (length == 0) // 0/0 avoidance
		length = 1;
	TmpData.x_vel += ((x_acc / length) * (horizontal_speed * (1 - Config.HORIZONTAL_SMOOTHENING))) / 2.0f;  //Weird math is to account for smoothening
	TmpData.y_vel += ((y_acc / length) * (horizontal_speed * (1 - Config.HORIZONTAL_SMOOTHENING))) / 2.0f;
	TmpData.z_vel += ((z_acc / length) * (vertical_speed * (1 - Config.VERTICAL_SMOOTHENING))) / 2.0f;
	TmpData.camera_x += TmpData.x_vel;
	TmpData.camera_y += TmpData.y_vel;
	TmpData.camera_z += TmpData.z_vel;
	TmpData.x_vel *= Config.HORIZONTAL_SMOOTHENING;
	TmpData.y_vel *= Config.HORIZONTAL_SMOOTHENING;
	TmpData.z_vel *= Config.VERTICAL_SMOOTHENING;

	TmpData.pitch_vel += pitch_acc;
	TmpData.yaw_vel += yaw_acc;
	TmpData.pitch += TmpData.pitch_vel;
	TmpData.yaw += TmpData.yaw_vel;
	TmpData.pitch_vel *= Config.CAMERA_SMOOTHENING;
	TmpData.yaw_vel *= Config.CAMERA_SMOOTHENING;

	TmpData.oldPoint.x = point.x, TmpData.oldPoint.y = point.y;

	TmpData.camera_x = min(max(TmpData.camera_x, -900), 900);
	TmpData.camera_y = min(max(TmpData.camera_y, -900), 900);

	if (IsFeatureEnabled == true)
	{
		WriteTmp();
	}
	else
	{
		UpdateTmp();
	}

	TmpData.oldScrollPos = TmpData.scrollPos;
}
void OverridedBattleCamera::UpdateTmp()
{
	TmpData.camera_x = *Variables.CAMERA_X;
	TmpData.camera_y = *Variables.CAMERA_Y;
	TmpData.camera_z = *Variables.CAMERA_Z;


	TmpData.target_x = *Variables.TARGET_X;
	TmpData.target_y = *Variables.TARGET_Y;
	TmpData.target_z = *Variables.TARGET_Z;

	float length = sqrt(pow(TmpData.target_x - TmpData.camera_x, 2) + pow(TmpData.target_y - TmpData.camera_y, 2) + pow(TmpData.target_z - TmpData.camera_z, 2));
	TmpData.pitch = asin((TmpData.target_z - TmpData.camera_z) / length);
	TmpData.yaw = atan2((TmpData.target_y - TmpData.camera_y) / length, (TmpData.target_x - TmpData.camera_x) / length);
	if (isnan(TmpData.pitch))
	{
		TmpData.pitch = 0;
	}
	if (isnan(TmpData.yaw))
	{
		TmpData.yaw = 0;
	}
}
void OverridedBattleCamera::WriteTmp()
{
	if (Config.OVERRIDE_MOVEMENT)
	{
		*Variables.CAMERA_X = TmpData.camera_x;
		*Variables.CAMERA_Y = TmpData.camera_y;
		*Variables.CAMERA_Z = TmpData.camera_z;

	}
	else
	{
		TmpData.camera_x = *Variables.CAMERA_X;
		TmpData.camera_y = *Variables.CAMERA_Y;
		TmpData.camera_z = *Variables.CAMERA_Z;
	}

	TmpData.pitch = max(TmpData.pitch, -(M_PI / 2.0f) * 0.9f);
	TmpData.pitch = min(TmpData.pitch, (M_PI / 2.0f) * 0.9f);
	TmpData.target_x = (cos(TmpData.yaw) * cos(TmpData.pitch) * 1000.0f) + TmpData.camera_x;
	TmpData.target_y = (sin(TmpData.yaw) * cos(TmpData.pitch) * 1000.0f) + TmpData.camera_y;
	TmpData.target_z = (sin(TmpData.pitch) * 1000.0f) + TmpData.camera_z;

	*Variables.TARGET_X = TmpData.target_x;
	*Variables.TARGET_Y = TmpData.target_y;
	*Variables.TARGET_Z = TmpData.target_z;
}
void OverridedBattleCamera::Init(int gameVersion, const std::string& modPath)
{
	Patcher.Init(gameVersion);
	InitVariables(gameVersion);
	InitKeys();
	LoadJsonCfg(modPath);
	IsFeatureEnabled = true;

	GetCursorPos(&TmpData.oldPoint);
	UpdateTmp();
}



void OverridedBattleCamera::LoadJsonCfg(const std::string& modPath)
{
	std::string fPath = modPath;
	fPath += "\\eopData\\OverridedBattleCameraCfg.json";

	try
	{
		jsn::json json = fastFunctsHelpers::loadJsonFromFile(fPath);
		std::string tmpCfgString;
		if (json.contains("MOVE_FORWARD"))
		{
			getJson(tmpCfgString, "MOVE_FORWARD");
			Config.MOVE_FORWARD = GetVK_Key(tmpCfgString);
		}
		if (json.contains("MOVE_LEFT"))
		{
			getJson(tmpCfgString, "MOVE_LEFT");
			Config.MOVE_LEFT = GetVK_Key(tmpCfgString);
		}
		if (json.contains("MOVE_BACKWARD"))
		{
			getJson(tmpCfgString, "MOVE_BACKWARD");
			Config.MOVE_BACKWARD = GetVK_Key(tmpCfgString);
		}
		if (json.contains("MOVE_RIGHT"))
		{
			getJson(tmpCfgString, "MOVE_RIGHT");
			Config.MOVE_RIGHT = GetVK_Key(tmpCfgString);
		}
		if (json.contains("MOVE_UP"))
		{
			getJson(tmpCfgString, "MOVE_UP");
			Config.MOVE_UP = GetVK_Key(tmpCfgString);
		}
		if (json.contains("MOVE_DOWN"))
		{
			getJson(tmpCfgString, "MOVE_DOWN");
			Config.MOVE_DOWN = GetVK_Key(tmpCfgString);
		}
		if (json.contains("ROTATE_LEFT"))
		{
			getJson(tmpCfgString, "ROTATE_LEFT");
			Config.ROTATE_LEFT = GetVK_Key(tmpCfgString);
		}
		if (json.contains("ROTATE_RIGHT"))
		{
			getJson(tmpCfgString, "ROTATE_RIGHT");
			Config.ROTATE_RIGHT = GetVK_Key(tmpCfgString);
		}
		if (json.contains("ROTATE_UP"))
		{
			getJson(tmpCfgString, "ROTATE_UP");
			Config.ROTATE_UP = GetVK_Key(tmpCfgString);
		}
		if (json.contains("ROTATE_DOWN"))
		{
			getJson(tmpCfgString, "ROTATE_DOWN");
			Config.ROTATE_DOWN = GetVK_Key(tmpCfgString);
		}
		if (json.contains("EXIT"))
		{
			getJson(tmpCfgString, "EXIT");
			Config.EXIT = GetVK_Key(tmpCfgString);
		}
		if (json.contains("PAUSE"))
		{
			getJson(tmpCfgString, "PAUSE");
			Config.PAUSE = GetVK_Key(tmpCfgString);
		}
		if (json.contains("FAST"))
		{
			getJson(tmpCfgString, "FAST");
			Config.FAST = GetVK_Key(tmpCfgString);
		}
		if (json.contains("SLOW"))
		{
			getJson(tmpCfgString, "SLOW");
			Config.SLOW = GetVK_Key(tmpCfgString);
		}
		if (json.contains("FREECAM"))
		{
			getJson(tmpCfgString, "FREECAM");
			Config.FREECAM = GetVK_Key(tmpCfgString);
		}
		if (json.contains("SENSITIVITY"))
		{
			getJson(Config.SENSITIVITY, "SENSITIVITY");
		}
		if (json.contains("HORIZONTAL_SMOOTHENING"))
		{
			getJson(Config.HORIZONTAL_SMOOTHENING, "HORIZONTAL_SMOOTHENING");
			Config.HORIZONTAL_SMOOTHENING = max(min(Config.HORIZONTAL_SMOOTHENING, 0.99999999f), 0.f);
		}
		if (json.contains("VERTICAL_SMOOTHENING"))
		{
			getJson(Config.VERTICAL_SMOOTHENING, "VERTICAL_SMOOTHENING");
			Config.VERTICAL_SMOOTHENING = max(min(Config.VERTICAL_SMOOTHENING, 0.99999999f), 0.f);
		}
		if (json.contains("CAMERA_SMOOTHENING"))
		{
			getJson(Config.CAMERA_SMOOTHENING, "CAMERA_SMOOTHENING");
			Config.CAMERA_SMOOTHENING = max(min(Config.CAMERA_SMOOTHENING, 0.99999999f), 0.f);
		}
		if (json.contains("HORIZONTAL_SPEED"))
		{
			getJson(Config.HORIZONTAL_SPEED, "HORIZONTAL_SPEED");
		}
		if (json.contains("VERTICAL_SPEED"))
		{
			getJson(Config.VERTICAL_SPEED, "VERTICAL_SPEED");
		}
		if (json.contains("PAN_SPEED"))
		{
			getJson(Config.VERTICAL_SPEED, "PAN_SPEED");
		}
		if (json.contains("FAST_MOD"))
		{
			getJson(Config.FAST_MOD, "FAST_MOD");
		}
		if (json.contains("SLOW_MOD"))
		{
			getJson(Config.SLOW_MOD, "SLOW_MOD");
		}
		if (json.contains("BOTH_MOD"))
		{
			getJson(Config.BOTH_MOD, "BOTH_MOD");
		}
		if (json.contains("INVERT_MOUSE"))
		{
			getJson(Config.INVERT_MOUSE, "INVERT_MOUSE");
		}
		if (json.contains("INVERT_SCROLL"))
		{
			getJson(Config.INVERT_SCROLL, "INVERT_SCROLL");
		}
		if (json.contains("ENABLE_EDGEPAN"))
		{
			getJson(Config.ENABLE_EDGEPAN, "ENABLE_EDGEPAN");
		}
		if (json.contains("ROTATION_CONTROLS"))
		{
			getJson(Config.ROTATION_CONTROLS, "ROTATION_CONTROLS");
		}
		if (json.contains("OVERRIDE_MOVEMENT"))
		{
			getJson(Config.OVERRIDE_MOVEMENT, "OVERRIDE_MOVEMENT");
		}
		if (json.contains("EDGEPAN_MARGIN"))
		{
			getJson(Config.EDGEPAN_MARGIN, "EDGEPAN_MARGIN");
		}
		if (json.contains("UPS"))
		{
			getJson(Config.UPS, "UPS");
		}
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
	}
}

void OverridedBattleCamera::InitVariables(int gameVersion)
{
	if (gameVersion == 2)//steam
	{
		Variables.BATTLE = reinterpret_cast<int*>(0x0193D683);
		Variables.TARGET_X = reinterpret_cast<float*>(0x0193D5DC);
		Variables.TARGET_Y = reinterpret_cast<float*>(0x0193D5E4);
		Variables.TARGET_Z = reinterpret_cast<float*>(0x0193D5E0);
		Variables.CAMERA_X = reinterpret_cast<float*>(0x0193D598);
		Variables.CAMERA_Y = reinterpret_cast<float*>(0x0193D5A0);
		Variables.CAMERA_Z = reinterpret_cast<float*>(0x0193D59C);
	}
	else if (gameVersion == 1)//kingdoms
	{
		Variables.BATTLE = reinterpret_cast<int*>(0x019867D3);
		Variables.TARGET_X = reinterpret_cast<float*>(0x0198672C);
		Variables.TARGET_Y = reinterpret_cast<float*>(0x01986734);
		Variables.TARGET_Z = reinterpret_cast<float*>(0x01986730);
		Variables.CAMERA_X = reinterpret_cast<float*>(0x019866E8);
		Variables.CAMERA_Y = reinterpret_cast<float*>(0x019866F0);
		Variables.CAMERA_Z = reinterpret_cast<float*>(0x019866EC);
	}
}

void OverridedBattleCamera::InitKeys()
{
	KeyFinder.emplace("VK_LBUTTON", 0x01);
	KeyFinder.emplace("VK_RBUTTON", 0x02);
	KeyFinder.emplace("VK_CANCEL", 0x03);
	KeyFinder.emplace("VK_MBUTTON", 0x04);
	KeyFinder.emplace("VK_XBUTTON1", 0x05);
	KeyFinder.emplace("VK_XBUTTON2", 0x06);
	KeyFinder.emplace("VK_BACK", 0x08);
	KeyFinder.emplace("VK_TAB", 0x09);
	KeyFinder.emplace("VK_CLEAR", 0x0C);
	KeyFinder.emplace("VK_RETURN", 0x0D);
	KeyFinder.emplace("VK_SHIFT", 0x10);
	KeyFinder.emplace("VK_CONTROL", 0x11);
	KeyFinder.emplace("VK_MENU", 0x12);
	KeyFinder.emplace("VK_PAUSE", 0x13);
	KeyFinder.emplace("VK_CAPITAL", 0x14);
	KeyFinder.emplace("VK_KANA", 0x15);
	KeyFinder.emplace("VK_HANGUEL", 0x15);
	KeyFinder.emplace("VK_HANGUL", 0x15);
	KeyFinder.emplace("VK_IME_ON", 0x16);
	KeyFinder.emplace("VK_JUNJA", 0x17);
	KeyFinder.emplace("VK_FINAL", 0x18);
	KeyFinder.emplace("VK_HANJA", 0x19);
	KeyFinder.emplace("VK_KANJI", 0x19);
	KeyFinder.emplace("VK_IME_OFF", 0x1A);
	KeyFinder.emplace("VK_ESCAPE", 0x1B);
	KeyFinder.emplace("VK_CONVERT", 0x1C);
	KeyFinder.emplace("VK_NONCONVERT", 0x1D);
	KeyFinder.emplace("VK_ACCEPT", 0x1E);
	KeyFinder.emplace("VK_MODECHANGE", 0x1F);
	KeyFinder.emplace("VK_SPACE", 0x20);
	KeyFinder.emplace("VK_PRIOR", 0x21);
	KeyFinder.emplace("VK_NEXT", 0x22);
	KeyFinder.emplace("VK_END", 0x23);
	KeyFinder.emplace("VK_HOME", 0x24);
	KeyFinder.emplace("VK_LEFT", 0x25);
	KeyFinder.emplace("VK_UP", 0x26);
	KeyFinder.emplace("VK_RIGHT", 0x27);
	KeyFinder.emplace("VK_DOWN", 0x28);
	KeyFinder.emplace("VK_SELECT", 0x29);
	KeyFinder.emplace("VK_PRINT", 0x2A);
	KeyFinder.emplace("VK_EXECUTE", 0x2B);
	KeyFinder.emplace("VK_SNAPSHOT", 0x2C);
	KeyFinder.emplace("VK_INSERT", 0x2D);
	KeyFinder.emplace("VK_DELETE", 0x2E);
	KeyFinder.emplace("VK_HELP", 0x2F);
	KeyFinder.emplace("VK_LWIN", 0x5B);
	KeyFinder.emplace("VK_RWIN", 0x5C);
	KeyFinder.emplace("VK_APPS", 0x5D);
	KeyFinder.emplace("VK_SLEEP", 0x5F);
	KeyFinder.emplace("VK_NUMPAD0", 0x60);
	KeyFinder.emplace("VK_NUMPAD1", 0x61);
	KeyFinder.emplace("VK_NUMPAD2", 0x62);
	KeyFinder.emplace("VK_NUMPAD3", 0x63);
	KeyFinder.emplace("VK_NUMPAD4", 0x64);
	KeyFinder.emplace("VK_NUMPAD5", 0x65);
	KeyFinder.emplace("VK_NUMPAD6", 0x66);
	KeyFinder.emplace("VK_NUMPAD7", 0x67);
	KeyFinder.emplace("VK_NUMPAD8", 0x68);
	KeyFinder.emplace("VK_NUMPAD9", 0x69);
	KeyFinder.emplace("VK_MULTIPLY", 0x6A);
	KeyFinder.emplace("VK_ADD", 0x6B);
	KeyFinder.emplace("VK_SEPARATOR", 0x6C);
	KeyFinder.emplace("VK_SUBTRACT", 0x6D);
	KeyFinder.emplace("VK_DECIMAL", 0x6E);
	KeyFinder.emplace("VK_DIVIDE", 0x6F);
	KeyFinder.emplace("VK_F1", 0x70);
	KeyFinder.emplace("VK_F2", 0x71);
	KeyFinder.emplace("VK_F3", 0x72);
	KeyFinder.emplace("VK_F4", 0x73);
	KeyFinder.emplace("VK_F5", 0x74);
	KeyFinder.emplace("VK_F6", 0x75);
	KeyFinder.emplace("VK_F7", 0x76);
	KeyFinder.emplace("VK_F8", 0x77);
	KeyFinder.emplace("VK_F9", 0x78);
	KeyFinder.emplace("VK_F10", 0x79);
	KeyFinder.emplace("VK_F11", 0x7A);
	KeyFinder.emplace("VK_F12", 0x7B);
	KeyFinder.emplace("VK_F13", 0x7C);
	KeyFinder.emplace("VK_F14", 0x7D);
	KeyFinder.emplace("VK_F15", 0x7E);
	KeyFinder.emplace("VK_F16", 0x7F);
	KeyFinder.emplace("VK_F17", 0x80);
	KeyFinder.emplace("VK_F18", 0x81);
	KeyFinder.emplace("VK_F19", 0x82);
	KeyFinder.emplace("VK_F20", 0x83);
	KeyFinder.emplace("VK_F21", 0x84);
	KeyFinder.emplace("VK_F22", 0x85);
	KeyFinder.emplace("VK_F23", 0x86);
	KeyFinder.emplace("VK_F24", 0x87);
	KeyFinder.emplace("VK_NUMLOCK", 0x90);
	KeyFinder.emplace("VK_SCROLL", 0x91);
	KeyFinder.emplace("VK_LSHIFT", 0xA0);
	KeyFinder.emplace("VK_RSHIFT", 0xA1);
	KeyFinder.emplace("VK_LCONTROL", 0xA2);
	KeyFinder.emplace("VK_RCONTROL", 0xA3);
	KeyFinder.emplace("VK_LMENU", 0xA4);
	KeyFinder.emplace("VK_RMENU", 0xA5);
	KeyFinder.emplace("VK_BROWSER_BACK", 0xA6);
	KeyFinder.emplace("VK_BROWSER_FORWARD", 0xA7);
	KeyFinder.emplace("VK_BROWSER_REFRESH", 0xA8);
	KeyFinder.emplace("VK_BROWSER_STOP", 0xA9);
	KeyFinder.emplace("VK_BROWSER_SEARCH", 0xAA);
	KeyFinder.emplace("VK_BROWSER_FAVORITES", 0xAB);
	KeyFinder.emplace("VK_BROWSER_HOME", 0xAC);
	KeyFinder.emplace("VK_VOLUME_MUTE", 0xAD);
	KeyFinder.emplace("VK_VOLUME_DOWN", 0xAE);
	KeyFinder.emplace("VK_VOLUME_UP", 0xAF);
	KeyFinder.emplace("VK_MEDIA_NEXT_TRACK", 0xB0);
	KeyFinder.emplace("VK_MEDIA_PREV_TRACK", 0xB1);
	KeyFinder.emplace("VK_MEDIA_STOP", 0xB2);
	KeyFinder.emplace("VK_MEDIA_PLAY_PAUSE", 0xB3);
	KeyFinder.emplace("VK_LAUNCH_MAIL", 0xB4);
	KeyFinder.emplace("VK_LAUNCH_MEDIA_SELECT", 0xB5);
	KeyFinder.emplace("VK_LAUNCH_APP1", 0xB6);
	KeyFinder.emplace("VK_LAUNCH_APP2", 0xB7);
	KeyFinder.emplace("VK_OEM_1", 0xBA);
	KeyFinder.emplace("VK_OEM_PLUS", 0xBB);
	KeyFinder.emplace("VK_OEM_COMMA", 0xBC);
	KeyFinder.emplace("VK_OEM_MINUS", 0xBD);
	KeyFinder.emplace("VK_OEM_PERIOD", 0xBE);
	KeyFinder.emplace("VK_OEM_2", 0xBF);
	KeyFinder.emplace("VK_OEM_3", 0xC0);
	KeyFinder.emplace("VK_OEM_4", 0xDB);
	KeyFinder.emplace("VK_OEM_5", 0xDC);
	KeyFinder.emplace("VK_OEM_6", 0xDD);
	KeyFinder.emplace("VK_OEM_7", 0xDE);
	KeyFinder.emplace("VK_OEM_8", 0xDF);
	KeyFinder.emplace("VK_OEM_102", 0xE2);
	KeyFinder.emplace("VK_PROCESSKEY", 0xE5);
	KeyFinder.emplace("VK_PACKET", 0xE7);
	KeyFinder.emplace("VK_ATTN", 0xF6);
	KeyFinder.emplace("VK_CRSEL", 0xF7);
	KeyFinder.emplace("VK_EXSEL", 0xF8);
	KeyFinder.emplace("VK_EREOF", 0xF9);
	KeyFinder.emplace("VK_PLAY", 0xFA);
	KeyFinder.emplace("VK_ZOOM", 0xFB);
	KeyFinder.emplace("VK_PA1", 0xFD);
	KeyFinder.emplace("VK_OEM_CLEAR", 0xFE);
}

unsigned int OverridedBattleCamera::GetVK_Key(const std::string& key)
{
	if (key.length() == 1)
		return VkKeyScanExA(tolower(key[0]), GetKeyboardLayout(0));
	else if (key.length() > 1)
		return KeyFinder.at(key);
	else
	{
		throw std::runtime_error("Key is empty");
	}
}

bool OverridedBattleCamera::IsPressed(unsigned int keycode)
{
	return GetKeyState(keycode) < 0;
}
