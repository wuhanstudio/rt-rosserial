
#ifndef _ROS_H_
#define _ROS_H_

#include "ros/node_handle.h"
#include "RTTHardware.h"

namespace ros
{
  typedef NodeHandle_<RTTHardware> NodeHandle;
}

#endif
