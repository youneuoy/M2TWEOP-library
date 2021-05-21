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

          
            
            
            //Check that memory was initialized to 1
            char* mem = static_cast<char*>(region.get_address());
         /*   int strSize = *mem;
            f1 << strSize << endl;
            std::string buf;
            buf.resize(strSize);
            //example of the function that can write to a buffer with 10-100 characters.
            memcpy(&buf[0], mem + sizeof(strSize), strSize);

            buf.resize(strSize);
            buf.shrink_to_fit();
           
            result = buf;*/
            result = mem;

            *mem = 0;

            region.flush();
        }
        catch (...)
        {

        }

    } while (GetTickCount64() < endTime&& result.size()==0);

}