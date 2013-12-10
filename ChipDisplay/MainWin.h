#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>

class QGraphicsItem;

namespace Ui {
class MainWin;
}

class MainWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWin(QWidget *parent = 0);
    ~MainWin();

private slots:
    void on_actionOpen_CIF_triggered();
    void on_horizontalSlider_sliderMoved(int position);
    void focusItemChanged(QGraphicsItem*newItm, QGraphicsItem*, Qt::FocusReason);

private:
    Ui::MainWin *ui;
    QGraphicsItem* last;
};

#endif // MAINWIN_H
