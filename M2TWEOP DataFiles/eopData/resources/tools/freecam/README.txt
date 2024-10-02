
FREECAM -Bugis_Duckis
	This version of FREECAM most likely not perfect and weirdness may happen! I have included the source code for anyone who wants it. It's probably
	not very good, but I'll add that I'm not very accustomed to C++. Use it as you wish, no need to credit!


What does this program actually do?
	Freecam is for all intents and purposes a hack. It essentially hijacks game variables and writes its own values to them. With version 1.0 it 
	even edits the machine code of medieval 2 in real-time to stop jitteryness and make the program less CPU demanding. Due to the hacky nature
	of the program, IT IS IMPORTANT THAT YOU ONLY RUN THIS FOR THE CORRECT VERSION OF THE GAME, or else it will crash. Have fun!


REQUIREMENTS:
	Windows 10/11 64bit, other versions of windows have not been tested.
	Medieval 2 total war (Definitive edition V1.5.2.0/1.52), as for now gold edition has not been tested.
	Microsoft Visual c++ 2022.


How to use?
	step 1: launch the game.
	step 2: Make sure you use "total war" camera style ingame.
	step 3:	Go into conf.txt and make sure that the controls match those ingame.
	step 4: tab out of the game and start Freecam.exe.
	step 5: tab into the game and enjoy!

	Recommendations:
		Turn off restrict camera.
		Unbind alt-s in the ingame settings (It mutes sound). You can do this by binding alt-s to a useless bind such as getting rid of advisor.

	Notes:
		If you want to turn off freecam, then it is very important that you do so through the exit bind (END by default) and not the X button 
		in the corner. Else camera will be frozen in place until you restart the game!
		You do not need to place the executable in any specific place or mod folder, it can be run from anywhere!
		If you momentarily want vanilla controls, freecam can be paused with the the pause key (INSERT by default)
		Save often, crashing though rare may occur.


Questions:
	Where should i place the program? Anywhere you want! It doesn't not be in any place or modfolder, just make sure that it's next to the files it comes with!
	Does this work with the DLC:s? Yes!
	Does this work with mods? Yes!
	Does this work on multiplayer? Yes!
	Does this work with any other total war games? No!
	Can I use the code to develop my own projects/fix this one? Absolutely!


Known Issues:
	Camera going below game when double clicking on units, causing the game to freeze.
	There is a small chance of crashing currently (As for now it has only occurred when tabbing).


Troubleshooting:
	General Issues.
		Try starting freecam as administrator.


-----------------------------------------------------------------------------------------------------------
Changelog:
	1.2.2:
		Additions:
			Added an option for controlling the delay when the program gives or releases control of the camera.
	1.2.1:
		Fixes:
			Fixed an error with the config file. Where the program would mix up the binds for rotate_up and up.

		Changes:
			Changed the name of the FORWARD, BACKWARD, etc... to MOVE_FORWARD, MOVE_BACKWARD, etc...

	1.2:
		Fixes:
			VERTICAL_SPEED now functions properly.
			Fixed a typo in the code that stopped ROTATION_CONTROLS from working.

		Additions:
			Added UPS, allows you to set how many updates per second the program will be running at. for users with 60fps+ monitors. You will need to 
				tweak the variables in config.txt. As UPS simply changes how fast the program will run, camera will therefor appear to be twice as fast if you set it to 120 rather than the default 60 as an example.
			Added an Icon for the executable.

	1.1:
		Fixes:
			Fixed inconsitency in speed when pressing forward and touching the top of the screen.
			Fixed EDGEPAN_SPEED so it has similar speeds as camera sensitivity.

		Changes:
			Higher sensitivty values now actually makes the camera more sensitive rather than less.

		Additions:
			Added OVERRIDE_MOVEMENT, which makes the movement and scrolling controls provided by freecam optional.
			Added untested Gold Edition addresses. I have no way to test these as I dont own gold edition, only use 
				on custom battles until it has been confirmed that they infact do work. You might otherwise risk 
				losing progress on your save due to crashes.
			Added keybinds for rotating camera.


	1.0:
		Due to this version being completely seperate, earlier versions will not be included in this changelog as to avoid missinformation.
		Due to reports of instability from many users, I have decided to rewrite the entire project. While the general idea remains the same,
		some new features have been added to greatly improve stability and decrease CPU usage. The biggest of which is the fact that the 
		program now will delete lines of machine-code straight from the games process memory while it's running, which stops the game
		from writing data to the camera position variables. In earlier versions, the program was essentially having a battle to death over 
		control of the variables, and for some systems the game was winning that battle. In this version the games hands got chopped off.
		This only happens in RAM memory so you don't need to worry about game files, however the game might crash if you're running an
		incompatible version of the game.

		Hopefully this version will run better! Sadly I cant make this work for gold edition as I dont own that version, if someone manages
		to translate all of the addresses to gold edition then I'll be greatful! Otherwise I'll try to come with some solution in the future.

		Additions:
		- Greatly improved stability.
		- Decreased CPU usage.
		- Smoother camera movements.
		- More user friendly config file.
		- Custom memory addresses.
		- Pausing.

		Not included:
		- Control of camera orientation with keyboard (Kinda defeats the point of having freecam).
		- Gold Edition support, I will try do something for this in the future.
		- Some configuration regarding camera speed. This can be readded if people want it.