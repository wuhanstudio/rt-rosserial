#ifndef ROS_RTT_TCP_HARDWARE_H_
#define ROS_RTT_TCP_HARDWARE_H_

#include <rtthread.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>

#ifndef ROSSERIAL_TCP_RECV_TIMEOUT
    #define ROSSERIAL_TCP_RECV_TIMEOUT 200000
#endif

#ifndef ROSSERIAL_TCP_SEND_TIMEOUT
    #define ROSSERIAL_TCP_SEND_TIMEOUT 200000
#endif

class RTTTcpHardware {
public:
    RTTTcpHardware()
    {
        server_ = "127.0.0.1";
        serverPort_ = 11411;
    }

    void setConnection(const char* url, int port = 11411)
    {
        server_ = url;
        serverPort_ = port;
    }

    void init() {
        struct hostent *host;
        struct sockaddr_in server_addr;
        host = gethostbyname(this->server_);

        if ((this->sock_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            rt_kprintf("[rosserial] Socket error\n");
            return;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(this->serverPort_);
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
        rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

        // Receive timeout
        struct timeval recv_timeout;
        recv_timeout.tv_sec =    0;
        recv_timeout.tv_usec = ROSSERIAL_TCP_RECV_TIMEOUT;
        setsockopt(this->sock_, SOL_SOCKET, SO_RCVTIMEO,
                  (void *) &recv_timeout, sizeof(recv_timeout));

        // Send timeout
        struct timeval send_timeout;
        send_timeout.tv_sec =    0;
        send_timeout.tv_usec = ROSSERIAL_TCP_SEND_TIMEOUT;
        setsockopt(this->sock_, SOL_SOCKET, SO_SNDTIMEO,
                  (void *) &send_timeout, sizeof(send_timeout));

        if (connect(this->sock_, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
        {
            rt_kprintf("[rosserial] Connect fail!\n");
            closesocket(this->sock_);
            return;
        }
        rt_kprintf("[rosserial] Connect successful\n");
    }

    int read() {
        char ch[2];
        int bytes_received = recv(this->sock_, ch, 1, 0);
        if(bytes_received > 0)
        {
            return ch[0];
        }
        else
        {
            return -1;
        }
    }

    void write(const uint8_t* data, int length) {
        send(this->sock_, data, length, 0);
    }

    unsigned long time() {
        return ((unsigned long)rt_tick_get() * 1000 / RT_TICK_PER_SECOND);
    }

protected:
    const char *server_;
    rt_uint16_t serverPort_;
    int sock_;
};

#endif
