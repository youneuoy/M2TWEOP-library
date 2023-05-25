//
// Sintra library, example 0
//
// This program demonstrates simple point to point process communication, using one executable.
// A multi-process single executable program requires a call to init() in its main thread
// which initializes the sintra library, spawns the process swarm and starts the multi-process
// execution.
//
// In this example, there are 3 user processes, of which the first is acting as a sender, and
// the other two as receivers.
// Each receiver activates a slot function that handles arguments of a different type (int, string).
// The sender process sends messages of both types, which are handled by each process accordingly.
// A barrier is used to ensure that the slots are activated before the messages are sent.

#include <sintra/sintra.h>
#include <iostream>



using namespace std;
using namespace sintra;



int process_1()
{
    barrier("1st barrier"); // ensures that the slots receiving the messages have been activated.

    // send some messages
    world() << "good morning";
    world() << 1;
    world() << "good afternoon" << "good evening" << "good night";
    world() << 2 << 3 << 4;

    barrier("2nd barrier"); // ensures that the messages have been received and processed

    return 0;
}


int process_2()
{
    auto string_slot = [] (const string& str) {
        console() << "[process 2] Received string \"" << str << "\"\n";
    };

    activate_slot(string_slot);

    barrier("1st barrier"); // ensures that the slots receiving the messages have been activated.
    barrier("2nd barrier"); // ensures that the messages have been received and processed

    return 0;
}



int process_3()
{
    auto int_slot = [&] (int number) {
        console() << "[process 3] Received number " << number << "\n";
    };

    activate_slot(int_slot);

    barrier("1st barrier"); // ensures that the slots receiving the messages have been activated.
    barrier("2nd barrier"); // ensures that the messages have been received and processed

    return 0;
}



int main(int argc, char* argv[])
{
    init(argc, argv, process_1, process_2, process_3);

    // when finalize() returns, messaging will no longer work.
    finalize();

    if (process_index() == 0) {
        do {
            cout << '\n' << "Press ENTER to continue...";
        }
        while (cin.get() != '\n');
    }

    return 0;
}
