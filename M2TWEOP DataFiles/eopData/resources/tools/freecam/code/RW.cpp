#include "RW.h"

void RW::terminate(std::string reason)
{
    CloseHandle(hProc);
    for (int i = 0; i < hThread_length; i++)
        CloseHandle(hThreads[i]);
    std::cout << reason << "\n TERMINATING IN 6 SECONDS!";
    Sleep(6000);
    exit(0);
}

//READERS
byte RW::readMemoryByte(LPVOID address)
{
    byte var;
    ReadProcessMemory(hProc, address, &var, (DWORD)sizeof(var), NULL);
    return var;
}

float RW::readMemoryFloat(LPVOID address)
{
    float var;
    ReadProcessMemory(hProc, address, &var, (DWORD)sizeof(var), NULL);
    return var;
}

int RW::readMemoryInt(LPVOID address)
{
    int var;
    ReadProcessMemory(hProc, address, &var, (DWORD)sizeof(var), NULL);
    return var;
}

bool RW::readMemoryBool(LPVOID address)
{
    bool var;
    ReadProcessMemory(hProc, address, &var, (DWORD)sizeof(var), NULL);
    return var;
}

//WRITERS
bool RW::writeMemory(LPVOID address, byte data)
{
    return WriteProcessMemory(hProc, address, &data, (DWORD)sizeof(data), NULL);
}
bool RW::writeMemory(LPVOID address, float data)
{
    return WriteProcessMemory(hProc, address, &data, (DWORD)sizeof(data), NULL);
}
bool RW::writeMemory(LPVOID address, int data)
{
    return WriteProcessMemory(hProc, address, &data, (DWORD)sizeof(data), NULL);
}

void RW::init(LPCWSTR proc)
{
    //hwnd = FindWindow(0, L"medieval 2");
    hwnd = FindWindow(0, proc);
    if (hwnd == 0)
        terminate("Cannot find window, is the game currently on?");
    else
    {
        std::cout << proc << " Window found!\n";
        GetWindowThreadProcessId(hwnd, &pId);
        hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);

        if (!hProc)
            terminate("Cannot open process");
        else
            std::cout << "Process found!\n";

        //Find game thread so it can be paused
        HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

        THREADENTRY32 threadEntry;
        threadEntry.dwSize = sizeof(THREADENTRY32);

        std::list<HANDLE> handles{};
        Thread32First(hThreadSnapshot, &threadEntry);
        do
        {
            if (threadEntry.th32OwnerProcessID == pId)
            {
                HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
                    threadEntry.th32ThreadID);
                handles.push_front(hThread);
                hThread_length++;
            }
        } while (Thread32Next(hThreadSnapshot, &threadEntry));
        std::cout << "Process threads found!\n";

        hThreads = new HANDLE[handles.size()];
        for (int i = 0; i < hThread_length; i++)
            hThreads[i] = handles.front(), handles.pop_front();

        std::cout << "RW init complete!\n";

        CloseHandle(hThreadSnapshot);
    }
}

void RW::suspend()
{
    for (int i = 0; i < hThread_length; i++)
        SuspendThread(hThreads[i]);
}

void RW::resume()
{
    for (int i = 0; i < hThread_length; i++)
        ResumeThread(hThreads[i]);
}
