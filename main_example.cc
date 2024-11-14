#include <iostream>

#include "example.h"
#include "example_queue.h"
#include "thread.h"

// Example one uses two classes derived in example.cc
// It creates a regular thread and a real time thread.

void example1() {
  m_thread thread1;
  m_rt_thread thread2;

  thread1.start("Thread");
  // This must be set or the thread will not start
  // priority, policy, and cpu affinity are optional
  thread2.set_frequency(2);
  thread2.start("Thread RT");

  // join the threads so they just wait for completion
  thread1.join();
  thread2.join();
}

// Example two uses two derived classed in example_queue.cc
// This implements the single producer/single consumer queue
// Thread 1 is non-rt and puts a timing struct in the queue once per second.
// Thread 2 is a real time thread that runs at 100Hz and basically
// doesn't do anything except pick up the queue message and show
// one of thethe time attributes that was placed in it.

void example2() {
  q_thread thread1;
  q_rt_thread thread2;

  thread2.set_q(thread1.get_q());
  thread1.start("Q put");
  thread2.set_frequency(100);
  thread2.start("Q get");
  thread1.join();
  thread2.join();
}

int main() { example2(); }
