//
// Sintra library, example 1
//
// This example demonstrates the usage of the interprocess console.
//
// In this example, there are 2 user processes, that send ping-pong messages
// to each other. Whenever a ping/pong message is received, a message is sent
// to the interprocess console. A third process is observing the other two and
// reports the ping-pong rate.
//

#include <sintra/sintra.h>
#include <iostream>


using namespace std;
using namespace sintra;


struct Ping {};
struct Pong {};
struct Stop {};


void wait_for_stop()
{
    std::condition_variable cv;
    std::mutex m;
    bool done = false;

    activate_slot([&](Stop) {
        std::lock_guard<std::mutex> lk(m);
        done = true;
        cv.notify_one();
    });

    barrier("stop slot activation barrier");

    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [&]{return done;});

    deactivate_all_slots();
}



int process_1()
{
    activate_slot([=] (Ping) {
        world() << Pong();
    });
    barrier("ping-pong slot activation barrier");

    wait_for_stop();
    return 0;
}


int process_2()
{
    activate_slot([=] (Pong) {
        world() << Ping();
    });
    barrier("ping-pong slot activation barrier");

    // the spark
    world() << Ping();

    wait_for_stop();
    return 0;
}


int process_3()
{
    double ref_time = get_wtime();
    double next_ts = ref_time + 1.;
    uint64_t counter = 0;

    activate_slot([&] (Ping) {
        double ts = get_wtime();
        if (ts > next_ts) {
            next_ts = ts + 1.;
            console() << counter << " ping-pongs / second\n";
            counter = 0;
        }
        counter++;
    });
    barrier("ping-pong slot activation barrier");

    wait_for_stop();
    return 0;
}


int main(int argc, char* argv[])
{
    init(argc, argv, process_1, process_2, process_3);

    if (process_index() == 0) {
        cout << "Press ENTER to stop the ping-pong" << "\n\n";
        while (cin.get() != '\n') {}
        world() << Stop();
    }

    // after finalize() returns, messaging no longer works.
    finalize();

    if (process_index() == 0) {
        do {
            cout << '\n' << "Press ENTER to continue...";
        }
        while (cin.get() != '\n');
    }

    return 0;
}
