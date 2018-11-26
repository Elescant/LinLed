#include "Lin.h"
#include "QTime"
#include <QDebug>
#include <QThread>
#include <sys/time.h>

Lin::Lin(QSerialPort *port)
{
    serial = port;
}

bool Lin::sendMsgFrame(uint8_t command, uint8_t ack, uint8_t *dat, int num,char *buff)
{
    uint8_t cs;
    int i;

    buff[0] = MSG_HEAD;
    buff[1] = (num + 1);
    buff[2] = (num + 1) >> 8;
    buff[3] = command;
    if(ack)
    {
        buff[3] |= 0x40;
    }
    memcpy(buff+4,dat,num);
    cs = 0;
    for(i = 0;i < num + 4;i++)
    {
        cs +=buff[i];
    }
    buff[i++] = cs;
    buff[i++] = MSG_TAIL;
    if(NULL != serial)
    {
        if(serial->write(buff,i) == i)
        {
            return true;
        }
    }
    return false;
}

Lin::lin_err Lin::getMsgFrame(char *buff, int timeout_ms)
{
    static QByteArray arr;
    unsigned char c;
    int pos;
    static int length;
    static uint8_t writep = 0;
    uint8_t csum;
    static uint8_t state = U2LIN_STATE_INIT;
    int ret;
    int msec = clock();

    while(1)
    {
        qDebug()<<"bytes:"<<serial->bytesAvailable();
        ret = serial->read((char *)&c,1);
        if(ret == 1)
        {
           qDebug()<<"receive:" << c;
           msec = clock();
           switch(state)
           {
           case U2LIN_STATE_INIT:
               if(c == MSG_HEAD)
               {
                   qDebug()<<"head";
                   state = U2LIN_STATE_LL;
               }
               break;
           case U2LIN_STATE_LL:
               length = c;
               csum = 0x68 + c;
               state = U2LIN_STATE_LH;
               break;
           case U2LIN_STATE_LH:
               length += c<<8;
               qDebug()<<"length:"<<length;
               csum +=c;
               writep = 0;
               state = U2LIN_STATE_DAT;
               break;
           case U2LIN_STATE_DAT:
               buff[writep++] = c;
               csum +=c;
               if(writep == length)
               {
                   qDebug()<<"data receive";
                   state = U2LIN_STATE_CS;
               }
               break;
           case U2LIN_STATE_CS:
               if(csum == c)
               {
                   qDebug()<<"check ok";
                   state = U2LIN_STATE_END;
               }else
               {
                   qDebug()<<"check failed";
                   state = U2LIN_STATE_INIT;
               }
               break;
           case U2LIN_STATE_END:
               state = U2LIN_STATE_INIT;
               if(c == MSG_TAIL)
               {
                   qDebug()<<"return ok";
                   serial->clear(QSerialPort::Input);
                   return ERR_OK;
               }
               qDebug()<<"failed";
               break;
           default:
               state = U2LIN_STATE_INIT;
               break;
           }
        }else
        {
            qDebug()<<"sleep";
            QThread::msleep(1);
        }
        if(clock() - msec > timeout_ms)
        {
            qDebug()<<"time out";
            serial->clear(QSerialPort::Input);
            return ERR_TIMEOUT;
        }
    }



}

bool Lin::testlink()
{
    uint8_t dat[1] ={'l'};
    char buff[7]={0};
    char re_buf[128]={0};

    if(sendMsgFrame(U2LIN_COM_LINK,1,dat,1,buff))
    {
        if(getMsgFrame(re_buf,1000) == ERR_OK)
        {
            if((re_buf[0]&0x3F) == U2LIN_COM_LINK && re_buf[1] == 'U'
                    && re_buf[2] == '2' )//&& re_buf[3] =='L')
            {
                return true;
            }
        }
    }
    return false;
}


