#pragma once

namespace console
{
	void draw();
	void toggleConsole();
	void handleMessageBoxResult(int result);

	struct
	{
		std::string input;
		
		bool isDraw = false;
		int pressAmount = 0;
		int commandNum = 0;
		bool controlsDisabled = true;
		bool clearInput = true;
	}consoleData;
};

