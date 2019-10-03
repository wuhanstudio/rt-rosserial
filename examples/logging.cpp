/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */

#include <rtthread.h>
#include <board.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>

static ros::NodeHandle  nh;

static std_msgs::String str_msg;
static ros::Publisher chatter("chatter", &str_msg);

static char hello[13] = "hello world!";

static char debug[]= "debug statements";
static char info[] = "infos";
static char warn[] = "warnings";
static char error[] = "errors";
static char fatal[] = "fatalities";

static void rosserial_logging_thread_entry(void *parameter)
{
    nh.initNode();
    nh.advertise(chatter);
    while (1)
    {
        str_msg.data = hello;
        chatter.publish( &str_msg );

        nh.logdebug(debug);
        nh.loginfo(info);
        nh.logwarn(warn);
        nh.logerror(error);
        nh.logfatal(fatal);

        nh.spinOnce();
        rt_thread_mdelay(500);
    }
}

static void rosserial_logging_example(int argc,char *argv[])
{
    rt_thread_t thread = rt_thread_create("r_log", rosserial_logging_thread_entry, RT_NULL, 1024, 25, 10);
    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
        rt_kprintf("[rosserial] New thread r_log\n");
    }
    else
    {
        rt_kprintf("[rosserial] Failed to create thread r_log\n");
    }
}
MSH_CMD_EXPORT(rosserial_logging_example, roserial publish and subscribe with UART);

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
