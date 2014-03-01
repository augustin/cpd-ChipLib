#include "Globals.h"

#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDateTime>
#include <QDir>

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

    QByteArray logData = qFileGetContents(QDir::homePath() + "/.ChipLib.log");
    logstream.setFileName(QDir::homePath() + "/.ChipLib.log");
    logstream.open(QFile::WriteOnly);
    logstream.write(logData);
}

void writeLog(QString log)
{
    logstream.write(QDateTime::currentDateTime().toString("MM/dd/yy hh:mm:ss AP, ").toUtf8());
    logstream.write(log.toUtf8() + "\n");
    logstream.flush();
}
