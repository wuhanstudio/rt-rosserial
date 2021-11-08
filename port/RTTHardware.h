#ifndef ROS_RTT_HARDWARE_H_
#define ROS_RTT_HARDWARE_H_
#include <rtthread.h>
#include <rtdevice.h>

#ifndef ROSSERIAL_UART_NAME
    #define ROSSERIAL_UART_NAME "uart2"
#endif

static rt_device_t serial;
static struct rt_semaphore rx_sem;

// RX Callback
static rt_err_t uart_input(rt_device_t dev, rt_size_t size) {
    rt_sem_release(&rx_sem);
    return RT_EOK;
}

class RTTHardware {
    public:
        RTTHardware() {
            baud_ = BAUD_RATE_57600;
            deviceName_ = ROSSERIAL_UART_NAME;
        }

        RTTHardware(RTTHardware& h) {
            this->baud_ = h.baud_;
            this->deviceName_ = h.deviceName_;
        }
    
        void setBaud(long baud) {
            this->baud_= baud;
        }
    
        int getBaud() {
            return baud_;
        }

        void init() {
            serial = rt_device_find(this->deviceName_);
            if (!serial) {
                // Device Not Fond
                // rt_kprintf("roserial find %s failed!\n", "this->deviceName_");
                return;
            }

            /* Initialize Semaphore */
            rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
            /* config Serial */
            struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
            config.baud_rate = this->baud_;
            config.data_bits = DATA_BITS_8;
            config.stop_bits = STOP_BITS_1;
            config.parity    = PARITY_NONE;
            rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);
            /* Open Serial */
            rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
            /* Set RX Callback */
            rt_device_set_rx_indicate(serial, uart_input);
        }

        int read() {
            rt_uint8_t ch;
            if(rt_device_read(serial, -1, &ch, 1) != 1) {
                rt_sem_take(&rx_sem, 0);
                if(rt_device_read(serial, -1, &ch, 1) != 1) {
                    return -1;
                }
            };
            return ch;
        };

        void write(uint8_t* data, int length) {
            rt_device_write(serial, 0, data, length);
        }

        unsigned long time() {
            return ((unsigned long)rt_tick_get() * 1000 / RT_TICK_PER_SECOND);
        }

    protected:
        long baud_;
        const char* deviceName_;
};

#endif
