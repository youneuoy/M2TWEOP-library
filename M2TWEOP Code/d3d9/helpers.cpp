#include "helpers.h"
#include "json.hpp"

using namespace std;
vector<std::string> helpers::splitString(const std::string &phrase, const std::string &delimiter)
{
    vector<string> list;
    string s = string(phrase);
    size_t pos = 0;
    string token;
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
std::string helpers::doEOPPipe(int waitSeconds, bool cleanup)
{   
    std::string result;
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
            char* mem = static_cast<char*>(region.get_address());
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

std::string helpers::getModFolderFromPipe(const string &msg)
{
    vector<string> args = helpers::splitString(msg, "\n");
    return args[2];
}
bool IsEopPipeMessage(const string& msg)
{
    vector<string>args = helpers::splitString(msg, "\n");
    if (args.size() != 5)
    {
        return false;
    }

    if (args[0] != "m2tweopStartCommand")
    {
        return false;
    }

    if (args[1] != "eopModFolder:")
    {
        return false;
    }

    if (args[3] != "GameVer:")
    {
        return false;
    }

    return true;
}
std::string helpers::getModPathFromSharedMemory()
{
    string resMsg = helpers::doEOPPipe(5, false);
    if (IsEopPipeMessage(resMsg) == true)
    {
        return helpers::getModFolderFromPipe(resMsg);
    }
    else
    {
        return "";
    }
}

jsn::json helpers::loadJsonFromFile(const std::string &fpath)
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