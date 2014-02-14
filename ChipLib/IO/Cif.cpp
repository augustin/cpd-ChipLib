#include "Cif.h"

#include <math.h>
#include <QElapsedTimer>
#include <iostream>

namespace Cif {
File* Reader::Read(QString data)
{
    File* ret = new File;

    QStringList lines = data.split("\n");
    for(int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        /* A "(" is a comment. */
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
        if(objs.length() == 7) {
            ret.params.append(objs[5].toInt());
            ret.params.append(objs[6].toInt());
        }
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
    case WIRE:
        ret.token = WIRE;
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
                    i += 1;
                    continue;
                case R:
                    ret.params.append(R);
                    break;
                case T:
                    ret.params.append(T);
                    break;
                default:
                    LOG("ERR", lineNum+1, "Unrecognized CALL transform!");
                    i = objs.size();
                    *worked = false;
                    continue;
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
        LOG("WARN", lineNum+1, QString("Unrecognized token %1!").arg(objs[0][0]));
        *worked = false;
    }
    return ret;
}

ChipLayer* Interpreter::layer;

/**************************************************************
 ********************* THE INTERP! ****************************
 **************************************************************/
bool Interpreter::run(Chip *chip, Cif::File* file)
{
    ensureLog();
    QElapsedTimer t;
    t.start();

    QList<ChipObject*> a;
    foreach(File::Command s, file->rootCommands) {
        a.append(command(chip, file, s));
    }

    qint64 el = t.elapsed();
    writeLog(QString("I-001: Interp took %1 ms.").arg(el));
    return true;
}

/**************************************************************
 **************** PRIVATE INTERP STUFFS! **********************
 **************************************************************/
bool Interpreter::subroutine(Chip* chip, Cif::File* file, File::Subroutine func, QList<qint64> params, QList<interp_Transform> transforms)
{
    QList<interp_Transform> trans;
    for(int i = 1; i < params.size(); i+=2) {
        switch(params.at(i)) {
        case M:
        {
            interp_Transform t;
            t.type = M;
            t.params.append(params.at(i+1));
            trans.append(t);
        }
            continue;
        case R:
        {
            interp_Transform t;
            t.type = R;
            t.params.append(atan2(params.at(i+2), params.at(i+1))*180/M_PI);
            trans.append(t);
            i += 1;
        }
            continue;
        case T:
        {
            interp_Transform t;
            t.type = T;
            t.params.append(params.at(i+1));
            t.params.append(params.at(i+2));
            trans.append(t);
            i += 1;
        }
            continue;
        default:
            break;
        }
    }
    trans.append(transforms);

    QList<ChipObject*> items;
    foreach(File::Command c, func.commands) {
        items.append(command(chip, file, c, trans));
    }

    foreach(ChipObject* itm, items) {
        foreach(interp_Transform t, trans) {
            if(t.type == R) {
                int deg = t.params.at(0);
                foreach(Point* p, itm->points) {
                    qint64 x = p->x, y = p->y;
                    p->x = cos(deg*M_PI/180)*x + cos((deg+90)*M_PI/180)*y;
                    p->y = sin((deg+90)*M_PI/180)*y + sin(deg*M_PI/180)*x;
                }
            } else if(t.type == M) {
                if(t.params.at(0) == X) {
                    foreach(Point* p, itm->points) { p->x *= -1; }
                } else if(t.params.at(0) == Y) {
                    foreach(Point* p, itm->points) { p->y *= -1; }
                }
            } else if(t.type == T) {
                foreach(Point* p, itm->points) {
                    p->x += t.params.at(0);
                    p->y += t.params.at(1);
                }
            }
        }
    }

    return true;
}

/* ::command -- parses all the things in the command, adds them to chip, and returns what it added */
QList<ChipObject*> Interpreter::command(Chip* chip, Cif::File* file, File::Command cmd, QList<interp_Transform> trans)
{
    QList<ChipObject*> ret;
    switch(cmd.token) {
    case BOX:
    {
        qint64 length = cmd.params[0],
                width = cmd.params[1],
                xpos  = cmd.params[2],
                ypos  = cmd.params[3],
                rot   = (cmd.params.size() == 6) ? 1 : 0;
        if(rot != 0) { LOG("ERR", -1, "BOX rotation not supported!"); }
        ret.append(layer->addRect(length, width, xpos, ypos));
    }
        break;
    case POLYGON:
    {
        PointList p;
        for(int i = 0; i < cmd.params.size(); i+=2) {
            p.append(new Point(cmd.params[i], cmd.params[i+1]));
        }
        ret.append(layer->addPoly(p));
    }
        break;
    case WIRE:
    {
        PointList p;
        for(int i = 1; i < cmd.params.size(); i+=2) {
            p.append(new Point(cmd.params[i], cmd.params[i+1]));
        }
        ret.append(layer->addLine(p, cmd.params[0]));
    }
        break;
    case ROUNDFLASH:
        break;
    case LAYER:
        layer = chip->layer(cmd.name);
        break;
    case CALL:
        subroutine(chip, file, file->subroutines.value(cmd.params.at(0)), cmd.params, trans);
        break;
    default:
        LOG("ERR", -1, QString("Token '%1' not implemented!").arg(QChar(cmd.token)));
    }
    return ret;
}

}
