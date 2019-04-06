# rosserial 移植

移植可以参照官网说明：

	http://wiki.ros.org/rosserial_client/Tutorials/Adding%20Support%20for%20New%20Hardware

主要也就是实现这几个函数：

	class Hardware
	{
	
	  Hardware();
	
	  // any initialization code necessary to use the serial port
	  void init(); 
	
	  // read a byte from the serial port. -1 = failure
	  int read()
	
	  // write data to the connection to ROS
	  void write(uint8_t* data, int length);
	
	  // returns milliseconds since start of program
	  unsigned long time();
	
	};