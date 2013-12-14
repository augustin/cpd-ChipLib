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
        for(int i = 1; i < objs.size(); i++) {
            ret.params.append(objs[i].toInt());
        }
        break;
    case CALL:
        ret.token = CALL;
        ret.params.append(objs[1].toInt());
        if(objs.size() > 2) {
            for(int i = 2; i < objs.size(); i++) {
                switch(objs[i][0].toLatin1()) {
                case M:
                    ret.params.append(M);
                    if(objs[i+1].startsWith("X")) {
                        ret.params.append(X);
                    } else {
                        ret.params.append(Y);
                    }
                    ret.params.append(-1); /* empty command */
                    i += 1;
                    continue;
                case R:
                    ret.params.append(R);
                    break;
                case T:
                    ret.params.append(T);
                    break;
                default:
                    LOG("ERR", lineNum+1, "Unrecognized CALL token!");
                    *worked = false;
                }

                ret.params.append(objs[i+1].toInt());
                ret.params.append(objs[i+2].toInt());
                i += 2;
            }
        }
        break;
    case LAYER:
        ret.token = LAYER;
        ret.name = objs[1];
        break;
    default:
        LOG("WARN", lineNum+1, QString("unrecognized token %1").arg(objs[0][0]));
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
                LOG("ERR", -1, QString("Function '%1' not implemented!").arg(QChar(c.token)));
            }
        }
        break;
    }

    return true;
}

}
