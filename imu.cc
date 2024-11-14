/* This is an example of how to use each of of these classes */
#include <imu.h>
#include <unistd.h>

#include <iostream>

// This is the run function in the derived thread.
// It does the thread work.
// It could loop, or work and exit.

void imu_thread::run() {
  while (1) {
    std::cout << "[" << get_name() << "]: This is a thread without real-time attributes.\n";
    sleep(1);
  }
}
