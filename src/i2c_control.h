#pragma once

int i_open(const char* dev, unsigned int timeout, unsigned int retry);

/*
 * fd       : 文件描述符
 * timeout  : 发送超时时间
 * retry    : 重复发送次数
 */
 //重复发送次数可以设多一点，在调试的时候，只设置了一次，导致有时候发送会失败。
int i2c_set(int fd, unsigned int timeout, unsigned int retry);

/*
 * fd   : 文件描述符
 * addr : i2c的设备地址
 * reg  : 寄存器地址
 * val  : 要写的数据
 * 描述 ：从指定地址写数据
 */
int i2c_byte_write(int fd, unsigned char addr, unsigned char reg, unsigned char val);

/*
 * fd   : 文件描述符
 * addr : i2c的设备地址
 * reg  : 寄存器地址
 * val  : 要写的数据
 * len  : 数据长度
 * 描述 ：从指定地址写数据
 *        24c02以8字节为1个page，如果在一个page里面写，写的字节长度超过这个page的末尾，
 *        就会从page的开头写,覆盖开头的内容
 */
int i2c_nbytes_write(int fd, unsigned char addr, unsigned char reg, unsigned char* val, int len);

/*
 * fd   : 文件描述符
 * addr : i2c的设备地址
 * val  : 保存读取数据
 * 描述 ：从当前地址读取一个字节数据
 */
int i2c_byte_read(int fd, unsigned char addr, unsigned char* val);

/*
 * fd   : 文件描述符
 * addr : i2c的设备地址
 * reg  : 寄存器地址
 * val  : 保存读取的数据
 * len  : 读取数据的长度
 * 描述 ：读取达到eeprom的末尾时，会读取最开头的字节
 */
int i2c_nbytes_read(int fd, unsigned char addr, unsigned char reg, unsigned char* val, int len);
