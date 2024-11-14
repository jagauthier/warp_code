/* This is an example of how to use each of of these classes */
#include <thread.h>
#include <string>

class imu_thread : public thread {
 public:


 protected:
    void run() override;
};

