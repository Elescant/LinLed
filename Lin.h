#ifndef LIN_H
#define LIN_H

#include <QObject>
#include <QSerialPort>

class Lin : public QObject
{
    Q_OBJECT
public:
    explicit Lin(QSerialPort *port);

    enum {MSG_HEAD = 0x68,MSG_TAIL = 0x16,MSG_LENTH = 10};
    enum {U2LIN_COM_LINK=1};
    enum {U2LIN_STATE_INIT=0,U2LIN_STATE_LL,U2LIN_STATE_LH,U2LIN_STATE_DAT,U2LIN_STATE_CS,U2LIN_STATE_END};
    typedef enum {ERR_OK = 0,ERR_EMPTY,ERR_WAIT,ERR_TIMEOUT,ERR_FRAME} lin_err;

    bool sendMsgFrame(uint8_t command, uint8_t ack, uint8_t *dat, int num, char *buff);
    lin_err getMsgFrame(char *buff, int timeout_ms);
    bool testlink(void);
signals:

public slots:
private:
    QSerialPort *serial;
};

#endif // LIN_H
