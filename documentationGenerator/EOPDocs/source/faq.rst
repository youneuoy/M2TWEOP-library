.. M2TWEOP faq

M2TWEOP FAQ
=================================
This is a list of Frequently Asked Questions about M2TWEOP. Feel free to
suggest new entries in our discord!

--------------------------------------------

.. glossary::
	*Where can I download M2TWEOP?*
		The latest version of M2TWEOP is located on the "Releases" tab of the M2TWEOP github, here: https://github.com/youneuoy/M2TWEOP-library/releases. You need to download the archive with the word "M2TWEOP" in the title and the extension '.zip' (e.g M2TWEOP-beta-2.14.zip)

	*How do I install M2TWEOP?*
		Just unpack the downloaded archive into the folder with your modification.

	*I installed M2TWEOP. How can I run a modification using it?*
		Run M2TWEOPGUI.exe, specify your mods config file (e.g TATW.cfg) and hit 'Run Mod'.

	*I heard that M2TWEOP allows you to write plugins in the lua programming language. How to do it?*
		Read https://youneuoy.github.io/M2TWEOP-library/_static/LuaLib/index.html or watch a tutorial here: https://youtu.be/hUvbcz8i7Wk

	*How can I prevent users from running my mod without having EOP enabled?*
		You should run the M2TWEOP launcher, check the "Block modification launch without EOP" parameter in the settings ("game" submenu). Next, you should replace all "type" fields in your "export_descr_unit.txt" file with "eopOnlyType". After that, your mod will not work unless the user launches it with EOP.

	*How can I use M2TWEOP and ReShade together?*
		1. Delete d3d9.dll in Medieval 2 folder if you have one there already
		2. Install ReShade like normal
		3. Rename d3d9.dll in Medieval 2 folder added by ReShade to dxgi.dll
		4. Run mod via M2TWEOP like normal

	*How can I open the M2TWEOP Console, Reload the Script or use the Map Viewer?*
		* CTRL+1 = Open Console
		* CTRL+9+1 = Reloads `luaPluginScript.lua`
		* CTRL+2 = Open EOP Map Viewer