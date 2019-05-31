#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>
#include <QString>
#include <QThread>
#include "Holipheader.h"
#include "api/myhelper.h"
#include "api/myapp.h"

extern "C" {
#include "Sharebits.h"
#include "Crc8.h"
}


static quint32 dwBaud[INVERTER_FOUND_TM_MAX] = {
    QSerialPort::Baud9600,QSerialPort::Baud9600,QSerialPort::Baud38400,
    QSerialPort::Baud115200,QSerialPort::Baud115200,QSerialPort::Baud115200
};
static E_FOUND_INVERTER_PROCOTOL eDetectInverterProtocol[INVERTER_FOUND_TM_MAX] = {
    INVERTER_PROCOTOL_FC,
    INVERTER_PROCOTOL_MODBUS,
    INVERTER_PROCOTOL_TM,
    INVERTER_PROCOTOL_FC,
    INVERTER_PROCOTOL_MODBUS,
    INVERTER_PROCOTOL_TM
};
//quint8 byAddr[INVERTER_FOUND_TM_MAX] = {1,2,3,4,5,6,7,8};
static UNSIGNED8 byFeelInverterPacketFC[] = {0x01};
static UNSIGNED8 byFeelInverterPacketMODBUS[] = {0x01};
static UNSIGNED8 byFeelInverterPacketTM[] = {0x01};

static UNSIGNED8 byMsgBuff[MAIN_SERIAL_FRM_LEN] = {0};
static SIGNED8 cBuffRx[MAIN_SERIAL_FRM_LEN]={0};
static UNSIGNED8 byBuffRxLen=0;
static SIGNED8 * pcBuffRxPos=NULL;
static SIGNED8 cComParseBuff[MAIN_SERIAL_FRM_LEN]={0};
static UNSIGNED8 byComParseLen=0;

/*
 *  FC->
 * 3887=1:      02 0e 3f 2f 2f 00 00 00 00 00 01 04 3c 00 00 0a
 * 3886=11536:  02 0e 3f 2f 2e 00 00 00 00 2d 10 04 3c 00 00 37
 * 3886=10512:  02 0e 3f 2f 2e 00 00 00 00 29 10 04 3c 00 00 33
 * 1429=6111:   02 0e 3f 25 95 00 00 00 00 17 df 04 3c 00 00 73
 * 1428=1:      02 0e 3f 25 94 00 00 00 00 00 01 04 3c 00 00 bb
 *
 *  MODBUS->
 * 3887=1:      01 06 97 d5 00 01 75 86
 * 3886=11536:  01 06 97 cb 2d 10 C8 DC
 * 3886=10512:  01 06 97 cb 29 10 CA 1C
 * 1429=6111:   01 06 37 d1 17 df 99 EF
 * 1428=1:      01 06 37 c7 00 01 F7 83
*/

static SIGNED8 cPacket_3887_1_FC[] = {0x02 ,0x0e ,0x3f ,0x2f ,0x2f ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x04 ,0x3c ,0x00 ,0x00 ,0x0a};
static SIGNED8 cpacket_3886_11536_FC[] = {02 0e 3f 2f 2e 00 00 00 00 2d 10 04 3c 00 00 37};
static SIGNED8 cpacket_3886_10512_FC[] = {02 0e 3f 2f 2e 00 00 00 00 29 10 04 3c 00 00 33};
static SIGNED8 cpacket_1429_FC[] = {02 0e 3f 25 95 00 00 00 00 17 df 04 3c 00 00 73};
static SIGNED8 cpacket_1428_FC[] = {02 0e 3f 25 94 00 00 00 00 00 01 04 3c 00 00 bb};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    //start init fram
    vInitMainFram();

    //start init data
    vInitData();

}

MainWindow::~MainWindow()
{
    delete ui;
}

UNSIGNED16 MainWindow::wSerialSend(char *str, qint64 len)
{
    int ret = -1;

    if(!mainCommConnect){
        return ret;
    }

    ret = mainSerial->write(str,len);
    mainSerial->waitForBytesWritten(100);

//#ifdef  DEBUG_MAIN_COMM_SEND
//    qDebug("main Comm send : %s ; len : %d",str,len);
//#endif
    return ret;
}

int MainWindow::iPrepareStdFCPacket(UNSIGNED16 wPNU, UNSIGNED8 byIx, UNSIGNED8 byAK , UNSIGNED32 dwValue)
{
    UNSIGNED16 wTmp = 0;

    mainSendPkt.data[STD_FC_DATA_INDEX_STX] = STD_FC_HEADER;
    mainSendPkt.data[STD_FC_DATA_INDEX_LGE] = STD_FC_DATA_INDEX_END-1;
    mainSendPkt.data[STD_FC_DATA_INDEX_ADD] = byStepAddr;
    wTmp = byAK<<12 | wPNU;
    putleu16(mainSendPkt.data,STD_FC_DATA_INDEX_PNU,wTmp);
    wTmp = byIx;
    putleu16(mainSendPkt.data,STD_FC_DATA_INDEX_INX,wTmp);
    putleu32(mainSendPkt.data,STD_FC_DATA_INDEX_DATA,dwValue);
    wTmp = 0;
    putleu16(mainSendPkt.data,STD_FC_DATA_INDEX_CTW,wTmp);
    wTmp = 0;
    putleu16(mainSendPkt.data,STD_FC_DATA_INDEX_REF,wTmp);
    mainSendPkt.data[STD_FC_DATA_INDEX_END] = Crc8_byCalCrc8(mainSendPkt.data,STD_FC_DATA_INDEX_END,0x55);
    mainSendPkt.len = STD_FC_DATA_INDEX_END + 1;
    return mainSendPkt.data[STD_FC_DATA_INDEX_LGE];
}

void MainWindow::fillPortsInfo()
{
    ui->comboBoxSerialNum->clear();
    static const QString blankString = QObject::tr("N/A");
    QString description;
    QString manufacturer;
    QString serialNumber;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->comboBoxSerialNum->addItem(list.first(), list);
    }
}

void MainWindow::updateSettings()
{

}

void MainWindow::vInitMainFram()
{
    fillPortsInfo();
    vFillDeviceTypeInfo();
}

void MainWindow::vFillDeviceTypeInfo()
{
    ui->comboBoxDeviceType->addItem("A100", 1);
    ui->comboBoxDeviceType->addItem("A150", 2);
}

void MainWindow::vUpdateFramSelect()
{
    SerialSettings.name = ui->comboBoxSerialNum->currentText();
    DriveType.byDeviceType = ui->comboBoxDeviceType->currentIndex();
}

void MainWindow::vEnableSelect(bool bFlag)
{
    if(!bFlag)
    {
        ui->comboBoxDeviceType->setDisabled(true);
        ui->comboBoxSerialNum->setDisabled(true);
    }
    else
    {
        ui->comboBoxDeviceType->setEnabled(true);
        ui->comboBoxSerialNum->setEnabled(true);
    }
}

/**********************************************************************
 * Description:
 *
 * Input:
 * Output:
 * Auther: HYQ
 **********************************************************************/
void MainWindow::vFindInverterProcess()
{
    QString show;

    mainSerial->setDataBits(QSerialPort::Data8);

    /*
     *
     *loop stopbit
     *
     *
    */
    switch(byStopBit)
    {
        case 0:
        SerialSettings.stringCheck = "Even";
        SerialSettings.stringStopBits = "1";
        mainSerial->setStopBits(QSerialPort::OneStop);
        mainSerial->setParity(QSerialPort::EvenParity);
        break;

        case 1:
        SerialSettings.stringCheck = "Odd";
        SerialSettings.stringStopBits = "1";
        mainSerial->setStopBits(QSerialPort::OneStop);
        mainSerial->setParity(QSerialPort::OddParity);
        break;

        case 2:
        SerialSettings.stringCheck = "None";
        SerialSettings.stringStopBits = "1";
        mainSerial->setStopBits(QSerialPort::OneStop);
        mainSerial->setParity(QSerialPort::NoParity);
        break;

        case 3:
        SerialSettings.stringCheck = "None";
        SerialSettings.stringStopBits = "2";
        mainSerial->setStopBits(QSerialPort::TwoStop);
        mainSerial->setParity(QSerialPort::NoParity);
        break;

        default:
        break;

    }
    /*
     *
     *loop baud and procotol
     *
     *
    */
    switch(eInverterDetectStep)
    {
        case INVERTER_FOUND_FC_9600:
            SerialSettings.baudRate = dwBaud[eInverterDetectStep];
            DriveType.byInverterAddr = 1;
            SerialSettings.stringBaudRate = QString::number(SerialSettings.baudRate);
            mainSerial->setPortName(SerialSettings.name);
            mainSerial->setBaudRate(SerialSettings.baudRate);
        break;

        case INVERTER_FOUND_FC_115200:
        break;

        default:break;

    }
    sMyInverterDetectState.eDetectProtocolStep = eDetectInverterProtocol[eInverterDetectStep];
    /*
     *
     * start serial com
     *
     *
    */
    if(!mainCommConnect)
    {
        if (mainSerial->open(QIODevice::ReadWrite))
        {
            show.append(SerialSettings.name);
            show.append("已连接");
            show.append(",");
            show.append(SerialSettings.stringBaudRate);
            show.append(",");
            show.append(SerialSettings.stringDataLen);
            show.append(",");
            show.append(SerialSettings.stringCheck);
            show.append(",");
            show.append(SerialSettings.stringStopBits);
            mainCommConnect = true;
            this->wSerialSend((SIGNED8*)byFeelInverterPacketTM,sizeof(byFeelInverterPacketTM));
            TickTimer->start(1000);//this scale is ms
        }
        else
        {
            mainCommConnect = false;
            show.append(SerialSettings.name);
            show.append(" 打开失败");
            myHelper::ShowMessageBoxError("打开串口失败！");
        }

    }
    else
    {
        show.append(SerialSettings.name);
        show.append("已经打开了");
    }
    ui->textBrowserResponse->append(show);


}

void MainWindow::vInitData()
{
    //data init
    mainCommConnect = false;
    SerialSettings.stringDataLen = "8";
    vInitLoopData();
    //object init
    vInitNewObject();
    //signal and slot
    vInitSignalConnect();

}

void MainWindow::vInitLoopData()
{
    byStepAddr = 0;
    byStepBaud = 0;
    byStopBit = 0;
    eInverterDetectStep = INVERTER_FOUND_FC_9600;
    sMyInverterDetectState.eDetectProtocolStep = INVERTER_PROCOTOL_FC;
    sMyInverterDetectState.bDetectInverterFinish = false;
}

void MainWindow::vInitSignalConnect()
{
    //链接通信串口接收数据
    connect(mainSerial, SIGNAL(readyRead()), this, SLOT(readData()));
    //
    connect(TickTimer, SIGNAL(timeout()), this, SLOT(vParseDetectPacket()));
}

void MainWindow::vInitNewObject()
{
    mainSerial = new QSerialPort(this);
    TickTimer = new QTimer(this);
}

/**********************************************************************
 * Description:
 *
 *
 *  typedef enum
    {
       INVERTER_FOUND_FC_9600          = 0,
       INVERTER_FOUND_MODBUS_9600      = 1,
       INVERTER_FOUND_TM_38400         = 2,
       INVERTER_FOUND_FC_115200        = 3,
       INVERTER_FOUND_MODBUS_115200    = 4,
       INVERTER_FOUND_TM_115200        = 5,
       INVERTER_FOUND_TM_MAX           = 6
    }E_FOUND_INVERTER_STEP;
 *
 *  FC->
 * 3887=1:      02 0e 3f 2f 2f 00 00 00 00 00 01 04 3c 00 00 0a
 * 3886=11536:  02 0e 3f 2f 2e 00 00 00 00 2d 10 04 3c 00 00 37
 * 3886=10512:  02 0e 3f 2f 2e 00 00 00 00 29 10 04 3c 00 00 33
 * 1429=6111:   02 0e 3f 25 95 00 00 00 00 17 df 04 3c 00 00 73
 * 1428=1:      02 0e 3f 25 94 00 00 00 00 00 01 04 3c 00 00 bb
 *
 *  MODBUS->
 * 3887=1:      01 06 97 d5 00 01 75 86
 * 3886=11536:  01 06 97 cb 2d 10 C8 DC
 * 3886=10512:  01 06 97 cb 29 10 CA 1C
 * 1429=6111:   01 06 37 d1 17 df 99 EF
 * 1428=1:      01 06 37 c7 00 01 F7 83
 *
 * Input:
 * Output:
 * Auther: HYQ
 **********************************************************************/
void MainWindow::vSetInverter2TM()
{
    if(!sMyInverterDetectState.bDetectInverterFinish)
    {
        QString show;
        show.append("变频器未找到，请先寻找变频器！");
        ui->textBrowserResponse->append(show);
    }
    if((INVERTER_FOUND_FC_9600 == eInverterDetectStep) ||
       (INVERTER_FOUND_FC_115200 == eInverterDetectStep))
    {
        /*
         *(1)3887=1
         *(2)3886=11536 or 10512
         *(3)1429=6111
         *(4)1428=1
        */
    }
    else if((INVERTER_FOUND_MODBUS_9600 == eInverterDetectStep)||
            (INVERTER_FOUND_MODBUS_115200 == eInverterDetectStep))
    {
        /*
         *(1)3887=1
         *(2)3886=11536 or 10512
         *(3)1429=6111
         *(4)1428=1
        */
    }
}

void MainWindow::on_pushButtonFoundInverter_clicked()
{
    ui->pushButtonFoundInverter->setDisabled(true);
    vUpdateFramSelect();
    vEnableSelect(false);
    vFindInverterProcess();
}

void MainWindow::on_pushButtonFoundInverterStop_clicked()
{
    QString show;

    mainSerial->close();
    show.append(SerialSettings.name);
    show.append("断开连接");
    mainCommConnect = false;
    ui->pushButtonFoundInverter->setEnabled(true);
    ui->comboBoxDeviceType->setEnabled(true);
    ui->comboBoxSerialNum->setEnabled(true);
    ui->textBrowserResponse->append(show);
}

void MainWindow::readData()
{
    QString show;

    if(mainCommConnect)
    {
        memset(byMsgBuff,0,sizeof(byMsgBuff));

        QByteArray todRcvBuf = mainSerial->readAll();
        show = todRcvBuf.data();
        ui->textBrowserResponse->append(show);

        if(todRcvBuf.isEmpty())
        {
            return;
        }
        RxArrayBuff.clear();
        pcBuffRxPos = cBuffRx;
        memset(cBuffRx,0,MAIN_SERIAL_FRM_LEN);
        RxArrayBuff += todRcvBuf;

        pcBuffRxPos = RxArrayBuff.data();
        byBuffRxLen = RxArrayBuff.length();
        /*
        *
        *fill the serial packet
        *
        */
        if(byBuffRxLen>0){
            if(byBuffRxLen + byComParseLen > MAIN_SERIAL_FRM_LEN){
                byComParseLen = 0;
                memset(cComParseBuff,0,sizeof(cComParseBuff));
            }
            memcpy(cComParseBuff+byBuffRxLen,pcBuffRxPos,byBuffRxLen);
            byBuffRxLen += byBuffRxLen;
        }
    }
}

void MainWindow::vParseDetectPacket()
{
    TickTimer->stop();
    /*
     *
     *parse the reponse packet with FC or MODBUS or TM CMD , then set mode which is in
     *
     *
    */



    /*
     *
     *if parse packet succeed set  sMyInverterDetectState.bDetectInverterFinish = true
     *
     *
     */
    QString show;
    show.append("已经找到变频器");

    /*
     *
     *if false == sMyInverterDetectState.bDetectInverterFinish , then  eInverterDetectStep++ ,  and go on detect inverter
     *
     *
    */
}
