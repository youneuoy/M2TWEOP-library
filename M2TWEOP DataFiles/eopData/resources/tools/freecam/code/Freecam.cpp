#include <iostream>
#include <windows.h>
#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <list>
#include <cmath>
#include "AssemblyLine.h"
#include "codeKiller.h"
#include "Options.h"
#include "MouseDelta.h"


#define PI 3.14159265359

bool isBattle;
bool codeRemoved = false;
bool relieve_control = true; //This accounts for basically all camera pan related stuff ingame

float camera_x = 0, camera_y = 0, camera_z = 0, camera_ground_z = 0;
float target_x = 1, target_y = 0, target_z = 0;

float pitch = 0, yaw = 0;

CodeKiller cameraCode;
CodeKiller targetCode;

bool isPressed(unsigned int keycode)
{
    return GetKeyState(keycode) < 0;
}

// These are the addresses that are only read from, this one should be run all the time
void readPassives()
{
    isBattle = RW::readMemoryInt(Options::BATTLE);
}

// These are the addresses that are activly written too
void readActives()
{
    camera_x = RW::readMemoryFloat(Options::CAMERA_X);
    camera_y = RW::readMemoryFloat(Options::CAMERA_Y);
    camera_z = RW::readMemoryFloat(Options::CAMERA_Z);

    target_x = RW::readMemoryFloat(Options::TARGET_X);
    target_y = RW::readMemoryFloat(Options::TARGET_Y);
    target_z = RW::readMemoryFloat(Options::TARGET_Z);

    float length = sqrt(pow(target_x - camera_x, 2) + pow(target_y - camera_y, 2) + pow(target_z - camera_z, 2));
    pitch = asin((target_z - camera_z) / length);
    yaw = atan2((target_y - camera_y) / length, (target_x - camera_x) / length);
    if (isnan(pitch))
        pitch = 0;
    if (isnan(yaw))
        yaw = 0;

}

//Writes to the active addresses, should never be run at the same time as readActives!
void writeActives()
{

    //std::cout << yaw << "  " << pitch << "\n";
    if (Options::OVERRIDE_MOVEMENT)
    {
        RW::writeMemory(Options::CAMERA_X, camera_x);
        RW::writeMemory(Options::CAMERA_Y, camera_y);
        RW::writeMemory(Options::CAMERA_Z, camera_z);
    }
    else
    {
        camera_x = RW::readMemoryFloat(Options::CAMERA_X);
        camera_y = RW::readMemoryFloat(Options::CAMERA_Y);
        camera_z = RW::readMemoryFloat(Options::CAMERA_Z);
    }

    pitch = max(pitch, -(PI / 2.0f) * 0.9f);
    pitch = min(pitch, (PI / 2.0f) * 0.9f);
    target_x = (cos(yaw) * cos(pitch) * 1000.0f) + camera_x;
    target_y = (sin(yaw) * cos(pitch) * 1000.0f) + camera_y;
    target_z = (sin(pitch) * 1000.0f) + camera_z;

    RW::writeMemory(Options::TARGET_X, target_x);
    RW::writeMemory(Options::TARGET_Y, target_y);
    RW::writeMemory(Options::TARGET_Z, target_z);
}

void killCode()
{
    if (!codeRemoved)
    {
        RW::suspend();
        Sleep(Options::RELIEVE_DELAY);
        if (Options::OVERRIDE_MOVEMENT)
            cameraCode.kill();
        targetCode.kill();
        Sleep(Options::RELIEVE_DELAY);
        RW::resume();
        codeRemoved = true;
    }
}

void resurrectCode()
{
    if (codeRemoved)
    {
        RW::suspend();
        Sleep(Options::RELIEVE_DELAY);
        if (Options::OVERRIDE_MOVEMENT)
            cameraCode.resurrect();
        targetCode.resurrect();
        Sleep(Options::RELIEVE_DELAY);
        RW::resume();
        codeRemoved = false;
    }
}

void relieve(bool set)
{
    relieve_control = set;
    if (!set)
        killCode();
    else
        resurrectCode();
}

long scrollPos = 0;
void scroll_event()
{
    while (true)
        scrollPos += GetScrollDelta();
}


int main()
{
    std::cout << "Starting FREECAM!\n";
    std::cout << "Scroll event listener established!\n";
    thread se{ scroll_event };

    Options::init("config.txt");
    RW::init(L"medieval 2");
    Sleep(100);

    cameraCode = CodeKiller(SS::readFile("assemblyLines_Camera.txt"));
    std::cout << "successfully read assemblyLines_Camera.txt!\n";
    targetCode = CodeKiller(SS::readFile("assemblyLines_Target.txt"));
    std::cout << "successfully read assemblyLines_Target.txt!\n";



    readActives();

    POINT p = { 0,0 };
    GetCursorPos(&p);
    int oldX = p.x, oldY = p.y;
    int oldClickX = -50000, oldClickY = -50000;
    int oldScrollPos = 0;
    long clickTime = 0, oldClickTime;

    bool lButtonHeld = false;
    bool paused = false, pauseHeld;
    bool fast_press, slow_press;


    float x_vel = 0;
    float y_vel = 0;
    float z_vel = 0;

    float pitch_vel = 0;
    float yaw_vel = 0;

    unsigned int sleepTime = (1000 / Options::UPS) / 2;

    HWND hwnd = FindWindow(0, L"medieval 2");
    std::cout << "Main loop started!\nFreecam 1.2.1 is now active!\n";

    while (true)
    {
        RECT window;
        GetWindowRect(hwnd, &window);
        Sleep(sleepTime);
        readPassives();

        //Exit
        if (isPressed(Options::EXIT))
        {
            resurrectCode();
            RW::terminate("USER EXIT");
        }

        //PAUSE CLICKING
        if (isPressed(Options::PAUSE))
        {
            if (pauseHeld == false)
                paused = !paused, std::cout << (paused ? "paused" : "unpaused") << "\n";
            pauseHeld = true;
        }
        else
            pauseHeld = false;

        //Main camera code here. Only runs if you're tabbed in and when there is a battle
        if (isBattle && !paused && (window.left > -32000 && window.top >= 0))
        {
            GetCursorPos(&p);

            //Variables
            fast_press = isPressed(Options::FAST);
            slow_press = isPressed(Options::SLOW);

            float horizontal_speed = Options::HORIZONTAL_SPEED;
            float vertical_speed = Options::VERTICAL_SPEED;
            if (fast_press && slow_press)
                horizontal_speed *= Options::BOTH_MOD, vertical_speed *= Options::BOTH_MOD;
            else if (fast_press)
                horizontal_speed *= Options::FAST_MOD, vertical_speed *= Options::FAST_MOD;
            else if (slow_press)
                horizontal_speed *= Options::SLOW_MOD, vertical_speed *= Options::SLOW_MOD;

            float x_acc = 0;
            float y_acc = 0;
            float z_acc = 0;

            float pitch_acc = 0;
            float yaw_acc = 0;


            //################# VANILLA FUNCTION RETENTION ####################
            //Double click detection
            if (isPressed(VK_LBUTTON))
            {
                if (lButtonHeld == false)
                {
                    oldClickTime = clickTime;
                    clickTime = chrono::duration_cast<chrono::milliseconds>(chrono::time_point_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()).time_since_epoch()).count();
                    if (clickTime - oldClickTime < GetDoubleClickTime() && abs(oldClickX - p.x) < 2 && abs(oldClickY - p.y) < 2)
                        relieve(true);
                    oldClickX = p.x, oldClickY = p.y;
                }
                lButtonHeld = true;
            }
            else
                lButtonHeld = false;


            //################# CAMERA POSITION ####################
            if (isPressed(Options::FORWARD))
                y_acc += sin(0 + yaw), x_acc += cos(0 + yaw),
                relieve(false);

            if (isPressed(Options::BACKWARD))
                y_acc += sin(PI + yaw), x_acc += cos(PI + yaw),
                relieve(false);

            if (isPressed(Options::LEFT))
                y_acc += sin(PI / 2 + yaw), x_acc += cos(PI / 2 + yaw),
                relieve(false);

            if (isPressed(Options::RIGHT))
                y_acc += sin(3 * (PI / 2) + yaw), x_acc += cos(3 * (PI / 2) + yaw),
                relieve(false);

            //Up and down
            float diff = (scrollPos - oldScrollPos);
            diff = Options::INVERT_SCROLL ? -diff : diff;
            int isNegative = (diff != 0 ? (abs(diff) / diff) : 1);
            z_vel += (pow(diff, 2) * isNegative) * vertical_speed / 10.0f;  //Kinda ugly

            if (isPressed(Options::UP))
                z_acc -= 1,
                relieve(false);

            if (isPressed(Options::DOWN))
                z_acc -= -1,
                relieve(false);


            //################# CAMERA ORIENTATION ####################
            //FREEEECAAAAAAAAM
            float adjusted_sensitivty = (Options::SENSITIVITY * (1 - Options::CAMERA_SMOOTHENING));
            if (isPressed(Options::FREECAM))
            {
                float invert = Options::INVERT_MOUSE ? -1.0 : 1.0;
                pitch_acc -= (float)(invert * (p.y - oldY)) / (500.0f) * adjusted_sensitivty;
                yaw_acc -= (float)(invert * (p.x - oldX)) / (500.0f) * adjusted_sensitivty;
                relieve(false);
            }

            //Edge Detection
            float adjusted_panSpeed = (Options::PAN_PEED * (1 - Options::CAMERA_SMOOTHENING));
            if (Options::ENABLE_EDGEPAN)
            {
                if (p.x <= window.left + Options::EDGEPAN_MARGIN)
                    yaw_acc += (3.0f / 100.0f) * adjusted_panSpeed,
                    relieve(false);

                else if (p.x >= window.right - Options::EDGEPAN_MARGIN)
                    yaw_acc -= (3.0f / 100.0f) * adjusted_panSpeed,
                    relieve(false);

                if (p.y <= window.top + Options::EDGEPAN_MARGIN)
                    y_acc += sin(0 + yaw), x_acc += cos(0 + yaw),
                    relieve(false);

                else if (p.y >= window.bottom - Options::EDGEPAN_MARGIN)
                    y_acc += sin(PI + yaw), x_acc += cos(PI + yaw),
                    relieve(false);
            }

            //Keycontrolled orientation
            if (Options::ROTATION_CONTROLS)
            {
                if (isPressed(Options::ROTATE_UP))
                    pitch_acc += (3.0f / 100.0f) * adjusted_panSpeed,
                    relieve(false);

                if (isPressed(Options::ROTATE_DOWN))
                    pitch_acc -= (3.0f / 100.0f) * adjusted_panSpeed,
                    relieve(false);

                if (isPressed(Options::ROTATE_LEFT))
                    yaw_acc += (3.0f / 100.0f) * adjusted_panSpeed,
                    relieve(false);

                if (isPressed(Options::ROTATE_RIGHT))
                    yaw_acc -= (3.0f / 100.0f) * adjusted_panSpeed,
                    relieve(false);
            }

            //For unit zooming
            if (isPressed(Options::getVK_Key("VK_MBUTTON")))
                relieve(true);

            //################# ETC ####################
            float length = sqrt(pow(x_acc, 2) + pow(y_acc, 2) + pow(z_acc, 2));
            if (length == 0) // 0/0 avoidance
                length = 1;
            x_vel += ((x_acc / length) * (horizontal_speed * (1 - Options::HORIZONTAL_SMOOTHENING))) / 2.0f;  //Weird math is to account for smoothening
            y_vel += ((y_acc / length) * (horizontal_speed * (1 - Options::HORIZONTAL_SMOOTHENING))) / 2.0f;
            z_vel += ((z_acc / length) * (vertical_speed * (1 - Options::VERTICAL_SMOOTHENING))) / 2.0f;
            camera_x += x_vel;
            camera_y += y_vel;
            camera_z += z_vel;
            x_vel *= Options::HORIZONTAL_SMOOTHENING;
            y_vel *= Options::HORIZONTAL_SMOOTHENING;
            z_vel *= Options::VERTICAL_SMOOTHENING;

            pitch_vel += pitch_acc;
            yaw_vel += yaw_acc;
            pitch += pitch_vel;
            yaw += yaw_vel;
            pitch_vel *= Options::CAMERA_SMOOTHENING;
            yaw_vel *= Options::CAMERA_SMOOTHENING;

            oldX = p.x, oldY = p.y;

            camera_x = min(max(camera_x, -900), 900);
            camera_y = min(max(camera_y, -900), 900);

            //Writer / Reader
            if (!relieve_control)
                writeActives();
            else
                readActives();
        }
        else // When youre not tabbed in or in a battle
        {
            relieve(true);
            readActives();
        }

        oldScrollPos = scrollPos;
    }
    std::cout << " done!";
}
