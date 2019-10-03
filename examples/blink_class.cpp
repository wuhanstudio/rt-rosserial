#include <rtthread.h>
#include <board.h>
#include <ros.h>
#include <std_srvs/SetBool.h>
#include <std_msgs/UInt16.h>

#define LED0_PIN    GET_PIN(A, 5)

#define millis() ((unsigned long)rt_tick_get() * 1000 / RT_TICK_PER_SECOND)

static ros::NodeHandle nh;

class Blinker
{
public:
  Blinker(rt_base_t pin, uint16_t period)
  : pin_(pin), period_(period),
    subscriber_("set_blink_period", &Blinker::set_period_callback, this),
    service_server_("activate_blinker", &Blinker::service_callback, this)
  {
      active_ = RT_TRUE;
  }

  void init(ros::NodeHandle& nh)
  {
    rt_pin_mode(pin_, PIN_MODE_OUTPUT);
    nh.subscribe(subscriber_);
    nh.advertiseService(service_server_);
  }

  void run()
  {
    if(active_ && ((millis() - last_time_) >= period_))
    {
      last_time_ = millis();
      rt_pin_write(pin_, !rt_pin_read(pin_));
    }
  }

  void set_period_callback(const std_msgs::UInt16& msg)
  {
    period_ = msg.data;
  }

  void service_callback(const std_srvs::SetBool::Request& req,
                              std_srvs::SetBool::Response& res)
  {
    active_ = req.data;
    res.success = true;
    if(req.data)
    {
      res.message = "Blinker ON";
    }
    else
    {
      res.message = "Blinker OFF";
    }
  }

private:
  const rt_base_t pin_;
  rt_bool_t active_;
  uint16_t period_;
  uint32_t last_time_;
  ros::Subscriber<std_msgs::UInt16, Blinker> subscriber_;
  ros::ServiceServer<std_srvs::SetBool::Request, std_srvs::SetBool::Response, Blinker> service_server_;
};

static Blinker blinker(LED0_PIN, 500);

static void rosserial_blink_class_thread_entry(void *parameter)
{
    nh.initNode();
    blinker.init(nh);

    while (1)
    {
        blinker.run();
        nh.spinOnce();
        rt_thread_mdelay(500);
    }
}

static void rosserial_blink_class_example(int argc,char *argv[])
{
    rt_thread_t thread = rt_thread_create("r_blinkc", rosserial_blink_class_thread_entry, RT_NULL, 1024, 25, 10);
    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
        rt_kprintf("[rosserial] New thread r_blinkc\n");
    }
    else
    {
        rt_kprintf("[rosserial] Failed to create thread r_blinkc\n");
    }
}
MSH_CMD_EXPORT(rosserial_blink_class_example, roserial blink class example with UART);

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
