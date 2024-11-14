// #include "thread.h"
#include "example.h"

#include <unistd.h>

#include <iostream>
#include <functional>

// start function with passing name
void thread::start(std::string name) {
  this->name = name;
  start();
}

// start function. uses rt boolean to determine if real-time attributes
// need to be set.  Attributes (like CPU afifnity) need to be set after the
// thread is started or there will be a seg fault 
void thread::start() {
  if (rt && !get_frequency()) {
    std::cout << "[" << get_name() << "]: No frequency specified."
              << std::endl;
    return;
  }

  std::cout << "[" << get_name() << "]: Creating thread.\n";
  thread_ = std::thread(std::bind(&thread::run, this));
  setup_thread();
}


// used for logging/debug output. That's it.
// TODO: Implement a real logging system
std::string thread::get_name() {
  if (name.empty()) {
    return "Unnamed";
  } else {
    return name;
  }
}

void thread::join() {
  if (thread_.joinable()) {
    thread_.join();
  }
}

// run function, but shouldn't be called if the class is derived.
void thread::run() {
  std::cout
      << "[" << get_name()
      << "]: Non-RT thread: derive this class and provide the run() function."
      << std::endl;
}

// Used to validate the rt derived class is set up to run
uint32_t thread::get_frequency() {
  return frequency;
}

// Instantiation for derived thread class with rt attributes
rt_thread::rt_thread() : thread() {
  rt = true;
  priority = sched_get_priority_max(policy) / 2;
  CPU_ZERO(&cpuset);
  num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
}

// thread runs a tight, real-time loop
// Using a timing scheme to iterate the loop function
// at the specified frequency
void rt_thread::run() {
  struct timespec start_time, end_time;
  uint64_t elapsed_ns, sleep_time;
  uint64_t target_ns = BILLION / frequency;
  bool ret;

  std::cout << "[" << get_name() << "]: Running at "
            << get_frequency() << " Hz.\n";

  while (true) {
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    ret = loop();
    if (ret == STOP) {
      break;
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    elapsed_ns = (end_time.tv_sec - start_time.tv_sec) * 1000000000 +
                 (end_time.tv_nsec - start_time.tv_nsec);
    sleep_time = target_ns - elapsed_ns;
    if (sleep_time) {
      struct timespec sleep_time_spec;
      sleep_time_spec.tv_sec = 0;
      sleep_time_spec.tv_nsec = sleep_time;
      nanosleep(&sleep_time_spec, NULL);
    }
  }
}

// This function is overwritten from the derived class and will 
// do all the real time work
bool rt_thread::loop() {
  std::cout << "RT thread: derive this class and provide the loop() function."
            << std::endl;
  return STOP;
}

// the following functions set the real time thread attributes
void rt_thread::setup_thread() {
  params.sched_priority = priority;
  pthread_attr_init(&attr);
  pthread_setschedparam(thread_.native_handle(), this->policy, &params);
  std::cout << "[" << get_name() << "]: \tPriority: " << unsigned(priority)
            << " (Max: " << sched_get_priority_max(policy) << ")" << std::endl;
  std::cout << "[" << get_name() << "]: \tPolicy:   " << unsigned(policy)
            << std::endl;

  /* it's set */
  if (cpu >= 0) {
    pthread_setaffinity_np(thread_.native_handle(), sizeof(cpu_set_t), &cpuset);
  }
  std::cout << "[" << get_name() << "]: \tCPUs:     ";
  for (uint16_t cpu = 0; cpu < CPU_SETSIZE; ++cpu) {
    if (CPU_ISSET(cpu, &cpuset)) {
      std::cout << cpu << " ";
    }
  }
  std::cout << std::endl;
}

void rt_thread::set_thread_priority(uint8_t priority) {
  this->priority = priority;
}

void rt_thread::set_thread_policy(uint8_t policy) {
  this->policy = policy;
}

void rt_thread::set_cpu(uint16_t cpu) {
  if (cpu > num_cpus - 1) {
    std::cout << "Specified CPU is greater than maximum CPUs." << std::endl;
  } else {
    this->cpu = cpu;
    CPU_SET(this->cpu, &cpuset);  // Set CPU affinity
  }
}

void rt_thread::set_frequency(uint32_t frequency) {
  this->frequency = frequency;
}
