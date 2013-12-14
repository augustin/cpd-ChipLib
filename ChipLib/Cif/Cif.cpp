#include "Cif.h"

#include <QStringList>

#include "../GraphicsLayer.h"

namespace Cif {
File* Reader::Read(QString data)
{
    File* ret = new File;

    QStringList lines = data.split("\n");
    for(int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        /* A "(" is apparently a comment... ? */
        if(line.startsWith("(")) { continue; }

        if(line.startsWith("DS")) {
            i += 1;
            File::Subroutine a;
            QStringList params = line.split(" ");
            a.scaleNumerator = params[2].toInt();
            a.scaleDenominator = params[3].replace(";","").toInt();
            for(; i < lines.size(); i++) {
                line = lines.at(i);
                if(line.startsWith("DF")) {
                    break;
                } else if(line.startsWith("9")) {
                    a.name = line.replace(";","").split(" ")[1];
                } else {
                    bool worked;
                    File::Command c = readCommand(line, i, &worked);
                    if(worked) { a.commands.append(c); }
                }
            }
            ret->subroutines.insert(params[1].toInt(), a);
        } else if(line.startsWith("E")) {
            /* End of file -- we're done here! */
            break;
        } else {
            bool worked;
            File::Command c = readCommand(line, i, &worked);
            if(worked) { ret->rootCommands.append(c); }
        }
    }

    return ret;
}
File::Command Reader::readCommand(QString l, int lineNum, bool* worked)
{
    QStringList objs = l.trimmed().replace(";","").split(" ");
    File::Command ret;
    *worked = true;
    switch(objs[0][0].toLatin1()) {
    case BOX:
        ret.token = BOX;
        ret.params.append(objs[1].toInt());
        ret.params.append(objs[2].toInt());
        ret.params.append(objs[3].toInt());
        ret.params.append(objs[4].toInt());
        if(objs.length() == 6) { ret.params.append(objs[5].toInt()); qDebug("hi"); }
        break;
    case ROUNDFLASH:
        ret.token = ROUNDFLASH;
        *worked = false;
        break;
    case POLYGON:
        ret.token = POLYGON;
        *worked = false;
        break;
    case CALL:
        ret.token = CALL;
        *worked = false;
        break;
    case LAYER:
        ret.token = LAYER;
        ret.name = objs[1];
        break;
    default:
        LOG("WARN", lineNum, QString("unrecognized token %1").arg(objs[0][0].toLatin1()));
        *worked = false;
    }
    return ret;
}

bool Parse(GraphicsLayer *abs, Cif::File* file)
{
    foreach(File::Subroutine s, file->subroutines.values()) {
        foreach(File::Command c, s.commands) {
            if(c.token == BOX) {
                if(c.params.size() == 5) {
                    abs->rect(c.params[0],c.params[1],c.params[2],c.params[3],c.params[4]);
                } else {
                    abs->rect(c.params[0],c.params[1],c.params[2],c.params[3]);
                }
            } else {
                LOG("WARN", -2, "Parser function not implemented!!");
            }
        }
        break;
    }

    return true;
}

}
