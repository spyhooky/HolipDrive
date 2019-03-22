#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QDialog>
#include "Holipheader.h"

#define SERIAL_MAX_BAUD_CNT     7
#define SERIAL_MAX_ADDR_CNT     8

#define SERIAL_STOP_BIT_CNT     4

#define	MAIN_SERIAL_FRM_LEN		256

#define STD_FC_HEADER           0x02
#define STD_FC_DATA_INDEX_STX   0
#define STD_FC_DATA_INDEX_LGE   1
#define STD_FC_DATA_INDEX_ADD   2
#define STD_FC_DATA_INDEX_PNU   3
#define STD_FC_DATA_INDEX_INX   5
#define STD_FC_DATA_INDEX_DATA  7
#define STD_FC_DATA_INDEX_CTW   11
#define STD_FC_DATA_INDEX_REF   13
#define STD_FC_DATA_INDEX_END   15

typedef	struct {
    QString name;
    qint32 baudRate;
    QString stringBaudRate;
    QSerialPort::DataBits dataLen;
    QString stringDataLen;
    QSerialPort::Parity check;
    QString stringCheck;
    QSerialPort::StopBits stopBits;
    QString stringStopBits;
}CURSETS;

typedef	struct {
    qint8 byDeviceType;
    qint8 byInverterAddr;
}DEVICESETS;

typedef	struct {
    UNSIGNED8 data[MAIN_SERIAL_FRM_LEN];
    int  len;
} main_serial_packet_t;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    UNSIGNED16 wSerialSend(char * str , qint64 len);
    int iPrepareStdFCPacket(UNSIGNED16 wPNU, UNSIGNED8 byIx , UNSIGNED8 byAK , UNSIGNED32 dwValue);

private slots:
    void on_pushButtonFoundInverter_clicked();

    void on_pushButtonFoundInverterStop_clicked();
    void readData();

private:
    Ui::MainWindow *ui;
    void fillPortsInfo();
    void updateSettings();
    void vInitMainFram();
    void vFillDeviceTypeInfo();
    void vUpdateFramSelect();
    void vEnableSelect(bool bFlag);
    void vFindInverterProcess();
    void vInitData();
    void vInitLoopData();
    void vInitSignalConnect();
    void vInitNewObject();

private:
    CURSETS SerialSettings;
    DEVICESETS DriveType;
    QSerialPort *mainSerial;
    bool mainCommConnect;
    UNSIGNED8 byStepAddr;
    UNSIGNED8 byStepBaud;
    UNSIGNED8 byStopBit;
    main_serial_packet_t mainSendPkt;
};

#endif // MAINWINDOW_H
