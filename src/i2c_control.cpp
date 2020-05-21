#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define I2C_DEFAULT_TIMEOUT     3
#define I2C_DEFAULT_RETRY       3

//#define TIMEOUT	3
//#define RETRY	3

int i_open(const char* dev, unsigned int timeout, unsigned int retry)
{
    return open(dev, timeout, retry);
}


/*
 * fd       : 文件描述符
 * timeout  : 发送超时时间
 * retry    : 重复发送次数
 */
 //重复发送次数可以设多一点，在调试的时候，只设置了一次，导致有时候发送会失败。
int i2c_set(int fd, unsigned int timeout, unsigned int retry)
{
    if (fd == 0)
        return -1;

    if (ioctl(fd, I2C_TIMEOUT, timeout ? timeout : I2C_DEFAULT_TIMEOUT) < 0)
        return -1;
    if (ioctl(fd, I2C_RETRIES, retry ? retry : I2C_DEFAULT_RETRY) < 0)
        return -1;

    return 0;
}
/*
 * fd   : 文件描述符
 * addr : i2c的设备地址
 * reg  : 寄存器地址
 * val  : 要写的数据
 * 描述 ：从指定地址写数据
 */
int i2c_byte_write(int fd, unsigned char addr, unsigned char reg, unsigned char val)
{
    int ret = 0;
    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages;

    packets.nmsgs = 1;
    packets.msgs = &messages;

    //发送要读取的寄存器地址
    messages.addr = addr;
    messages.flags = 0;
    messages.len = 2;       //寄存器地址加数据，共发送2个字节
    messages.buf = outbuf;
    outbuf[0] = reg;
    outbuf[1] = val;
    
    ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets);   //读出来
    if (ret < 0)
        ret = -1;
    
    return ret;
}

/*
 * fd   : 文件描述符
 * add  r : i2c的设备地址
 * reg  : 寄存器地址
 * val  : 要写的数据
 * len  : 数据长度
 * 描述 ：从指定地址写数据
 *        24c02以8字节为1个page，如果在一个page里面写，写的字节长度超过这个page的末尾，
 *        就会从page的开头写,覆盖开头的内容
 */
int i2c_nbytes_write(int fd, unsigned char addr, unsigned char reg, unsigned char* val, int len)
{
    int ret = 0;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages;
    int i;

    packets.nmsgs = 1;
    packets.msgs = &messages;

    //发送要读取的寄存器地址
    messages.addr = addr;
    messages.flags = 0;         //write
    messages.len = (unsigned short)(len + 1);     //数据长度
    //发送数据
    messages.buf = (unsigned char*)malloc(len + 1);
    if (NULL == messages.buf)
    {
        ret = -1;
        goto err;
    }

    messages.buf[0] = reg;
    for (i = 0; i < len; i++)
    {
        messages.buf[1 + i] = val[i];
    }
    
    ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets);//读出来
    if (ret < 0) {
        printf("write error!\n");
        return -1;
    }
    

err:
    free(messages.buf);

    return ret;

}

/*
 * fd   : 文件描述符
 * addr : i2c的设备地址
 * val  : 保存读取数据
 * 描述 ：从当前地址读取一个字节数据
 */
int i2c_byte_read(int fd, unsigned char addr, unsigned char* val)
{
    int ret = 0;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages;

    packets.nmsgs = 1;              //数据帧类型只有一种，读操作,只需要发送一个起始信号，因此是1
    packets.msgs = &messages;

    //发送要读取的寄存器地址
    messages.addr = addr;           //i2c设备地址
    messages.flags = I2C_M_RD;      //读操作
    messages.len = 1;               //数据长度
    messages.buf = val;             //读取的数据保存在val

    ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets);  //发送数据帧
    if (ret < 0)
        ret = -1;

    return ret;
}

/*
 * fd   : 文件描述符
 * addr : i2c的设备地址
 * reg  : 寄存器地址
 * val  : 保存读取的数据
 * len  : 读取数据的长度
 * 描述 ：读取达到eeprom的末尾时，会读取最开头的字节
 */
int i2c_nbytes_read(int fd, unsigned char addr, unsigned char reg, unsigned char* val, int len)
{
    int ret = 0;
    unsigned char outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    /* 数据帧类型有2种
     * 写要发送起始信号，进行写寄存器操作，再发送起始信号,进行读操作,
     * 有2个起始信号，因此需要分开来操作。
     */
    packets.nmsgs = 2;
    //发送要读取的寄存器地址
    messages[0].addr = addr;
    messages[0].flags = 0;              //write
    messages[0].len = 1;                //数据长度
    messages[0].buf = &outbuf;          //发送寄存器地址
    outbuf = reg;
    //读取数据
    messages[1].len = (unsigned short)len;                           //读取数据长度
    messages[1].addr = addr;                         //设备地址
    messages[1].flags = I2C_M_RD;                    //read
    messages[1].buf = val;

    packets.msgs = messages;

    ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets); //发送i2c,进行读取操作 
    if (ret < 0)
        ret = -1;

    return ret;
}