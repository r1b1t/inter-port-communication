#include "logcontroller.h"

LogController::LogController(QTextBrowser *output, QObject *parent)
    : QObject(parent), m_output(output)
{
}

void LogController::log(const QString &text)
{
    if (!m_output) return;
    QString ts = QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
    m_output->append(QString("[%1] %2").arg(ts, text));
}

void LogController::clear()
{
    if (m_output)
        m_output->clear();
}
