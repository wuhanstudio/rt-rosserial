#include <rtthread.h>
#include <ros.h>
#include <std_msgs/String.h>

static ros::NodeHandle  nh;
static std_msgs::String str_msg;
static ros::Publisher chatter("chatter", &str_msg);

static char hello_msg[13] = "hello world!";

static void rosserial_thread_entry(void *parameter)
{
    // Please make sure you have network connection first
    // Set ip address and port
    nh.getHardware()->setConnection("192.168.199.100", 11411);

    nh.initNode();
    nh.advertise(chatter);

    while (1)
    {
        if ( nh.connected() ) 
        {
            str_msg.data = hello_msg;
            chatter.publish( &str_msg );
        } 
        /*
        else 
        {
            rt_kprintf("[rosserial] Not Connected \n");
        }
        */
        nh.spinOnce();
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
}

static void rosserial_hello_world_tcp_example(int argc,char *argv[])
{
    rt_thread_t thread = rt_thread_create("rosserial", rosserial_thread_entry, RT_NULL, 2048, 25, 10);
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
MSH_CMD_EXPORT(rosserial_hello_world_tcp_example, roserial hello world example with TCP);


// If you are using Keil, you can ignore everything below

// This is required
// If you'd like to compile with scons which uses arm-none-eabi-gcc
// extern "C" void __cxa_pure_virtual()
// {
//     while (1);
// }

// Moreover, you need to add:
// CXXFLAGS = CFLAGS + ' -fno-rtti'
// in rtconfig.py for arm-none-eabi-gcc
