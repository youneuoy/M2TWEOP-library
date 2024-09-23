#include "helpers.h"
#include "json.hpp"

using namespace std;
vector<std::wstring> helpers::splitString(const std::wstring &phrase, const std::wstring &delimiter)
{
    vector<wstring> list;
    wstring s = wstring(phrase);
    size_t pos = 0;
    wstring token;
    while ((pos = s.find(delimiter)) != string::npos)
    {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}

#define BOOST_DATE_TIME_NO_LIB 1
#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
std::wstring helpers::doEOPPipe(int waitSeconds, bool cleanup)
{   
    std::wstring result;
    ULONGLONG startTime = GetTickCount64();
    ULONGLONG endTime = startTime + 1000ull * waitSeconds;
    namespace bip = boost::interprocess;

    do
    {
        //boost way ;(
        try
        {
            //Open already created shared memory object.
            bip::windows_shared_memory shm(bip::open_only, "M2TWEOPStartMem1", bip::read_write);
            //Map the whole shared memory in this process
            bip::mapped_region region(shm, bip::read_write);
            wchar_t* mem = static_cast<wchar_t*>(region.get_address());
            result = mem;

            if (cleanup == true) {
                *mem = 0;
                region.flush();
            }
        }
        catch (...)
        {
        }

    } while (GetTickCount64() < endTime && result.size() == 0);

    return result;
}

std::wstring helpers::getModFolderFromPipe(const wstring &msg)
{
    vector<wstring> args = helpers::splitString(msg, L"\n");
    return args[2];
}
bool IsEopPipeMessage(const wstring& msg)
{
    vector<wstring>args = helpers::splitString(msg, L"\n");
    if (args.size() != 5)
    {
        return false;
    }

    if (args[0] != L"m2tweopStartCommand")
    {
        return false;
    }

    if (args[1] != L"eopModFolder:")
    {
        return false;
    }

    if (args[3] != L"GameVer:")
    {
        return false;
    }

    return true;
}
std::wstring helpers::getModPathFromSharedMemory()
{
    wstring resMsg = helpers::doEOPPipe(5, false);
    if (IsEopPipeMessage(resMsg) == true)
    {
        return helpers::getModFolderFromPipe(resMsg);
    }
    else
    {
        return L"";
    }
}

jsn::json helpers::loadJsonFromFile(const std::wstring &fpath)
{
    try
    {
        jsn::json json;

        std::ifstream f1(fpath);
        if (f1.is_open())
        {
            f1 >> json;
        }
        f1.close();
        return std::move(json);
    }
    catch (...)
    {
    }
}