#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openPorts();
    void closePorts();
    void sendPingFromA();
    void sendPingFromB();
    void onPortAReady();
    void onPortBReady();
    void clearLog();
    void sendCustomMessage();

private:
    Ui::MainWindow *ui;

    QSerialPort *portA;
    QSerialPort *portB;

    void log(const QString &text);
};

#endif // MAINWINDOW_H
