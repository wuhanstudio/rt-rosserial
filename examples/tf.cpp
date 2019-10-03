/* 
 * rosserial Time and TF Example
 * Publishes a transform at current time
 */

#include <rtthread.h>
#include <ros.h>
#include <ros/time.h>
#include <tf/transform_broadcaster.h>

ros::NodeHandle  nh;

geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;

char base_link[] = "/base_link";
char odom[] = "/odom";


static void rosserial_tf_thread_entry(void *parameter)
{
    nh.initNode();
    broadcaster.init(nh);
    while (1)
    {
        t.header.frame_id = odom;
        t.child_frame_id = base_link;
        t.transform.translation.x = 1.0; 
        t.transform.rotation.x = 0.0;
        t.transform.rotation.y = 0.0; 
        t.transform.rotation.z = 0.0; 
        t.transform.rotation.w = 1.0;  
        t.header.stamp = nh.now();
        broadcaster.sendTransform(t);
        nh.spinOnce();
        rt_thread_mdelay(10);
    }
}

static void rosserial_tf_example(int argc,char *argv[])
{
    rt_thread_t thread = rt_thread_create("r_tf", rosserial_tf_thread_entry, RT_NULL, 1024, 25, 10);
    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
        rt_kprintf("[rosserial] New thread r_tf\n");
    }
    else
    {
        rt_kprintf("[rosserial] Failed to create thread r_tf\n");
    }
}
MSH_CMD_EXPORT(rosserial_tf_example, roserial publish and subscribe with UART);

// If you are using Keil, you can ignore everything below

// This is required
// If you'd like to compile with scons which uses arm-none-eabi-gcc
// extern "C" void __cxa_pure_virtual()
// {
//     while (1);
//}

// Moreover, you need to add:
// CXXFLAGS = CFLAGS + ' -fno-rtti'
// in rtconfig.py for arm-none-eabi-gcc
