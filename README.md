# rt-rosserial

## 1、介绍


### 1.1 目录结构

> 说明：目录结构

| 名称 | 说明 |
| ---- | ---- |
| docs  | 文档目录 |
| examples | 例子目录|
| src  | 源代码目录 |
| port | 针对特定平台的移植代码目录 |

### 1.2 许可证

BSD 3-Clause License

### 1.3 依赖

可以选择下面2中通信方式中的一种：

- UART
- TCP 

如果使用 TCP 当然要先确保 rt-thread 有网卡设备，并获取到 IP 地址，例如：

- enc28j60 (SPI) + lwip。

### 2、如何打开 rosserial

使用 rosserial package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

	RT-Thread online packages
    	peripheral libraries and drivers --->
        	[*] rosserial: a protocol for Robots Operating System (ROS)

### 3、使用 rosserial

#### 3.1 安装 ROS

首先需要一个比较强劲的CPU用来运行 ROS 的主节点，例如 PC 或者 ARM ，安装过程可以参照 ROS 的[官网](http://wiki.ros.org/ROS/Installation)，现在官方支持最好的还是 Debian 系列。

下面以 Ubuntu 安装 ROS Melodic 为例：

依赖：

	sudo apt install python-rosinstall python-rosinstall-generator python-wstool build-essential

安装：  

	sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
	sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116
	sudo apt update
	sudo apt install ros-melodic-desktop-full

初始化： 

	sudo rosdep init
	rosdep update
	echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc
	source ~/.bashrc

#### 3.2 启动 ROS

首先启动一个主节点：

	roscore

#### 3.3 建立连接

如果使用的串口：

	rosrun rosserial_python serial_node.py /dev/ttyUSB0

如果使用的 TCP：

	rosrun rosserial_python serial_node.py tcp

#### 3.4 订阅话题

以 hello world 为例：

	rostopic echo /chatter

## 4、注意事项

暂无

## 5、感谢

- rosserial: https://github.com/ros-drivers/rosserial

## 6、联系方式

* 维护：Wu Han
* 主页：http://wuhanstudio.cc
* 邮箱：wuhanstudio@hust.edu.cn
