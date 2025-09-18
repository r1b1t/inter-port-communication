#include "portcontroller.h"

PortController::PortController(QObject *parent)
    : QObject(parent),
    portA(new QSerialPort(this)),
    portB(new QSerialPort(this))
{
    connect(portA, &QSerialPort::readyRead, this, &PortController::onAReady);
    connect(portB, &QSerialPort::readyRead, this, &PortController::onBReady);
}

PortController::~PortController()
{
    closePorts();
}

void PortController::openPorts(const QString &nameA, const QString &nameB)
{
    portA->setPortName(nameA);
    portB->setPortName(nameB);

    portA->setBaudRate(QSerialPort::Baud9600);
    portB->setBaudRate(QSerialPort::Baud9600);

    if (!portA->open(QIODevice::ReadWrite)) {
        emit logMessage("PortA açılamadı: " + portA->errorString());
        return;
    }
    if (!portB->open(QIODevice::ReadWrite)) {
        emit logMessage("PortB açılamadı: " + portB->errorString());
        portA->close();
        return;
    }

    emit logMessage("Portlar açıldı.");
}



void PortController::closePorts()
{
    if (portA->isOpen()) portA->close();
    if (portB->isOpen()) portB->close();
    emit logMessage("Portlar kapatıldı.");
}

void PortController::sendFromA(const QByteArray &data)
{
    if (!portA->isOpen()) { emit logMessage("PortA açık değil."); return; }
    portA->write(data);
    emit logMessage("A -> " + QString::fromUtf8(data).trimmed());
}

void PortController::sendFromB(const QByteArray &data)
{
    if (!portB->isOpen()) { emit logMessage("PortB açık değil."); return; }
    portB->write(data);
    emit logMessage("B -> " + QString::fromUtf8(data).trimmed());
}

void PortController::sendPingFromA() {
    sendFromA("PING\n");
}

void PortController::sendPingFromB() {
    sendFromB("PING\n");
}

void PortController::sendCustomMessage(const QString &sender, const QString &message)
{
    if (message.trimmed().isEmpty()) {
        emit logMessage("Mesaj boş olamaz.");
        return;
    }

    QByteArray data = message.toUtf8() + "\n";

    if (sender == "A") {
        sendFromA(data);
    } else if (sender == "B") {
        sendFromB(data);
    }
}



void PortController::onAReady()
{
    QString s = QString::fromUtf8(portA->readAll()).trimmed();
    if (s.isEmpty()) return;
    emit messageReceived("A", s);
}

void PortController::onBReady()
{
    QString s = QString::fromUtf8(portB->readAll()).trimmed();
    if (s.isEmpty()) return;
    emit messageReceived("B", s);
}
