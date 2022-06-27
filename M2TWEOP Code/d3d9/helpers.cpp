#include "helpers.h"
using namespace std;
vector<std::string> helpers::splitString(const std::string& phrase, const std::string& delimiter) {
    vector<string> list;
    string s = string(phrase);
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}


#define IPC_IMPLEMENTATION
#include "ipc.h"
void helpers::doEOPPipe(std::string& result, int waitSeconds)
{
    ULONGLONG startTime = GetTickCount64();
    ULONGLONG endTime = startTime + 1000ull * waitSeconds;

    ipc_sharedmemory mem;

    ipc_mem_init(&mem, (char*)"M2TWEOPStartMem1", 10000);

    do {
        if (ipc_mem_open_existing(&mem))
        {
            continue;//open failed
        }
        while (mem.data[0] != 1&& GetTickCount64() < endTime)
        {
            Sleep(5);
        }
        if (GetTickCount64() > endTime)
        {
            break;
        }
        if (mem.data[0] == mem.data[1] == mem.data[2] == mem.data[3] ==1)
        {
            result = (char*)mem.data;
            *mem.data = 0;
        }

    } while (GetTickCount64() < endTime && result.size() == 0);

    ipc_mem_close(&mem);
}