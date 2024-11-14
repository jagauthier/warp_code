/* This is an example of how to use each of of these classes */
#include <thread.h>
#include <string>

class m_thread : public thread {
 public:

 protected:
    void run() override;
};

class m_rt_thread : public rt_thread {
 public:
 protected:
  bool loop() override;
};