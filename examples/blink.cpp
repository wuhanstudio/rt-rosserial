/* 
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */

#include <rtthread.h>
#include <board.h>
#include <ros.h>
#include <std_msgs/Empty.h>

#define LED0_PIN    GET_PIN(A, 5)

static ros::NodeHandle  nh;

static void messageCb( const std_msgs::Empty& toggle_msg){
    rt_pin_write(LED0_PIN, PIN_HIGH - rt_pin_read(LED0_PIN));   // blink the led
}

static ros::Subscriber<std_msgs::Empty> sub("toggle_led", &messageCb );

static void rosserial_blink_thread_entry(void *parameter)
{
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    nh.initNode();
    nh.subscribe(sub);

    while (1)
    {
        nh.spinOnce();
        rt_thread_mdelay(500);
    }
}

static void rosserial_blink_example(int argc,char *argv[])
{
    rt_thread_t thread = rt_thread_create("r_blink", rosserial_blink_thread_entry, RT_NULL, 1024, 25, 10);
    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
        rt_kprintf("[rosserial] New thread r_blink\n");
    }
    else
    {
        rt_kprintf("[rosserial] Failed to create thread r_blink\n");
    }
}
MSH_CMD_EXPORT(rosserial_blink_example, roserial blink example with UART);

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
