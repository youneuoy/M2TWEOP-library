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


#define BOOST_DATE_TIME_NO_LIB 1
#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
void helpers::doEOPPipe(std::string& result, int waitSeconds)
{
    ULONGLONG startTime = GetTickCount64();
    ULONGLONG endTime = startTime + 1000ull * waitSeconds;
    namespace bip = boost::interprocess;

    do {
        //boost way ;(
        try
        {
            //Open already created shared memory object.
            bip::windows_shared_memory shm(bip::open_only, "M2TWEOPStartMem1", bip::read_write);
            //Map the whole shared memory in this process
            bip::mapped_region region(shm, bip::read_write);
            char* mem = static_cast<char*>(region.get_address());
            result = mem;

            *mem = 0;

            region.flush();
        }
        catch (...)
        {

        }

    } while (GetTickCount64() < endTime&& result.size()==0);

}