#include "MainWin.h"
#include "ui_MainWin.h"

#include "Chip.h"

#include <QGLWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsItem>
#include <QSvgGenerator>

#include "PainterLG.h"
#include "GraphicsSceneLG.h"

MainWin::MainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWin)
{
    ui->setupUi(this);
    last = 0;
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    ui->graphicsView->setScene(new QGraphicsScene(ui->graphicsView));
    ui->graphicsView->scale(1.0,-1.0);

    connect(ui->graphicsView->scene(),SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)),this,
            SLOT(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)));
}

MainWin::~MainWin()
{
    delete ui;
}

void MainWin::on_actionOpen_CIF_triggered()
{
    ui->graphicsView->setVisible(false);
    ui->graphicsView->scene()->clear();

    QString filename = QFileDialog::getOpenFileName(this, "Select CIF file");
    if(filename.size()) {
        Chip* c = new Chip;
        c->load(filename);
        QMap<QString, qint64> counts = c->countObjs();
        /*
        QLocale l = QLocale::system();
        foreach(QString key, counts.keys()) {
            qDebug(QString("%1\t%2").arg(key).arg(l.toString(counts.value(key))).toUtf8().constData());
        }*/
        c->render(new GraphicsSceneLG(ui->graphicsView->scene()), "", 12000);
        delete c;
    }

    ui->graphicsView->setVisible(true);
}

void MainWin::on_horizontalSlider_sliderMoved(int position)
{
    qreal scale = position/100.0;
    ui->graphicsView->scale(scale,scale);
    ui->horizontalSlider->setValue(100);
}

void MainWin::focusItemChanged(QGraphicsItem*newItm,QGraphicsItem*,Qt::FocusReason)
{
    if(!newItm) { return; }
    if(last && (last->boundingRect().x() == newItm->boundingRect().x())) { return; }
    last = newItm;
    QMessageBox::information(this,"info",QString("x: %1\ny: %2\nw: %3\nh: %4\nr: %5")
                             .arg(newItm->boundingRect().x())
                             .arg(newItm->boundingRect().y())
                             .arg(newItm->boundingRect().width())
                             .arg(newItm->boundingRect().height())
                             .arg(newItm->rotation()),
                             QMessageBox::Ok);
}

void MainWin::on_actionAbout_ChipLib_triggered()
{
    QMessageBox::information(this, tr("ChipLib version"), tr("ChipLib %1").arg(CHIPLIB_VER), QMessageBox::Ok);
}
