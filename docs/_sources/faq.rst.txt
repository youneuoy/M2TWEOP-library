.. M2TWEOP faq

M2TWEOP FAQ
=================================

This is a list of Frequently Asked Questions about M2TWEOP. Feel free to
suggest new entries in our discord!

Questions and answers
--------------------------------------------


.. glossary::
	*Where can I download M2TWEOP?*
		The latest version of m2tweop is located on the "Releases" tab of the M2TWEOP github, here: https://github.com/youneuoy/M2TWEOP-library/releases
		You need to download the archive with the word "M2TWEOP" in the title and the extension .zip

	*How do I install M2TWEOP?*
		Just unpack the downloaded archive into the folder with your modification.

	*I installed M2TWEOP. How can I run a modification using it?*
		Run M2TWEOPGUI.exe, specify your mods config file (e.g TATW.cfg) and hit 'Run Mod'.

	*I heard that M2TWEOP allows you to write plugins in the lua programming language. How to do it?*
		Read https://youneuoy.github.io/M2TWEOP-library/_static/LuaLib/index.html or watch a tutorial here: https://youtu.be/hUvbcz8i7Wk

	*I made a great modification using M2TWEOP features. How can I make it possible to start the modification only with M2TWEOP (so that it is impossible to start it with standard means - via batch file, etc)?*
		You should run the M2TWEOP launcher, check the "Block modification launch without EOP" parameter in the settings ("game" submenu). Next, you should replace all "type" fields in your "export_descr_unit.txt" file with "eopOnlyType". After that, your mod will not work unless the user launches it with EOP.
