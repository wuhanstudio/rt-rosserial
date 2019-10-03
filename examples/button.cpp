/* 
 * Button Example for Rosserial
 */

#include <rtthread.h>
#include <board.h>
#include <ros.h>
#include <std_msgs/Bool.h>

#define millis() ((unsigned long)rt_tick_get() * 1000 / RT_TICK_PER_SECOND)

static ros::NodeHandle nh;

static std_msgs::Bool pushed_msg;
static ros::Publisher pub_button("pushed", &pushed_msg);

static const int button_pin = GET_PIN(C, 8);
static const int led_pin = GET_PIN(A, 5);

static bool last_reading;
static long last_debounce_time=0;
static long debounce_delay=50;
static bool published = true;

static void rosserial_button_thread_entry(void *parameter)
{
    nh.initNode();
    nh.advertise(pub_button);

    //initialize an LED output pin 
    //and a input pin for our push button
    rt_pin_mode(led_pin, PIN_MODE_OUTPUT);
    rt_pin_mode(button_pin, PIN_MODE_INPUT);

    //Enable the pullup resistor on the button
    rt_pin_write(button_pin, PIN_HIGH);

    //The button is a normally button
    last_reading = ! rt_pin_read(button_pin);

    while (1)
    {
        bool reading = ! rt_pin_read(button_pin);

        if (last_reading!= reading){
            last_debounce_time = millis();
            published = false;
        }

        //if the button value has not changed for the debounce delay, we know its stable
        if ( !published && (millis() - last_debounce_time)  > debounce_delay) 
        {
            rt_pin_write(led_pin, reading);
            pushed_msg.data = reading;
            pub_button.publish(&pushed_msg);
            published = true;
        }

        last_reading = reading;

        nh.spinOnce();
        rt_thread_mdelay(500);
    }
}

static void rosserial_button_example(int argc,char *argv[])
{
    rt_thread_t thread = rt_thread_create("r_btn", rosserial_button_thread_entry, RT_NULL, 1024, 25, 10);
    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
        rt_kprintf("[rosserial] New thread r_btn\n");
    }
    else
    {
        rt_kprintf("[rosserial] Failed to create thread r_btn\n");
    }
}
MSH_CMD_EXPORT(rosserial_button_example, roserial button example with UART);

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
