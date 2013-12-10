#ifndef CIF_H
#define CIF_H

#include <QList>
#include <QMap>
#include <QVariant>
#include <QStringList>
#include "../Globals.h"

class GraphicsLayer;

namespace Cif {
enum Token {
    BOX = 'B',
    ROUNDFLASH ='R',
    POLYGON = 'P',
    CALL = 'C',
    LAYER = 'L'
};
struct File {
    /* An operation. Either creates a graphics object,
     * or makes a CALL to execute a Subroutine of commands. */
    struct Command {
        Token token;
        QList<qint64> params;
        QString name; /* Only used by the LAYER command */
    };

    /* The CIF equivalent of a function. */
    struct Subroutine {
        QList<Command> commands;
        QString name; /* only used by the 9 command */
        int scaleNumerator, scaleDenominator;
    };

    QMap<int, Subroutine> subroutines;
    QList<Command> rootCommands;
};

class Reader {
public:
    static Cif::File* Read(QString data);

private:
    static File::Command readCommand(QString l, int lineNum, bool *worked);
};

bool Parse(GraphicsLayer *abs, Cif::File* file);
}

#endif // CIF_H
