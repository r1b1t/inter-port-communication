#pragma once
#include <QObject>
#include <QSerialPort>

class PortController : public QObject
{
    Q_OBJECT
public:
    explicit PortController(QObject *parent = nullptr);
    ~PortController();

public slots:
    void openPorts(const QString &nameA, const QString &nameB);
    void closePorts();
    void sendFromA(const QByteArray &data);
    void sendFromB(const QByteArray &data);
    void sendPingFromA();
    void sendPingFromB();
    void sendCustomMessage(const QString &sender, const QString &message);



signals:
    void logMessage(const QString &text);
    void messageReceived(const QString &sender, const QString &msg);

private slots:
    void onAReady();
    void onBReady();


private:
    QSerialPort *portA;
    QSerialPort *portB;
};
