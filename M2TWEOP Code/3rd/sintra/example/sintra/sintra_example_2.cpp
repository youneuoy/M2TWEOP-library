//
// Sintra library, example 2
//
// This example introduces Transceivers, RPC and demonstrates
// the following library features:
// - how member functions of Transceiver derivatives can be exported for RPC
// - how to assign a name on the object, which is usable across processes
// - how to make the remote call of the exported method, using the known
//   name of the object
// - RPC failure and interprocess exceptions
//
// In this example, Remotely_accessible is a struct defined to offer an
// 'append' method, which takes a string and an integer and returns another
// string with its arguments concatenated.
// If the supplied string exceeds 10 characters, it throws an exception,
// which is then transferred to the calling process.
// If the integer is 2020, the program should crash, which, to the other
// processes of the swarm, is communicated with an RPC failure exception
// and an abnormal termination signal. The latter is not covered in this
// example.
// Finally, attempting to call a non-existent instance (which will also be
// the case if the process it lived in has crashed) will cause another
// exception.
// 
// A Remotely_accessible instance is created and used locally in process_1,
// and remotely in process_2.
// 
// The program could output:
// 
// 2000: Sydney
// 2004: Athens
// 2008: Beijing
// An RPC exception was thrown: string too long
// An RPC exception was thrown: RPC failed
// An RPC exception was thrown: Attempted to make an RPC call using an
// invalid instance ID.
//
// nevertheless, the output is not really deterministic, as it depends on
// the timing of the call that will cause process_1 to crash, thus 'Sydney'
// and 'Athens' might theoretically not show at all.
//

#include <sintra/sintra.h>
#include <iostream>


using namespace std;
using namespace sintra;



struct Remotely_accessible: Derived_transceiver<Remotely_accessible>
{
    string append(const string& s, int v)
    {
        if (s.size() > 10) {
            throw std::logic_error("string too long");
        }
        else
        if (v == 2020) {
            // crash
            *((int*)0) = 0;
        }
        return to_string(v) + ": " + s;
    }

    // The following statement exports the member function 'append' for RPC use.
    // A static function 'rpc_append' is defined here, which could be called as follows:
    //
    //     Remotely_accessible::rpc_append(instance_id, some_string, some_int);
    // 
    // when the instance id is known, or
    // 
    //     Remotely_accessible::rpc_append("instance name", some_string, some_int);
    //
    // when the instance is named. The latter requires "instance name" to be resolved
    // to the actual instance id, which will internally cause another interprocess call
    // the first time this resolution takes place. The id will then be cached locally.
    // If the instance is local to the process, the call will bypass the RPC mechanism
    // and resort to a local function call.
    SINTRA_RPC(append)

    // This statement will have a similar effect, with the difference that the function
    // will always use the RPC mechanism, regardless of whether the instance is local to
    // the process.
    //SINTRA_RPC_STRICT(append)
};



void print_city_year(const char* city, int year)
{
    try {
        console() << Remotely_accessible::rpc_append("instance name", city, year) << "\n";
    }
    catch (std::exception& e) {
        console() << "An RPC exception was thrown: " << e.what() << "\n";
    }
}



int process_1()
{
    Remotely_accessible ra;

    ra.assign_name("instance name");

    // ensure that the instance has been named before attempting
    // to access it from another process
    barrier("1st barrier");

    // The RPC mechanism may as well be used with local Transceiver instances.
    // Depending on how the method is exported, it may either resort to a local call,
    // or be forced to go through the interprocess communication rings.
    print_city_year("Sydney", 2000);
    print_city_year("Athens", 2004);

    // ensure that ra still exists
    barrier("2nd barrier");
    return 0;
}



int process_2()
{
    // ensure that the object has been named before attempting
    // to access it with its name
    barrier("1st barrier");

    string test_string;
    print_city_year("Beijing", 2008);
    print_city_year("Rio de Janeiro", 2016);
    print_city_year("Tokyo", 2020);
    print_city_year("Paris", 2024);

    // ensure that the remotely accessible instance still exists
    barrier("2nd barrier");

    return 0;
}


int main(int argc, char* argv[])
{
    init(argc, argv, process_1, process_2);
    finalize();

    if (process_index() == 0) {
        do {
            cout << '\n' << "Press ENTER to continue...";
        } while (cin.get() != '\n');
    }

    return 0;
}
