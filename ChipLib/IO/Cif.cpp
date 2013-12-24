#include "Cif.h"

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
                    LOG("ERR", lineNum+1, "Unrecognized CALL token!");
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
        LOG("WARN", lineNum+1, QString("unrecognized token %1").arg(objs[0][0]));
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
    QList<ChipObject*> a;
    foreach(File::Command s, file->rootCommands) {
        a.append(command(chip, file, s));
    }
    if(!a.size()) {
        LOG("ERR", -1, "Interpreter didn't work!");
        return false;
    }
    return true;
}

/**************************************************************
 **************** PRIVATE INTERP STUFFS! **********************
 **************************************************************/
bool Interpreter::subroutine(Chip* chip, Cif::File* file, File::Subroutine func, QList<qint64> params)
{
    QList<interp_Transform> trans;
    bool containsR;
    for(int i = 1; i < params.size(); i++) {
        switch(params.at(i)) {
        case M:
        {
            interp_Transform t;
            t.type = M;
            t.params.append(params.at(i+1));
            i += 1;
        }
            continue;
        case R:
        {
            interp_Transform t;
            t.type = R;
            t.params.append(params.at(i+1));
            t.params.append(params.at(i+2));
            trans.append(t);
            i += 2;
            containsR = true;
        }
            continue;
        case T:
        {
            interp_Transform t;
            t.type = T;
            t.params.append(params.at(i+1));
            t.params.append(params.at(i+2));
            trans.append(t);
            i += 2;
        }
            continue;
        default:
            LOG("ERR", -1, "Unexpected transform type!");
        }
    }
    QList<ChipObject*> items;
    foreach(File::Command c, func.commands) {
        items.append(command(chip, file, c, trans));
    }

    foreach(interp_Transform t, trans) {
        if(t.type == R) {
            // TODO
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
                rot   = (cmd.params.size() == 5) ? cmd.params[4] : 0;
        foreach(interp_Transform t, trans) {
            if(t.type == M) {
                if(t.params.at(0) == X) { width = width * -1; }
                if(t.params.at(0) == Y) { length = length * -1; }
            } else if(t.type == T) {
                xpos += t.params.at(0);
                ypos += t.params.at(1);
            } // ignoring R type, it's handled by the caller
        }
        ret.append(layer->addRect(length, width, xpos, ypos, rot));
    }
        break;
    case POLYGON:
        break;
    case WIRE:
    {
        PointList p;
        for(int i = 1; i < cmd.params.size(); i+=2) {
            p.append(qMakePair(cmd.params[i], cmd.params[i+1]));
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
        subroutine(chip, file, file->subroutines.value(cmd.params.at(0)), cmd.params);
        break;
    default:
        LOG("ERR", -1, QString("Token '%1' not implemented!").arg(QChar(cmd.token)));
    }
    return ret;
}

}
