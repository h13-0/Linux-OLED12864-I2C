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
 * fd       : �ļ�������
 * timeout  : ���ͳ�ʱʱ��
 * retry    : �ظ����ʹ���
 */
 //�ظ����ʹ����������һ�㣬�ڵ��Ե�ʱ��ֻ������һ�Σ�������ʱ���ͻ�ʧ�ܡ�
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
 * fd   : �ļ�������
 * addr : i2c���豸��ַ
 * reg  : �Ĵ�����ַ
 * val  : Ҫд������
 * ���� ����ָ����ַд����
 */
int i2c_byte_write(int fd, unsigned char addr, unsigned char reg, unsigned char val)
{
    int ret = 0;
    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages;

    packets.nmsgs = 1;
    packets.msgs = &messages;

    //����Ҫ��ȡ�ļĴ�����ַ
    messages.addr = addr;
    messages.flags = 0;
    messages.len = 2;       //�Ĵ�����ַ�����ݣ�������2���ֽ�
    messages.buf = outbuf;
    outbuf[0] = reg;
    outbuf[1] = val;
    
    ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets);   //������
    if (ret < 0)
        ret = -1;
    
    return ret;
}

/*
 * fd   : �ļ�������
 * add  r : i2c���豸��ַ
 * reg  : �Ĵ�����ַ
 * val  : Ҫд������
 * len  : ���ݳ���
 * ���� ����ָ����ַд����
 *        24c02��8�ֽ�Ϊ1��page�������һ��page����д��д���ֽڳ��ȳ������page��ĩβ��
 *        �ͻ��page�Ŀ�ͷд,���ǿ�ͷ������
 */
int i2c_nbytes_write(int fd, unsigned char addr, unsigned char reg, unsigned char* val, int len)
{
    int ret = 0;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages;
    int i;

    packets.nmsgs = 1;
    packets.msgs = &messages;

    //����Ҫ��ȡ�ļĴ�����ַ
    messages.addr = addr;
    messages.flags = 0;         //write
    messages.len = (unsigned short)(len + 1);     //���ݳ���
    //��������
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
    
    ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets);//������
    if (ret < 0) {
        printf("write error!\n");
        return -1;
    }
    

err:
    free(messages.buf);

    return ret;

}

/*
 * fd   : �ļ�������
 * addr : i2c���豸��ַ
 * val  : �����ȡ����
 * ���� ���ӵ�ǰ��ַ��ȡһ���ֽ�����
 */
int i2c_byte_read(int fd, unsigned char addr, unsigned char* val)
{
    int ret = 0;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages;

    packets.nmsgs = 1;              //����֡����ֻ��һ�֣�������,ֻ��Ҫ����һ����ʼ�źţ������1
    packets.msgs = &messages;

    //����Ҫ��ȡ�ļĴ�����ַ
    messages.addr = addr;           //i2c�豸��ַ
    messages.flags = I2C_M_RD;      //������
    messages.len = 1;               //���ݳ���
    messages.buf = val;             //��ȡ�����ݱ�����val

    ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets);  //��������֡
    if (ret < 0)
        ret = -1;

    return ret;
}

/*
 * fd   : �ļ�������
 * addr : i2c���豸��ַ
 * reg  : �Ĵ�����ַ
 * val  : �����ȡ������
 * len  : ��ȡ���ݵĳ���
 * ���� ����ȡ�ﵽeeprom��ĩβʱ�����ȡ�ͷ���ֽ�
 */
int i2c_nbytes_read(int fd, unsigned char addr, unsigned char reg, unsigned char* val, int len)
{
    int ret = 0;
    unsigned char outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    /* ����֡������2��
     * дҪ������ʼ�źţ�����д�Ĵ����������ٷ�����ʼ�ź�,���ж�����,
     * ��2����ʼ�źţ������Ҫ�ֿ���������
     */
    packets.nmsgs = 2;
    //����Ҫ��ȡ�ļĴ�����ַ
    messages[0].addr = addr;
    messages[0].flags = 0;              //write
    messages[0].len = 1;                //���ݳ���
    messages[0].buf = &outbuf;          //���ͼĴ�����ַ
    outbuf = reg;
    //��ȡ����
    messages[1].len = (unsigned short)len;                           //��ȡ���ݳ���
    messages[1].addr = addr;                         //�豸��ַ
    messages[1].flags = I2C_M_RD;                    //read
    messages[1].buf = val;

    packets.msgs = messages;

    ret = ioctl(fd, I2C_RDWR, (unsigned long)&packets); //����i2c,���ж�ȡ���� 
    if (ret < 0)
        ret = -1;

    return ret;
}