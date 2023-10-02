//
// Sintra library, example 3
//
// This example demonstrates message dispatch, within the same process.
// It is very similar to example #1.
//
// In this example, there is only one process, with 3 registered slots.
// Two of them reply with a message of the opposite type of what they are
// handling. The third is an observer that is counting the cycles.
// 

#include <sintra/sintra.h>
#include <iostream>


using namespace std;
using namespace sintra;


struct Ping {};
struct Pong {};

static double timeout_in_seconds = 10.0;

int main(int argc, char* argv[])
{
    init(argc, argv);

    auto ping_slot = [](Ping) {
        world() << Pong();
    };

    auto pong_slot = [](Pong) {
        world() << Ping();
    };

    double ts = get_wtime();
    double next_ts = ts + 1.;
    uint64_t counter = 0;

    auto benchmark_slot = [&](Ping) {
        double ts = get_wtime();
        if (ts > next_ts) {
            next_ts = ts + 1.;
            console() << counter << " ping-pongs / second\n";
            counter = 0;
        }
        counter++;
    };

    activate_slot(ping_slot);
    activate_slot(pong_slot);
    activate_slot(benchmark_slot);

    // the spark
    world() << Ping();

    std::this_thread::sleep_for(std::chrono::duration<double>(timeout_in_seconds));

    finalize();

    return 0;
}
