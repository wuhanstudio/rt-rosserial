#include <rtthread.h>
#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle  nh;
std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

char hello_msg[13] = "hello world!";

static void rosserial_thread_entry(void *parameter)
{
    nh.initNode();
    nh.advertise(chatter);
    while (1)
    {
        str_msg.data = hello_msg;
        chatter.publish( &str_msg );
        nh.spinOnce();
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
}

static void rosserial_hello_world_serial_example(int argc,char *argv[])
{
    rt_thread_t thread = rt_thread_create("rosserial", rosserial_thread_entry, RT_NULL, 1024, 25, 10);
    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
        rt_kprintf("[rosserial] New thread rosserial\n");
    }
    else
    {
        rt_kprintf("[rosserial] Failed to create thread rosserial\n");
    }
}
MSH_CMD_EXPORT(rosserial_hello_world_serial_example, roserial hello world example with UART);
