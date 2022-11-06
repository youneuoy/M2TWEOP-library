///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
#include "plugData.h"


#include "eopSoundHelpers.h"

void luaP::initEopSounds()
{
	struct
	{
		sol::table M2TWEOPSoundsTable;
	}tables;


	///M2TWEOPSounds
	//@section M2TWEOPSounds

	/***
	Basic M2TWEOPSounds table. Contains descriptions of m2tweop sounds feathures.
	Attention! You can use 256 sounds max.
	Support the audio file formats WAV, OGG/Vorbis and FLAC.
	You can play many sounds at the same time.
	It should be used for small sounds that can fit in memory and should suffer no lag when they are played(i.e.not music).

	@tfield createEOPSound createEOPSound
	@tfield deleteEOPSound deleteEOPSound
	@tfield playEOPSound playEOPSound
	@tfield setEOPSoundOffset setEOPSoundOffset
	@tfield getEOPSoundOffset getEOPSoundOffset
	@tfield pauseEOPSound pauseEOPSound
	@tfield stopEOPSound stopEOPSound

	@table M2TWEOPSounds
	*/

	tables.M2TWEOPSoundsTable = luaState.create_table("M2TWEOPSounds");

	/***
	Create(and load) new sound
	@function M2TWEOPSounds.createEOPSound
	@tparam string soundPath path to sound file
	@treturn int soundID id what be used for this model. Return nil if can`t load.

	@usage
	local ourNewSound=M2TWEOPSounds.createEOPSound(M2TWEOP.getModPath().."/eopData/testsound.wav");
	*/
	tables.M2TWEOPSoundsTable.set_function("createEOPSound", &eopSoundHelpers::createEOPSound);
	/***
	Delete sound
	@function M2TWEOPSounds.deleteEOPSound
	@tparam int soundID id of sound what you want delete

	@usage
	local ourNewSound=M2TWEOPSounds.createEOPSound(M2TWEOP.getModPath().."/eopData/testsound.wav");
	M2TWEOPSounds.deleteEOPSound(ourNewSound);
	*/
	tables.M2TWEOPSoundsTable.set_function("deleteEOPSound", &eopSoundHelpers::deleteEOPSound);
	/***
	Play sound
	@function M2TWEOPSounds.playEOPSound
	@tparam int soundID id of sound

	@usage
	local ourNewSound=M2TWEOPSounds.createEOPSound(M2TWEOP.getModPath().."/eopData/testsound.wav");
	M2TWEOPSounds.playEOPSound(ourNewSound);
	*/
	tables.M2TWEOPSoundsTable.set_function("playEOPSound", &eopSoundHelpers::playEOPSound);
	/***
	Set sound playing offset
	@function M2TWEOPSounds.setEOPSoundOffset
	@tparam int soundID id of sound
	@tparam int millisecondsOffset new playing position, from the beginning of the sound

	@usage
	local ourNewSound=M2TWEOPSounds.createEOPSound(M2TWEOP.getModPath().."/eopData/testsound.wav");
	M2TWEOPSounds.setEOPSoundOffset(ourNewSound,1000);
	M2TWEOPSounds.playSound(ourNewSound);
	*/
	tables.M2TWEOPSoundsTable.set_function("setEOPSoundOffset", &eopSoundHelpers::setEOPSoundOffset);
	/***
	Get sound playing offset
	@function M2TWEOPSounds.getEOPSoundOffset
	@tparam int soundID id of sound
	@treturn int millisecondsOffset new playing position, from the beginning of the sound

	@usage
	local ourNewSound=M2TWEOPSounds.createEOPSound(M2TWEOP.getModPath().."/eopData/testsound.wav");
	M2TWEOPSounds.playSound(ourNewSound);
	local currentPlayPosition=M2TWEOPSounds.getEOPSoundOffset(ourNewSound);
	*/
	tables.M2TWEOPSoundsTable.set_function("getEOPSoundOffset", &eopSoundHelpers::getEOPSoundOffset);
	/***
	Pause sound
	@function M2TWEOPSounds.pauseEOPSound
	@tparam int soundID id of sound

	@usage
	local ourNewSound=M2TWEOPSounds.createEOPSound(M2TWEOP.getModPath().."/eopData/testsound.wav");
	M2TWEOPSounds.playSound(ourNewSound);
	M2TWEOPSounds.pauseEOPSound(ourNewSound);
	*/
	tables.M2TWEOPSoundsTable.set_function("pauseEOPSound", &eopSoundHelpers::pauseEOPSound);
	/***
	Stop sound playing
	@function M2TWEOPSounds.stopEOPSound
	@tparam int soundID id of sound

	@usage
	local ourNewSound=M2TWEOPSounds.createEOPSound(M2TWEOP.getModPath().."/eopData/testsound.wav");
	M2TWEOPSounds.playSound(ourNewSound);
	M2TWEOPSounds.stopEOPSound(ourNewSound);
	*/
	tables.M2TWEOPSoundsTable.set_function("stopEOPSound", &eopSoundHelpers::stopEOPSound);
}