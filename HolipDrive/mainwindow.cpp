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


quint32 dwBaud[SERIAL_MAX_BAUD_CNT] = {QSerialPort::Baud2400,QSerialPort::Baud4800,QSerialPort::Baud9600,QSerialPort::Baud19200,QSerialPort::Baud38400,QSerialPort::Baud57600,QSerialPort::Baud115200};
quint8 byAddr[SERIAL_MAX_ADDR_CNT] = {1,2,3,4,5,6,7,8};


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

    while(byStepAddr<SERIAL_MAX_ADDR_CNT)
    {
        while(byStepBaud<SERIAL_MAX_BAUD_CNT)
        {
            while(byStopBit<SERIAL_STOP_BIT_CNT)
            {
                SerialSettings.baudRate =  dwBaud[byStepBaud];
                DriveType.byInverterAddr = byAddr[byStepAddr];
                SerialSettings.stringBaudRate = QString::number(SerialSettings.baudRate);
                mainSerial->setPortName(SerialSettings.name);
                mainSerial->setBaudRate(SerialSettings.baudRate);

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
                byStopBit++;
                if(byStopBit>=SERIAL_STOP_BIT_CNT)
                {
                    byStopBit = 0;
                    byStepBaud++;
                    if(byStepBaud>=SERIAL_MAX_BAUD_CNT)
                    {
                        byStepBaud = 0;
                        byStepAddr++;
                    }
                }
                return;
            }
        }

    }

    //if run here , it means the whole loop is finish and do not find the inverter


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
}

void MainWindow::vInitSignalConnect()
{
    //链接通信串口接收数据
    connect(mainSerial, SIGNAL(readyRead()), this, SLOT(readData()));
}

void MainWindow::vInitNewObject()
{
    mainSerial = new QSerialPort(this);
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

}
