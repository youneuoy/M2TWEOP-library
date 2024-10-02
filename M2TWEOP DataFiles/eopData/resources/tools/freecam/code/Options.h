#pragma once
#include <iostream>
#include "ss.h"
#include <regex>
#include <cmath>
#include <unordered_map>

namespace Options
{
	std::unordered_map<std::string, unsigned int> keyFinder;
	unsigned int UPS = 60;
	unsigned int RELIEVE_DELAY = 2;

	bool OVERRIDE_MOVEMENT;
	bool ROTATION_CONTROLS;

	unsigned int EXIT, PAUSE;

	unsigned int FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN;
	unsigned int ROTATE_UP, ROTATE_DOWN, ROTATE_LEFT, ROTATE_RIGHT;
	unsigned int FAST, SLOW;
	unsigned int FREECAM;

	float SENSITIVITY;
	float HORIZONTAL_SMOOTHENING;
	float VERTICAL_SMOOTHENING;
	float CAMERA_SMOOTHENING;
	float HORIZONTAL_SPEED;
	float VERTICAL_SPEED;
	float PAN_PEED;

	float FAST_MOD, SLOW_MOD, BOTH_MOD;

	bool INVERT_MOUSE;
	bool INVERT_SCROLL;

	bool ENABLE_EDGEPAN;
	unsigned int EDGEPAN_MARGIN;

	LPVOID BATTLE;
	LPVOID CAMERA_X, CAMERA_Y, CAMERA_Z;
	LPVOID TARGET_X, TARGET_Y, TARGET_Z;

	std::string getVar(string text, string varName)
	{
		std::string out = std::regex_replace(text, std::regex("\n+"), " ");
		out = std::regex_replace(out, std::regex(".*(\t| |^)" + varName + "="), "");
		out = std::regex_replace(out, std::regex(" .*"), "");
		//std::cout << "\t" << varName << " = " << out << "\n";
		return out;
	}

	unsigned int getVK_Key(std::string key)
	{
		if (key.length() == 1)
			return VkKeyScanExA(tolower(key[0]), GetKeyboardLayout(0));
		else if (key.length() > 1)
			return keyFinder.at(key);
		else
		{
			std::cerr << "key " << key << " does not exist!\n" ;
			throw std::runtime_error("Key is empty");
		}
	}

	void init(std::string file_path)
	{
		{
			keyFinder.emplace("VK_LBUTTON", 0x01);
			keyFinder.emplace("VK_RBUTTON", 0x02);
			keyFinder.emplace("VK_CANCEL", 0x03);
			keyFinder.emplace("VK_MBUTTON", 0x04);
			keyFinder.emplace("VK_XBUTTON1", 0x05);
			keyFinder.emplace("VK_XBUTTON2", 0x06);
			keyFinder.emplace("VK_BACK", 0x08);
			keyFinder.emplace("VK_TAB", 0x09);
			keyFinder.emplace("VK_CLEAR", 0x0C);
			keyFinder.emplace("VK_RETURN", 0x0D);
			keyFinder.emplace("VK_SHIFT", 0x10);
			keyFinder.emplace("VK_CONTROL", 0x11);
			keyFinder.emplace("VK_MENU", 0x12);
			keyFinder.emplace("VK_PAUSE", 0x13);
			keyFinder.emplace("VK_CAPITAL", 0x14);
			keyFinder.emplace("VK_KANA", 0x15);
			keyFinder.emplace("VK_HANGUEL", 0x15);
			keyFinder.emplace("VK_HANGUL", 0x15);
			keyFinder.emplace("VK_IME_ON", 0x16);
			keyFinder.emplace("VK_JUNJA", 0x17);
			keyFinder.emplace("VK_FINAL", 0x18);
			keyFinder.emplace("VK_HANJA", 0x19);
			keyFinder.emplace("VK_KANJI", 0x19);
			keyFinder.emplace("VK_IME_OFF", 0x1A);
			keyFinder.emplace("VK_ESCAPE", 0x1B);
			keyFinder.emplace("VK_CONVERT", 0x1C);
			keyFinder.emplace("VK_NONCONVERT", 0x1D);
			keyFinder.emplace("VK_ACCEPT", 0x1E);
			keyFinder.emplace("VK_MODECHANGE", 0x1F);
			keyFinder.emplace("VK_SPACE", 0x20);
			keyFinder.emplace("VK_PRIOR", 0x21);
			keyFinder.emplace("VK_NEXT", 0x22);
			keyFinder.emplace("VK_END", 0x23);
			keyFinder.emplace("VK_HOME", 0x24);
			keyFinder.emplace("VK_LEFT", 0x25);
			keyFinder.emplace("VK_UP", 0x26);
			keyFinder.emplace("VK_RIGHT", 0x27);
			keyFinder.emplace("VK_DOWN", 0x28);
			keyFinder.emplace("VK_SELECT", 0x29);
			keyFinder.emplace("VK_PRINT", 0x2A);
			keyFinder.emplace("VK_EXECUTE", 0x2B);
			keyFinder.emplace("VK_SNAPSHOT", 0x2C);
			keyFinder.emplace("VK_INSERT", 0x2D);
			keyFinder.emplace("VK_DELETE", 0x2E);
			keyFinder.emplace("VK_HELP", 0x2F);
			keyFinder.emplace("VK_LWIN", 0x5B);
			keyFinder.emplace("VK_RWIN", 0x5C);
			keyFinder.emplace("VK_APPS", 0x5D);
			keyFinder.emplace("VK_SLEEP", 0x5F);
			keyFinder.emplace("VK_NUMPAD0", 0x60);
			keyFinder.emplace("VK_NUMPAD1", 0x61);
			keyFinder.emplace("VK_NUMPAD2", 0x62);
			keyFinder.emplace("VK_NUMPAD3", 0x63);
			keyFinder.emplace("VK_NUMPAD4", 0x64);
			keyFinder.emplace("VK_NUMPAD5", 0x65);
			keyFinder.emplace("VK_NUMPAD6", 0x66);
			keyFinder.emplace("VK_NUMPAD7", 0x67);
			keyFinder.emplace("VK_NUMPAD8", 0x68);
			keyFinder.emplace("VK_NUMPAD9", 0x69);
			keyFinder.emplace("VK_MULTIPLY", 0x6A);
			keyFinder.emplace("VK_ADD", 0x6B);
			keyFinder.emplace("VK_SEPARATOR", 0x6C);
			keyFinder.emplace("VK_SUBTRACT", 0x6D);
			keyFinder.emplace("VK_DECIMAL", 0x6E);
			keyFinder.emplace("VK_DIVIDE", 0x6F);
			keyFinder.emplace("VK_F1", 0x70);
			keyFinder.emplace("VK_F2", 0x71);
			keyFinder.emplace("VK_F3", 0x72);
			keyFinder.emplace("VK_F4", 0x73);
			keyFinder.emplace("VK_F5", 0x74);
			keyFinder.emplace("VK_F6", 0x75);
			keyFinder.emplace("VK_F7", 0x76);
			keyFinder.emplace("VK_F8", 0x77);
			keyFinder.emplace("VK_F9", 0x78);
			keyFinder.emplace("VK_F10", 0x79);
			keyFinder.emplace("VK_F11", 0x7A);
			keyFinder.emplace("VK_F12", 0x7B);
			keyFinder.emplace("VK_F13", 0x7C);
			keyFinder.emplace("VK_F14", 0x7D);
			keyFinder.emplace("VK_F15", 0x7E);
			keyFinder.emplace("VK_F16", 0x7F);
			keyFinder.emplace("VK_F17", 0x80);
			keyFinder.emplace("VK_F18", 0x81);
			keyFinder.emplace("VK_F19", 0x82);
			keyFinder.emplace("VK_F20", 0x83);
			keyFinder.emplace("VK_F21", 0x84);
			keyFinder.emplace("VK_F22", 0x85);
			keyFinder.emplace("VK_F23", 0x86);
			keyFinder.emplace("VK_F24", 0x87);
			keyFinder.emplace("VK_NUMLOCK", 0x90);
			keyFinder.emplace("VK_SCROLL", 0x91);
			keyFinder.emplace("VK_LSHIFT", 0xA0);
			keyFinder.emplace("VK_RSHIFT", 0xA1);
			keyFinder.emplace("VK_LCONTROL", 0xA2);
			keyFinder.emplace("VK_RCONTROL", 0xA3);
			keyFinder.emplace("VK_LMENU", 0xA4);
			keyFinder.emplace("VK_RMENU", 0xA5);
			keyFinder.emplace("VK_BROWSER_BACK", 0xA6);
			keyFinder.emplace("VK_BROWSER_FORWARD", 0xA7);
			keyFinder.emplace("VK_BROWSER_REFRESH", 0xA8);
			keyFinder.emplace("VK_BROWSER_STOP", 0xA9);
			keyFinder.emplace("VK_BROWSER_SEARCH", 0xAA);
			keyFinder.emplace("VK_BROWSER_FAVORITES", 0xAB);
			keyFinder.emplace("VK_BROWSER_HOME", 0xAC);
			keyFinder.emplace("VK_VOLUME_MUTE", 0xAD);
			keyFinder.emplace("VK_VOLUME_DOWN", 0xAE);
			keyFinder.emplace("VK_VOLUME_UP", 0xAF);
			keyFinder.emplace("VK_MEDIA_NEXT_TRACK", 0xB0);
			keyFinder.emplace("VK_MEDIA_PREV_TRACK", 0xB1);
			keyFinder.emplace("VK_MEDIA_STOP", 0xB2);
			keyFinder.emplace("VK_MEDIA_PLAY_PAUSE", 0xB3);
			keyFinder.emplace("VK_LAUNCH_MAIL", 0xB4);
			keyFinder.emplace("VK_LAUNCH_MEDIA_SELECT", 0xB5);
			keyFinder.emplace("VK_LAUNCH_APP1", 0xB6);
			keyFinder.emplace("VK_LAUNCH_APP2", 0xB7);
			keyFinder.emplace("VK_OEM_1", 0xBA);
			keyFinder.emplace("VK_OEM_PLUS", 0xBB);
			keyFinder.emplace("VK_OEM_COMMA", 0xBC);
			keyFinder.emplace("VK_OEM_MINUS", 0xBD);
			keyFinder.emplace("VK_OEM_PERIOD", 0xBE);
			keyFinder.emplace("VK_OEM_2", 0xBF);
			keyFinder.emplace("VK_OEM_3", 0xC0);
			keyFinder.emplace("VK_OEM_4", 0xDB);
			keyFinder.emplace("VK_OEM_5", 0xDC);
			keyFinder.emplace("VK_OEM_6", 0xDD);
			keyFinder.emplace("VK_OEM_7", 0xDE);
			keyFinder.emplace("VK_OEM_8", 0xDF);
			keyFinder.emplace("VK_OEM_102", 0xE2);
			keyFinder.emplace("VK_PROCESSKEY", 0xE5);
			keyFinder.emplace("VK_PACKET", 0xE7);
			keyFinder.emplace("VK_ATTN", 0xF6);
			keyFinder.emplace("VK_CRSEL", 0xF7);
			keyFinder.emplace("VK_EXSEL", 0xF8);
			keyFinder.emplace("VK_EREOF", 0xF9);
			keyFinder.emplace("VK_PLAY", 0xFA);
			keyFinder.emplace("VK_ZOOM", 0xFB);
			keyFinder.emplace("VK_PA1", 0xFD);
			keyFinder.emplace("VK_OEM_CLEAR", 0xFE);
		}

		try
		{
			std::string contents = SS::readFile(file_path);

			contents = std::regex_replace(contents, std::regex("#.*"), "");
			contents = std::regex_replace(contents, std::regex("\n+"), "\n");
			contents = std::regex_replace(contents, std::regex("^\n+"), "");
			contents = std::regex_replace(contents, std::regex("\n+$"), "");
			contents = std::regex_replace(contents, std::regex(" "), "");
			contents = std::regex_replace(contents, std::regex("\t"), "");

			FORWARD = getVK_Key(getVar(contents, "MOVE_FORWARD"));
			LEFT	= getVK_Key(getVar(contents, "MOVE_LEFT"));
			BACKWARD= getVK_Key(getVar(contents, "MOVE_BACKWARD"));
			RIGHT	= getVK_Key(getVar(contents, "MOVE_RIGHT"));
			UP		= getVK_Key(getVar(contents, "MOVE_UP"));
			DOWN	= getVK_Key(getVar(contents, "MOVE_DOWN"));
			ROTATE_LEFT = getVK_Key(getVar(contents, "ROTATE_LEFT"));
			ROTATE_RIGHT = getVK_Key(getVar(contents, "ROTATE_RIGHT"));
			ROTATE_UP = getVK_Key(getVar(contents, "ROTATE_UP"));
			ROTATE_DOWN = getVK_Key(getVar(contents, "ROTATE_DOWN"));

			EXIT	= getVK_Key(getVar(contents, "EXIT"));
			PAUSE	= getVK_Key(getVar(contents, "PAUSE"));
			FAST	= getVK_Key(getVar(contents, "FAST"));
			SLOW	= getVK_Key(getVar(contents, "SLOW"));
			FREECAM = getVK_Key(getVar(contents, "FREECAM"));

			SENSITIVITY			= stof(getVar(contents, "SENSITIVITY"));
			HORIZONTAL_SMOOTHENING = max(min(SS::percentageToFloat(getVar(contents, "HORIZONTAL_SMOOTHENING")),0.99999999f),0);
			VERTICAL_SMOOTHENING = max(min(SS::percentageToFloat(getVar(contents, "VERTICAL_SMOOTHENING")), 0.99999999f), 0);
			CAMERA_SMOOTHENING  = max(min(SS::percentageToFloat(getVar(contents, "CAMERA_SMOOTHENING")), 0.99999999f), 0);
			HORIZONTAL_SPEED	= stof(getVar(contents, "HORIZONTAL_SPEED"));
			VERTICAL_SPEED		= stof(getVar(contents, "VERTICAL_SPEED"));
			PAN_PEED = stof(getVar(contents, "PAN_PEED"));

			FAST_MOD = SS::percentageToFloat(getVar(contents, "FAST_MOD"));
			SLOW_MOD = SS::percentageToFloat(getVar(contents, "SLOW_MOD"));
			BOTH_MOD = SS::percentageToFloat(getVar(contents, "BOTH_MOD"));

			INVERT_MOUSE	 = getVar(contents, "INVERT_MOUSE") == "true" ? true : false;
			INVERT_SCROLL	 = getVar(contents, "INVERT_SCROLL") == "true" ? true : false;
			ENABLE_EDGEPAN	 = getVar(contents, "ENABLE_EDGEPAN") == "true" ? true : false;
			ROTATION_CONTROLS= getVar(contents, "ROTATION_CONTROLS") == "true" ? true : false;

			EDGEPAN_MARGIN	= stoi(getVar(contents, "EDGEPAN_MARGIN"));


			OVERRIDE_MOVEMENT = getVar(contents, "OVERRIDE_MOVEMENT") == "true" ? true : false;

			//Addresses
			BATTLE = (LPVOID)std::stoul(getVar(contents, "BATTLE"), nullptr, 16);
			CAMERA_X = (LPVOID)std::stoul(getVar(contents, "CAMERA_X"), nullptr, 16);
			CAMERA_Y = (LPVOID)std::stoul(getVar(contents, "CAMERA_Y"), nullptr, 16);
			CAMERA_Z = (LPVOID)std::stoul(getVar(contents, "CAMERA_Z"), nullptr, 16);
			TARGET_X = (LPVOID)std::stoul(getVar(contents, "TARGET_X"), nullptr, 16);
			TARGET_Y = (LPVOID)std::stoul(getVar(contents, "TARGET_Y"), nullptr, 16);
			TARGET_Z = (LPVOID)std::stoul(getVar(contents, "TARGET_Z"), nullptr, 16);

			UPS = std::stoi(getVar(contents, "UPS")); 
			RELIEVE_DELAY = std::stoi(getVar(contents, "RELIEVE_DELAY"));

			std::cout << "Options in " + file_path + " successfully read!\n";
		}
		catch (...)
		{
			std::cerr << "Failed to read options " + file_path + "\n";
			Sleep(6000);
			throw std::runtime_error("Failed to read options from " + file_path);
			exit(0);
		}
	}
}