/*
 * rosserial Service Client
 */

#include <rtthread.h>
#include <ros.h>
#include <std_msgs/String.h>
#include "port/Test.h"

static ros::NodeHandle  nh;
using rosserial_rtt::Test;

static ros::ServiceClient<Test::Request, Test::Response> client("test_srv");

static std_msgs::String str_msg;
static ros::Publisher chatter("chatter", &str_msg);

static char hello[13] = "hello world!";

static void rosserial_client_thread_entry(void *parameter)
{
    nh.initNode();
    nh.serviceClient(client);
    nh.advertise(chatter);
    while(!nh.connected()) nh.spinOnce();
    nh.loginfo("Startup complete");

    while (1)
    {
        Test::Request req;
        Test::Response res;
        req.input = hello;
        client.call(req, res);
        str_msg.data = res.output;
        chatter.publish( &str_msg );
        nh.spinOnce();
        rt_thread_mdelay(10);
    }
}

static void rosserial_client_example(int argc,char *argv[])
{
    rt_thread_t thread = rt_thread_create("r_client", rosserial_client_thread_entry, RT_NULL, 1024, 25, 10);
    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
        rt_kprintf("[rosserial] New thread r_client\n");
    }
    else
    {
        rt_kprintf("[rosserial] Failed to create thread r_client\n");
    }
}
MSH_CMD_EXPORT(rosserial_client_example, roserial publish and subscribe with UART);

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
