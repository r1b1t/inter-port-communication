#ifndef LOGCONTROLLER_H
#define LOGCONTROLLER_H

#include <QObject>
#include <QString>
#include <QTextBrowser>
#include <QDateTime>

class LogController : public QObject
{
    Q_OBJECT
public:
    explicit LogController(QTextBrowser *output, QObject *parent = nullptr);

    void log(const QString &text);
    void clear();

private:
    QTextBrowser *m_output;
};

#endif // LOGCONTROLLER_H
