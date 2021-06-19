/*
 * rosserial kobuki control Example
 * Control kobuki robot using teleop
 */

#include <rtthread.h>
#include <kobuki.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>

static ros::NodeHandle  nh;

static struct kobuki robot;
static double linear_x = 0;
static double angular_z = 0;

static void kobuki_entry(void *parameter)
{
    kobuki_init(&robot);
    while(1)
    {
        rt_thread_mdelay(100);
        kobuki_set_speed(linear_x, angular_z);
    }
    kobuki_close(&robot);
}

static void messageCb( const geometry_msgs::Twist& twist_msg)
{
    rt_kprintf("[rosserial] received control cmd\n");
    linear_x  = twist_msg.linear.x;
    angular_z = twist_msg.angular.z;
}

static ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", messageCb );

static void rosserial_kobuki_thread_entry(void *parameter)
{
    // Please make sure you have network connection first
    // Set ip address and port
#if defined ROSSERIAL_USE_TCP
    nh.getHardware()->setConnection("192.168.199.100", 11411);
#endif

    nh.initNode();
    nh.subscribe(sub);
    while (1)
    {
        nh.spinOnce();
        rt_thread_mdelay(100);
    }
}

void rosserial_kobuki_control(int argc,char *argv[])
{
    rt_thread_t thread = rt_thread_create("r_kobuki", rosserial_kobuki_thread_entry, RT_NULL, 2048, 15, 10);
    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
        rt_kprintf("[rosserial] New thread r_kobuki\n");
    }
    else
    {
        rt_kprintf("[rosserial] Failed to create thread r_pubsub\n");
    }

    rt_thread_t threadk = rt_thread_create("kobuki", kobuki_entry, RT_NULL, 2048, 10, 10);
    if(threadk != RT_NULL)
    {
        rt_thread_startup(threadk);
        rt_kprintf("[micro_ros] New thread kobuki\n");
    }
    else
    {
        rt_kprintf("[micro_ros] Failed to create thread kobuki\n");
    }
}
MSH_CMD_EXPORT(rosserial_kobuki_control, roserial control kobuki);
