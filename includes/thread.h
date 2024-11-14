#ifndef THREAD_H
#define THREAD_H

#include <iostream>
#include <string>
#include <thread>

#define THOUSAND 1000LL
#define MILLION 1000000LL
#define BILLION 1000000000LL

/* simple thread wrapper and should be derived for functionality */

class thread {
 public:
  thread() = default;
  virtual ~thread() { join(); }

  void start();
  void start(std::string);
  void join();

 protected:
  std::thread thread_;
  std::string name;
  // boolean set when thread is real-time, used in derived class
  bool rt = false;

  virtual void setup_thread() {};
  uint32_t frequency = 0;
  uint32_t get_frequency();

  // Run must be provided by derived class
  virtual void run();
  std::string get_name();

 private:
};

/* Dervied thread class. Needs to be derived further for full functionality */

class rt_thread : public thread {
 public:
  rt_thread();
  ~rt_thread() override = default;

  // RT specific functionality
  void set_thread_priority(uint8_t);
  void set_thread_policy(uint8_t);
  void set_cpu(uint16_t);
  void set_frequency(uint32_t);

 protected:
  void run() override;
  // Loop must be provided by derived class and is executed
  // at frequency
  virtual bool loop();
  void setup_thread();

  // default scheduling policy, typical for RT
  int8_t policy = SCHED_FIFO;
  // loop control (see example)
  const uint8_t STOP = 0;
  const uint8_t CONTINUE = 1;

  // RT specific vars
 private:
  uint8_t num_cpus;
  uint8_t priority;
  int16_t cpu = -1;
  pthread_attr_t attr;
  struct sched_param params;
  cpu_set_t cpuset;
};

#endif  // THREAD_H
