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
#include <boost/interprocess/managed_shared_memory.hpp>
void helpers::doEOPPipe(std::string& result, int waitSeconds)
{
    Sleep(waitSeconds * 1000);
    using namespace boost::interprocess;
    //boost way ;(
    try
    {
        int correctResponce = 0;
        //Open already created shared memory object.
        shared_memory_object shm(open_only, "M2TWEOPStartMem", read_write);

        //Map the whole shared memory in this process
        mapped_region region(shm, read_write);

        //Check that memory was initialized to 1
        char* mem = static_cast<char*>(region.get_address());
        int strSize = *mem;

        std::string buf;
        buf.resize(strSize);
        //example of the function that can write to a buffer with 10-100 characters.
        memcpy(&buf[0], mem + sizeof(strSize), strSize);
        buf.resize(strSize);
        buf.shrink_to_fit();

        result = buf;
        *mem = correctResponce;

        region.flush();

        /*
        typedef boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager> CharAllocator;
        typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> ourS;

        //Open already created shared memory object.
        managed_shared_memory shm(open_only, "M2TWEOPStartPipe",0,);

        std::pair<ourS*, std::size_t> ret = shm.find_no_lock<ourS>("EOPStr");
        if (ret.first != nullptr && ret.second!=0)
        {
            result = (ret.first->c_str());
            shm.destroy<ourS>("EOPStr");
        }
        */


    }
    catch (...)
    {
    }

}