#include "Globals.h"

#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDateTime>

QFile logstream;

QByteArray qFileGetContents(QString fileName)
{
    QFile f(fileName);
    if(!f.open(QFile::ReadOnly)) {
        return "";
    }
    QByteArray ret = f.readAll();
    f.close();
    return ret;
}

void ensureLog()
{
    if(logstream.isOpen()) { return; }
    logstream.setFileName("/var/log/ChipLib.log");
    logstream.open(QFile::WriteOnly);
    writeLog(QDateTime::currentDateTime().toString("\nMM/dd/yy hh:mm:ss AP"));
}

void writeLog(QString log)
{
    logstream.write(log.toUtf8() + "\n");
    logstream.flush();
}
