/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QComboBox *comboBoxSerialNum;
    QComboBox *comboBoxDeviceType;
    QLabel *labelSerialNum;
    QLabel *labelDeviceType;
    QPushButton *pushButtonFoundInverter;
    QTextBrowser *textBrowserResponse;
    QPushButton *pushButtonFoundInverterStop;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1278, 638);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        comboBoxSerialNum = new QComboBox(centralWidget);
        comboBoxSerialNum->setObjectName(QStringLiteral("comboBoxSerialNum"));
        comboBoxSerialNum->setGeometry(QRect(90, 60, 161, 51));
        comboBoxDeviceType = new QComboBox(centralWidget);
        comboBoxDeviceType->setObjectName(QStringLiteral("comboBoxDeviceType"));
        comboBoxDeviceType->setGeometry(QRect(300, 60, 161, 51));
        labelSerialNum = new QLabel(centralWidget);
        labelSerialNum->setObjectName(QStringLiteral("labelSerialNum"));
        labelSerialNum->setGeometry(QRect(100, 10, 101, 41));
        labelDeviceType = new QLabel(centralWidget);
        labelDeviceType->setObjectName(QStringLiteral("labelDeviceType"));
        labelDeviceType->setGeometry(QRect(300, 10, 101, 41));
        pushButtonFoundInverter = new QPushButton(centralWidget);
        pushButtonFoundInverter->setObjectName(QStringLiteral("pushButtonFoundInverter"));
        pushButtonFoundInverter->setGeometry(QRect(500, 60, 191, 51));
        textBrowserResponse = new QTextBrowser(centralWidget);
        textBrowserResponse->setObjectName(QStringLiteral("textBrowserResponse"));
        textBrowserResponse->setGeometry(QRect(90, 140, 971, 381));
        pushButtonFoundInverterStop = new QPushButton(centralWidget);
        pushButtonFoundInverterStop->setObjectName(QStringLiteral("pushButtonFoundInverterStop"));
        pushButtonFoundInverterStop->setGeometry(QRect(730, 60, 191, 51));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1278, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "HolipDrive", 0));
        labelSerialNum->setText(QApplication::translate("MainWindow", "\344\270\262\345\217\243\351\200\211\346\213\251", 0));
        labelDeviceType->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\351\200\211\346\213\251", 0));
        pushButtonFoundInverter->setText(QApplication::translate("MainWindow", "\345\257\273\346\211\276\345\217\230\351\242\221\345\231\250", 0));
        pushButtonFoundInverterStop->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\350\277\236\346\216\245", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
