/* This is an example of how to use each of of these classes */
#include <thread.h>
#include <string>
#include <readerwriterqueue/readerwriterqueue.h>

// The readerwriter queue needs a template to define the type of object it will
// hold. So, therefore it cannot edist on the parent class and must be specific
// to the derived classes So this example will create a data set to share
// between two threads. One will be RT and the other
// is not.

// The regular thread will get the time once per second and put it in a queue
// the RT thread will run at 10Hz, but only pick up the queue when there's data

struct timing {
  uint64_t ms;
  uint64_t us;
  uint64_t ns;
};

class q_thread : public thread {
 public:
  q_thread();
  moodycamel::ReaderWriterQueue<struct timing>* get_q();

 protected:
  void run() override;

 private:
  struct timing in_q_time;
  moodycamel::ReaderWriterQueue<struct timing> q;
};

class q_rt_thread : public rt_thread {
 public:
  void set_q(moodycamel::ReaderWriterQueue<struct timing> *);

 protected:
  bool loop() override;

 private:
  struct timing out_q_time;
  moodycamel::ReaderWriterQueue<struct timing> *q;
};