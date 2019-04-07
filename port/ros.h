
#ifndef _ROS_H_
#define _ROS_H_

#include "ros/node_handle.h"

#if defined(ROSSERIAL_USE_TCP)
  #include "RTTTcpHardware.h"
#elif defined(ROSSERIAL_USE_SERIAL)
  #include "RTTHardware.h"
#endif

namespace ros
{
#if defined(ROSSERIAL_USE_TCP)
  typedef NodeHandle_<RTTTcpHardware> NodeHandle;
#elif defined(ROSSERIAL_USE_SERIAL)
  typedef NodeHandle_<RTTHardware> NodeHandle;
#endif
}

#endif
