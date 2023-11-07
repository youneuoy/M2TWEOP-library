#pragma once
#include "OverridedBattleCameraPatcher.h"
#include<string>
#include<unordered_map>
class OverridedBattleCamera
{
public:
	void UpdateState(bool isDisable);
	void OnWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Tick();
	void Init(int gameVersion, const std::string& modPath);
private:
	HWND CurrentWindow = NULL;
	void UpdateTmp();
	void WriteTmp();
	bool IsFeatureEnabled = false;
	OverridedBattleCameraPatcher Patcher;
	void LoadJsonCfg(const std::string& modPath);
	struct CfgT
	{
		int MOVE_FORWARD;
		int MOVE_LEFT;
		int MOVE_BACKWARD;
		int MOVE_RIGHT;
		int MOVE_UP;
		int MOVE_DOWN;
		int ROTATE_LEFT;
		int ROTATE_RIGHT;
		int ROTATE_UP;
		int ROTATE_DOWN;
		int EXIT;
		int PAUSE;
		int FAST;
		int SLOW;
		int FREECAM;

		float SENSITIVITY;
		float HORIZONTAL_SMOOTHENING;
		float VERTICAL_SMOOTHENING;
		float CAMERA_SMOOTHENING;

		float HORIZONTAL_SPEED;
		float VERTICAL_SPEED;
		float PAN_SPEED;

		float FAST_MOD;
		float SLOW_MOD;
		float BOTH_MOD;

		bool INVERT_MOUSE;
		bool INVERT_SCROLL;
		bool ENABLE_EDGEPAN;
		bool ROTATION_CONTROLS;
		bool OVERRIDE_MOVEMENT;

		int EDGEPAN_MARGIN;
		int UPS;
	}Config;
	struct VariablesT
	{
		int* BATTLE = nullptr;
		float* TARGET_X = nullptr;
		float* TARGET_Y = nullptr;
		float* TARGET_Z = nullptr;
		float* CAMERA_X = nullptr;
		float* CAMERA_Y = nullptr;
		float* CAMERA_Z = nullptr;
	}Variables;
	struct TmpDataT
	{
		POINT oldPoint = { 0,0 }; 
		POINT oldClickPoint = { 0,0 }; 
		long clickTime = 0;
		long oldClickTime = 0;

		float pitch = 0;
		float yaw = 0;

		float camera_x = 0;
		float camera_y = 0;
		float camera_z = 0;

		float target_x = 0;
		float target_y = 0;
		float target_z = 0;

		bool lButtonHeld = false;

		long scrollPos = 0;
		long oldScrollPos = 0;
		float x_vel = 0;
		float y_vel = 0;
		float z_vel = 0;

		float pitch_vel = 0;
		float yaw_vel = 0;
	}TmpData;
	void InitVariables(int gameVersion);
	void InitKeys();
	unsigned int GetVK_Key(const std::string& key);
	std::unordered_map<std::string, unsigned int> KeyFinder;
	bool IsPressed(unsigned int keycode);
};

