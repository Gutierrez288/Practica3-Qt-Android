/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btnIzquierda;
    QPushButton *btnAtras;
    QPushButton *btnDerecha;
    QPushButton *btnAdelante;
    QWidget *contenedorGrafica;
    QCheckBox *chkTracking;
    QCheckBox *chkFuga;
    QLabel *lblServo;
    QSlider *sliderServo;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"    background-color: #1e1e1e;\n"
"}\n"
"QPushButton {\n"
"    background-color: #333333;\n"
"    color: white;\n"
"    border: 2px solid #555555;\n"
"    border-radius: 8px;\n"
"    padding: 15px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #007acc;\n"
"    border: 2px solid #005999;\n"
"}\n"
"QLabel, QCheckBox {\n"
"    color: white;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"QCheckBox::indicator {\n"
"    width: 20px;\n"
"    height: 20px;\n"
"}\n"
"QCheckBox::indicator:checked {\n"
"    background-color: #007acc;\n"
"    border-radius: 4px;\n"
"}\n"
"QCheckBox::indicator:unchecked {\n"
"    background-color: #333333;\n"
"    border: 1px solid #555555;\n"
"    border-radius: 4px;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        btnIzquierda = new QPushButton(centralwidget);
        btnIzquierda->setObjectName("btnIzquierda");
        btnIzquierda->setGeometry(QRect(0, 170, 101, 51));
        btnAtras = new QPushButton(centralwidget);
        btnAtras->setObjectName("btnAtras");
        btnAtras->setGeometry(QRect(70, 230, 111, 51));
        btnDerecha = new QPushButton(centralwidget);
        btnDerecha->setObjectName("btnDerecha");
        btnDerecha->setGeometry(QRect(140, 170, 111, 51));
        btnAdelante = new QPushButton(centralwidget);
        btnAdelante->setObjectName("btnAdelante");
        btnAdelante->setGeometry(QRect(70, 110, 111, 51));
        contenedorGrafica = new QWidget(centralwidget);
        contenedorGrafica->setObjectName("contenedorGrafica");
        contenedorGrafica->setGeometry(QRect(410, 60, 400, 350));
        chkTracking = new QCheckBox(centralwidget);
        chkTracking->setObjectName("chkTracking");
        chkTracking->setGeometry(QRect(30, 300, 131, 21));
        chkFuga = new QCheckBox(centralwidget);
        chkFuga->setObjectName("chkFuga");
        chkFuga->setGeometry(QRect(30, 330, 121, 23));
        lblServo = new QLabel(centralwidget);
        lblServo->setObjectName("lblServo");
        lblServo->setGeometry(QRect(30, 360, 181, 18));
        sliderServo = new QSlider(centralwidget);
        sliderServo->setObjectName("sliderServo");
        sliderServo->setGeometry(QRect(30, 390, 160, 16));
        sliderServo->setMaximum(180);
        sliderServo->setValue(90);
        sliderServo->setOrientation(Qt::Orientation::Horizontal);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnIzquierda->setText(QCoreApplication::translate("MainWindow", "Izquierda", nullptr));
        btnAtras->setText(QCoreApplication::translate("MainWindow", "Atr\303\241s", nullptr));
        btnDerecha->setText(QCoreApplication::translate("MainWindow", "Derecha", nullptr));
        btnAdelante->setText(QCoreApplication::translate("MainWindow", "Adelante", nullptr));
        chkTracking->setText(QCoreApplication::translate("MainWindow", "Modo Tracking", nullptr));
        chkFuga->setText(QCoreApplication::translate("MainWindow", "Modo Fuga", nullptr));
        lblServo->setText(QCoreApplication::translate("MainWindow", "Control Manual Servo: 90\302\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
