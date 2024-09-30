///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "pch.h"
#include "eopVideos.h"

#include "eopVideo.h"

#include "graphicsD3D.h"
void eopVideos::addToLua(sol::state& luaState)
{
    struct
    {
        sol::table M2TWEOPVideosTable;
    }tables;

    ///M2TWEOPVideos
    //@section M2TWEOPVideos

    /***
    Basic M2TWEOPVideos table. Contains descriptions of M2TWEOP videos features.
    Fully integrated with M2TWEOP Imgui, based on FFMPEG library. You can play videos directly in your Imgui windows now!
    It supports the following formats:
     - MP4


    @tfield createEOPVideo createEOPVideo
    @tfield deleteEOPVideo deleteEOPVideo
    @tfield playEOPVideo playEOPVideo


    @table M2TWEOPVideos
    */

    tables.M2TWEOPVideosTable = luaState.create_table("M2TWEOPVideos");

    /***
    Create (use lazy load) a new video.
    @function M2TWEOPVideos.createEOPVideo
    @tparam string videoPath Path to video file
    @treturn int videoID ID that will be used by this video. Returns nil if the video can't load..

    @usage
    local ourNewVideo=M2TWEOPVideos.createEOPVideo(M2TWEOP.getModPath().."/eopData/testvideo.mp4");
    */
    tables.M2TWEOPVideosTable.set_function("createEOPVideo", &eopVideos::createEOPVideo);
    /***
    Delete a video
    @function M2TWEOPVideos.deleteEOPVideo
    @tparam int videoID ID of the video that you want to delete.

    @usage
    local ourNewVideo=M2TWEOPVideos.createEOPVideo(M2TWEOP.getModPath().."/eopData/testvideo.mp4");
    M2TWEOPVideos.deleteEOPVideo(ourNewVideo);
    */
    tables.M2TWEOPVideosTable.set_function("deleteEOPVideo", &eopVideos::deleteEOPVideo);
    /***
    Play a video frame.
    @function M2TWEOPVideos.playEOPVideo
    @tparam int videoID ID of the video that you want to play.

    @usage
    local ourNewVideo=M2TWEOPVideos.createEOPVideo(M2TWEOP.getModPath().."/eopData/testvideo.mp4");

    ImGui.Begin("My Window with video", open, ImGuiWindowFlags.NoDecoration)
    M2TWEOPVideos.playEOPVideo(ourNewVideo);
    ImGui.End()
    */
    tables.M2TWEOPVideosTable.set_function("playEOPVideo", &eopVideos::playEOPVideo);
}
void* eopVideos::createEOPVideo(const char* path)
{
    const auto device = graphicsExport::GetDevice();
    eopVideo* eopVid = new eopVideo((char*)path, device);
    if (eopVid->IsOk() == false)
    {
        MessageBoxA(NULL, eopVid->getErrorMessage().c_str(), "Error at createEOPVideo", NULL);

        delete eopVid;
        return nullptr;
    }
    return eopVid;
}

void eopVideos::deleteEOPVideo(void* video)
{
    if (video == nullptr)
    {
        return;
    }
    eopVideo* vid = reinterpret_cast<eopVideo*>(video);

    delete vid;
}

void eopVideos::playEOPVideo(void* video)
{
    if (video == nullptr)
    {
        return;
    }
    const auto device = graphicsExport::GetDevice();
    eopVideo* vid = reinterpret_cast<eopVideo*>(video);
    vid->PlayFrame();
}
