/* This is an example of how to use each of of these classes */
#include <example_queue.h>
#include <unistd.h>

#include <iostream>

// Just a function that was used to capture timing.
struct timing get_timing() {
  struct timing this_timing;
  static struct timespec start_time;
  struct timespec end_time;
  uint64_t ms, us, ns;
  if (!start_time.tv_sec) {
    clock_gettime(CLOCK_MONOTONIC, &start_time);
  }
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  ms = (end_time.tv_sec - start_time.tv_sec) * THOUSAND;
  ms += (end_time.tv_nsec - start_time.tv_nsec) / MILLION;

  us = (end_time.tv_sec - start_time.tv_sec) * MILLION;
  us += (end_time.tv_nsec - start_time.tv_nsec) / THOUSAND;

  ns = (end_time.tv_sec - start_time.tv_sec) * BILLION;
  ns += (end_time.tv_nsec - start_time.tv_nsec);

  this_timing.ms = ms;
  this_timing.us = us;
  this_timing.ns = ns;

  return this_timing;
}


// Constructor. Do any thread specific set up here.
q_thread::q_thread() : thread() {
  // start the timer
  get_timing();
  // allocate more than 1.. not that this example needs it
  moodycamel::ReaderWriterQueue<struct timing> q(10);
}


void q_thread::run() {
  bool succeeded;
  while (1) {
    std::cout << "[" << get_name() << "]: Placing data into the queue.\n";
    in_q_time = get_timing();
    succeeded = q.try_enqueue(in_q_time);  // Will only succeed if the queue has
                                           // an empty slot (never allocates)
    assert(succeeded);
    usleep(1'000'000);
  }
}

// helper class to return the queue that was defined
moodycamel::ReaderWriterQueue<struct timing>* q_thread::get_q() {
  std::cout << "q: " << &q << std::endl;
  return &q;
}

// Loop that is called at the specified frequency
bool q_rt_thread::loop() {
  if (q->peek()) {
    bool succeeded = q->try_dequeue(out_q_time);
    assert(succeeded);
    std::cout << "[" << get_name()
              << "]: Got data from queue: " << out_q_time.ms << std::endl;
  }

  // return STOP and the thread ends
  return CONTINUE;
}

// Helper function to set the queue on the RT thread to the same
// queue on the regular thread
void q_rt_thread::set_q(moodycamel::ReaderWriterQueue<struct timing>* q) {
  this->q = q;
}
