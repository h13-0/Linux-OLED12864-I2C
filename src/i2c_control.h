#pragma once

int i_open(const char* dev, unsigned int timeout, unsigned int retry);

/*
 * fd       : �ļ�������
 * timeout  : ���ͳ�ʱʱ��
 * retry    : �ظ����ʹ���
 */
 //�ظ����ʹ����������һ�㣬�ڵ��Ե�ʱ��ֻ������һ�Σ�������ʱ���ͻ�ʧ�ܡ�
int i2c_set(int fd, unsigned int timeout, unsigned int retry);

/*
 * fd   : �ļ�������
 * addr : i2c���豸��ַ
 * reg  : �Ĵ�����ַ
 * val  : Ҫд������
 * ���� ����ָ����ַд����
 */
int i2c_byte_write(int fd, unsigned char addr, unsigned char reg, unsigned char val);

/*
 * fd   : �ļ�������
 * addr : i2c���豸��ַ
 * reg  : �Ĵ�����ַ
 * val  : Ҫд������
 * len  : ���ݳ���
 * ���� ����ָ����ַд����
 *        24c02��8�ֽ�Ϊ1��page�������һ��page����д��д���ֽڳ��ȳ������page��ĩβ��
 *        �ͻ��page�Ŀ�ͷд,���ǿ�ͷ������
 */
int i2c_nbytes_write(int fd, unsigned char addr, unsigned char reg, unsigned char* val, int len);

/*
 * fd   : �ļ�������
 * addr : i2c���豸��ַ
 * val  : �����ȡ����
 * ���� ���ӵ�ǰ��ַ��ȡһ���ֽ�����
 */
int i2c_byte_read(int fd, unsigned char addr, unsigned char* val);

/*
 * fd   : �ļ�������
 * addr : i2c���豸��ַ
 * reg  : �Ĵ�����ַ
 * val  : �����ȡ������
 * len  : ��ȡ���ݵĳ���
 * ���� ����ȡ�ﵽeeprom��ĩβʱ�����ȡ�ͷ���ֽ�
 */
int i2c_nbytes_read(int fd, unsigned char addr, unsigned char reg, unsigned char* val, int len);
