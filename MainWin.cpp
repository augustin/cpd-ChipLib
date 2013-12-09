#include "MainWin.h"
#include "ui_MainWin.h"

#include <QGLWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsItem>

#include "Cif.h"
#include "GraphicsLayerView.h"

MainWin::MainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWin)
{
    ui->setupUi(this);
    last = 0;
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    //ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
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
    QString filename = QFileDialog::getOpenFileName(this, "Select CIF file");
    if(filename.size()) {
        Cif::File* f;
        f = Cif::Reader::Read(qFileGetContents(filename));
        Cif::Parse(new GraphicsLayerView(ui->graphicsView->scene()), f);
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
    QMessageBox::information(this,"info",QString("x: %1 y: %2 w: %3 h: %4")
                             .arg(newItm->boundingRect().x())
                             .arg(newItm->boundingRect().y())
                             .arg(newItm->boundingRect().width())
                             .arg(newItm->boundingRect().height()),
                             QMessageBox::Ok);
}
