/* This is an example of how to use each of of these classes */
#include <example.h>
#include <unistd.h>

#include <iostream>

// This is the run function in the derived thread.
// It does the thread work.
// It could loop, or work and exit.

void m_thread::run() {
  while (1) {
    std::cout << "[" << get_name() << "]: This is a thread without real-time attributes.\n";
    sleep(1);
  }
}

// This is the loop thread that is called at the specified frequency
bool m_rt_thread::loop() {
  std::cout << "[" << get_name() << "]: This is the RT thread running at " << get_frequency() << " Hz.\n";
  return CONTINUE;
}