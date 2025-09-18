#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "portcontroller.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , controller(new PortController(this))   // <-- PortController eklendi
    , logger(nullptr)
{
    ui->setupUi(this);
    logger = new LogController(ui->textBrowser, this);


    // Varsayılan port isimleri
    ui->lineEdit->setText("COM1");
    ui->lineEdit_2->setText("COM2");

    // Controller sinyalleri MainWindow slotlarına bağla
    connect(controller, &PortController::logMessage, logger, &LogController::log);
    connect(controller, &PortController::messageReceived, this, [=](QString who, QString msg){
        logger->log(who + " received: " + msg);
    });

    // UI butonlarını controller’a bağla
    connect(ui->pushButton, &QPushButton::clicked, this, [=]{
        controller->openPorts(ui->lineEdit->text(), ui->lineEdit_2->text());
    });
    connect(ui->pushButton_2, &QPushButton::clicked, controller, &PortController::closePorts);

    connect(ui->pushButton_3, &QPushButton::clicked, controller, &PortController::sendPingFromA);

    connect(ui->pushButton_4, &QPushButton::clicked, controller, &PortController::sendPingFromB);

    // Temizleme butonu
    connect(ui->pushButton_5, &QPushButton::clicked, logger, &LogController::clear);

    connect(ui->pushButton_6, &QPushButton::clicked, this, [=]{
        controller->sendCustomMessage(ui->comboBox->currentText(), ui->lineEdit_3->text());
        ui->lineEdit_3->clear();
    });


}

MainWindow::~MainWindow()
{
    disconnect(controller, nullptr, this, nullptr);  // controller → MainWindow bağlantılarını kopar
    delete ui;
}


