//sfml(for audio)
#if _DEBUG
#pragma comment(lib,"sfml-system-s-d.lib")
#pragma comment(lib,"sfml-audio-s-d.lib")
#else
#pragma comment(lib,"sfml-system-s.lib")
#pragma comment(lib,"sfml-audio-s.lib")

#endif
#pragma comment(lib,"vorbisfile.lib")
#pragma comment(lib,"vorbisenc.lib")
#pragma comment(lib,"vorbis.lib")
#pragma comment(lib,"openal32.lib")
#pragma comment(lib,"ogg.lib")
#pragma comment(lib,"flac.lib")
#pragma comment(lib,"discord_game_sdk.dll.lib")

#include "headersSTD.h"

#include "dataG.h"
#include "resource.h"

#include "helpers.h"
#include "toolRoutine.h"

#include "managerG.h"

#include <stb_image.h>

#include "discordManager.h"

struct {
    SDL_Window* window;
    SDL_GLContext gl_context;
}mainData;

void beginRender()
{

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame(mainData.window);
    ImGui::NewFrame();
}

void endRender()
{
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    SDL_GL_SwapWindow(mainData.window);
}

void initRender()
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        MessageBoxA(NULL, SDL_GetError(), "ERROR", NULL);
        exit(0);
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    mainData.window = SDL_CreateWindow("M2TWEOP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1, 1, window_flags);
    mainData.gl_context = SDL_GL_CreateContext(mainData.window);
    SDL_GL_MakeCurrent(mainData.window, mainData.gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_HideWindow(mainData.window);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleGrey();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(mainData.window, mainData.gl_context);
    ImGui_ImplOpenGL2_Init();


}
typedef int __stdcall appTickFunc(bool*isOpen);
void runApp(appTickFunc drawTick)
{
    bool done = false;
    bool isGUIOpen = true;
    // Main loop
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(mainData.window))
                done = true;
        }
        beginRender();

        drawTick(&isGUIOpen);

        endRender();

        if (isGUIOpen == false || done == true)
        {
            break;
        }
    }
}
#include <filesystem>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    initRender();

    managerG::init();

    if (dataG::data.gameData.isDiscordRichPresenceEnabled == true){
        discordManager::initDiscordRichPresence();
    }
    bool isOpen = true;
    int isExit = 0;

    bool done = false;


    bool isRedistsInstallNeeded = managerG::isRedistsInstallNeeded();
    if (isRedistsInstallNeeded == true)
    {
        runApp(toolRoutine::drawRedistTick);
    }

    if (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(mainData.window))
                done = true;
        }

        beginRender();

        toolRoutine::tryJustStartMod();

        endRender();
    }


    dataG::data.audio.bkgMusic.music = new sf::Music();
    if (dataG::data.modData.hideLauncherAtStart == false)
    {
        dataG::data.audio.bkgMusic.musicLoaded=dataG::data.audio.bkgMusic.music->openFromFile("eopData/music/bkg.flac");

            if (dataG::data.audio.bkgMusic.musicLoaded == true)
            {
                dataG::data.audio.bkgMusic.music->setLoop(true);
                dataG::data.audio.bkgMusic.music->setVolume(dataG::data.audio.bkgMusic.musicVolume);
                if (dataG::data.audio.bkgMusic.isMusicNeeded == true)
                {
                    dataG::data.audio.bkgMusic.music->play();
                }
            }
    }
    // Main loop
    runApp(toolRoutine::drawTick);

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(mainData.gl_context);
    SDL_DestroyWindow(mainData.window);
    SDL_Quit();

    return 0;
}
