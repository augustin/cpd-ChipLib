#ifndef CIFREAD_H
#define CIFREAD_H

class QGraphicsView;
class QString;
class QByteArray;

#define LOG(type, line, desc) qDebug(QString("%1: %2 \t %3").arg(type).arg(line+1).arg(desc).toUtf8().constData())

QByteArray qFileGetContents(QString fileName);

#endif // CIFREAD_H
