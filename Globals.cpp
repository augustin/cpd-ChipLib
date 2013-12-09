#include "Globals.h"

#include <QByteArray>
#include <QString>
#include <QFile>

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
