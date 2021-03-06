#ifndef CIF_H
#define CIF_H

#include <QList>
#include <QMap>
#include <QStringList>

#include "../Chip.h"
#include "Globals.h"

namespace Cif {
enum Token {
    BOX = 'B',
    ROUNDFLASH ='R',
    POLYGON = 'P',
    WIRE = 'W',
    CALL = 'C',
    LAYER = 'L'
};
enum TransType {
    M = 'M', /* mirror */
    X = 'X', /* X (for "mirror") */
    Y = 'Y', /* Y (for "mirror") */
    R = 'R',  /* rotate */
    T = 'T' /* translate */
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

class Interpreter {
public:
    static bool run(Chip *chip, Cif::File* file);

private:
    /* private parser stuff */
    struct interp_Transform {
        TransType type;
        qint64 tparams[2];
    };

    static bool subroutine(Chip *chip, Cif::File* file, File::Subroutine func,
                           QList<qint64> params = QList<qint64>(),
                           QList<interp_Transform> transforms = QList<interp_Transform>());
    static QList<ChipObject*> command(Chip *chip, Cif::File* file, File::Command cmd,
                                      QList<interp_Transform> trans = QList<interp_Transform>());

    static ChipLayer* layer;
};

}

#endif // CIF_H
