#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , portA(new QSerialPort(this))
    , portB(new QSerialPort(this))
{
    ui->setupUi(this);

    // Varsayılan port isimleri
    ui->lineEdit->setText("COM1");
    ui->lineEdit_2->setText("COM2");

    // Buton bağlantıları
    connect(ui->pushButton,   &QPushButton::clicked, this, &MainWindow::openPorts);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::closePorts);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::sendPingFromA);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::sendPingFromB);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::clearLog);

    // Port bağlantıları
    connect(portA, &QSerialPort::readyRead, this, &MainWindow::onPortAReady);
    connect(portB, &QSerialPort::readyRead, this, &MainWindow::onPortBReady);
}

MainWindow::~MainWindow()
{
    closePorts();
    delete ui;
}

void MainWindow::log(const QString &text)
{
    QString ts = QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
    ui->textBrowser->append(QString("[%1] %2").arg(ts, text));
}

void MainWindow::openPorts()
{
    if (portA->isOpen() || portB->isOpen()) {
        log("Portlar zaten açık.");
        return;
    }

    QString nameA = ui->lineEdit->text().trimmed();
    QString nameB = ui->lineEdit_2->text().trimmed();

    if (nameA.isEmpty() || nameB.isEmpty()) {
        log("Port isimlerini giriniz.");
        return;
    }

    portA->setPortName(nameA);
    portA->setBaudRate(QSerialPort::Baud9600);
    portA->setDataBits(QSerialPort::Data8);
    portA->setParity(QSerialPort::NoParity);
    portA->setStopBits(QSerialPort::OneStop);

    portB->setPortName(nameB);
    portB->setBaudRate(QSerialPort::Baud9600);
    portB->setDataBits(QSerialPort::Data8);
    portB->setParity(QSerialPort::NoParity);
    portB->setStopBits(QSerialPort::OneStop);

    if (!portA->open(QIODevice::ReadWrite)) {
        log("PortA açılamadı: " + portA->errorString());
        return;
    }
    if (!portB->open(QIODevice::ReadWrite)) {
        log("PortB açılamadı: " + portB->errorString());
        portA->close();
        return;
    }

    log(QString("Portlar açıldı: %1 <-> %2").arg(nameA, nameB));
}

void MainWindow::closePorts()
{
    if (portA->isOpen()) portA->close();
    if (portB->isOpen()) portB->close();
    log("Portlar kapatıldı.");
}

void MainWindow::sendPingFromA()
{
    if (!portA->isOpen()) {
        log("PortA açık değil.");
        return;
    }
    QByteArray msg = "PING\n";
    portA->write(msg);
    portA->flush();
    log("A -> PING");
}

void MainWindow::sendPingFromB()
{
    if (!portB->isOpen()) {
        log("PortB açık değil.");
        return;
    }
    QByteArray msg = "PING\n";
    portB->write(msg);
    portB->flush();
    log("B -> PING");
}

void MainWindow::onPortAReady()
{
    QByteArray data = portA->readAll();
    QString s = QString::fromUtf8(data).trimmed();
    if (s.isEmpty()) return;

    log("PortA received: " + s);
    if (s == "PING") {
        portA->write("PONG\n");
        portA->flush();
        log("PortA -> PONG");
    } else if (s == "PONG") {
        log("PortA got PONG");
    }
}

void MainWindow::onPortBReady()
{
    QByteArray data = portB->readAll();
    QString s = QString::fromUtf8(data).trimmed();
    if (s.isEmpty()) return;

    log("PortB received: " + s);
    if (s == "PING") {
        portB->write("PONG\n");
        portB->flush();
        log("PortB -> PONG");
    } else if (s == "PONG") {
        log("PortB got PONG");
    }
}

void MainWindow::clearLog()
{
    ui->textBrowser->clear();   // QTextBrowser içindeki tüm yazıları siler
}
