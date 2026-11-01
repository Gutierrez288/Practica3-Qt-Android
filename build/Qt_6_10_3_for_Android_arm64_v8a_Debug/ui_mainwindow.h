/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QWidget *contenedorGrafica;
    QGridLayout *gridLayout;
    QPushButton *btnAdelante;
    QPushButton *btnAtras;
    QPushButton *btnDerecha;
    QPushButton *btnIzquierda;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *lblServo;
    QSlider *sliderServo;
    QCheckBox *chkTracking;
    QCheckBox *chkFuga;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(895, 600);
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
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        contenedorGrafica = new QWidget(centralwidget);
        contenedorGrafica->setObjectName("contenedorGrafica");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(contenedorGrafica->sizePolicy().hasHeightForWidth());
        contenedorGrafica->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(contenedorGrafica);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        btnAdelante = new QPushButton(centralwidget);
        btnAdelante->setObjectName("btnAdelante");

        gridLayout->addWidget(btnAdelante, 1, 0, 1, 2);

        btnAtras = new QPushButton(centralwidget);
        btnAtras->setObjectName("btnAtras");

        gridLayout->addWidget(btnAtras, 4, 0, 1, 2);

        btnDerecha = new QPushButton(centralwidget);
        btnDerecha->setObjectName("btnDerecha");

        gridLayout->addWidget(btnDerecha, 2, 1, 1, 1);

        btnIzquierda = new QPushButton(centralwidget);
        btnIzquierda->setObjectName("btnIzquierda");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(80);
        sizePolicy1.setVerticalStretch(80);
        sizePolicy1.setHeightForWidth(btnIzquierda->sizePolicy().hasHeightForWidth());
        btnIzquierda->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnIzquierda, 2, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setLineWidth(1);

        verticalLayout->addWidget(label);

        lblServo = new QLabel(centralwidget);
        lblServo->setObjectName("lblServo");

        verticalLayout->addWidget(lblServo);

        sliderServo = new QSlider(centralwidget);
        sliderServo->setObjectName("sliderServo");
        sliderServo->setMaximum(180);
        sliderServo->setValue(90);
        sliderServo->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout->addWidget(sliderServo);

        chkTracking = new QCheckBox(centralwidget);
        chkTracking->setObjectName("chkTracking");

        verticalLayout->addWidget(chkTracking);

        chkFuga = new QCheckBox(centralwidget);
        chkFuga->setObjectName("chkFuga");

        verticalLayout->addWidget(chkFuga);


        horizontalLayout->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 895, 23));
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
        btnAdelante->setText(QCoreApplication::translate("MainWindow", "Adelante", nullptr));
        btnAtras->setText(QCoreApplication::translate("MainWindow", "Atr\303\241s", nullptr));
        btnDerecha->setText(QCoreApplication::translate("MainWindow", "Derecha", nullptr));
        btnIzquierda->setText(QCoreApplication::translate("MainWindow", "Izquierda", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Oscar Daniel Gutierrez Barba ", nullptr));
        lblServo->setText(QCoreApplication::translate("MainWindow", "Control Manual Servo: 90\302\260", nullptr));
        chkTracking->setText(QCoreApplication::translate("MainWindow", "Modo Tracking", nullptr));
        chkFuga->setText(QCoreApplication::translate("MainWindow", "Modo Fuga", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
