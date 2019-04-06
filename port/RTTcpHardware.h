#ifndef ROS_RTT_TCP_HARDWARE_H_
#define ROS_RTT_TCP_HARDWARE_H_

#include <rtthread.h>

class RTTHardware {
public:
  RTTHardware()
  {
  }

  void setConnection()
  {
  }

  IPAddress getLocalIP()
  {

  }

  void init() {

  }

  int read() {

  }

  void write(const uint8_t* data, int length) {

  }

  unsigned long time() {

  }

protected:
  // IPAddress server_;
  uint16_t serverPort_ = 11411;
};

#endif
