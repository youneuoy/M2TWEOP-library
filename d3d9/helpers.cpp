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
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
void helpers::doEOPPipe(std::string& result, int waitSeconds)
{
    Sleep(waitSeconds * 1000);
    //boost way ;(
    try
    {
        using namespace boost::interprocess;


        typedef boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager> CharAllocator;
        typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> ourS;

        //Open already created shared memory object.
        managed_shared_memory shm(open_only, "M2TWEOPStartPipe");

        std::pair<ourS*, std::size_t> ret = shm.find_no_lock<ourS>("EOPStr");
        if (ret.first != nullptr)
        {
            result = (ret.first->c_str());
            shm.destroy<ourS>("EOPStr");
        }


    }
    catch (...)
    {
    }

}