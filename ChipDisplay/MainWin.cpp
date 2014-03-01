#include "MainWin.h"
#include "ui_MainWin.h"

#include "Chip.h"

#include <QGLWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsItem>

#include <QSvgGenerator>

#include "PainterLG.h"
#include "GDLG.h"
#include "GraphicsSceneLG.h"

MainWin::MainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWin)
{
    ui->setupUi(this);
    last = 0;
    c = 0;

    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
#ifndef QT_NO_OPENGL
    ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
#endif

    ui->graphicsView->setScene(new QGraphicsScene(ui->graphicsView));
    ui->graphicsView->scale(1.0, -1.0);

    connect(ui->graphicsView->scene(),SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)),this,
            SLOT(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)));
}

MainWin::~MainWin()
{
    if(c) { delete c; }
    delete ui;
}

void MainWin::on_actionOpen_CIF_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select CIF file");
    if(!filename.size()) { return; }

    ui->graphicsView->scene()->clear();
    if(c) { delete c; }
    c = new Chip;
    c->load(filename);

    QFileInfo f(filename);
    setWindowTitle(QString("%1 - ChipDisplay").arg(f.fileName()));

    QMap<qint64, QString> counts = c->countObjs();
    ui->layersCmb->clear();
    QLocale l = QLocale::system();
    foreach(qint64 key, counts.keys()) {
        QString str("%1\t%2");
        str = str.arg(counts.value(key)).arg(l.toString(key));
        ui->layersCmb->insertItem(0, str);

        QRect br = c->boundingRect(counts.value(key));
        qDebug(qPrintable(QString("%5 x %1 y %2 w %3 h %4").arg(br.x()).arg(br.y()).arg(br.width()).arg(-br.height()).arg(counts.value(key))));
    }
    ui->layersCmb->setCurrentIndex(0);
    on_btnUpdate_clicked();
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
    QMessageBox::information(this,"info",QString("x: %1\ny: %2\nw: %3\nh: %4\n")
                             .arg(newItm->boundingRect().x())
                             .arg(newItm->boundingRect().y())
                             .arg(newItm->boundingRect().width())
                             .arg(newItm->boundingRect().height()),
                             QMessageBox::Ok);
}

void MainWin::on_actionAbout_ChipLib_triggered()
{
    QMessageBox::information(this, tr("ChipLib version"), tr("ChipLib %1").arg(CHIPLIB_VER), QMessageBox::Ok);
}

void MainWin::on_btnUpdate_clicked()
{
    ui->graphicsView->scene()->clear();
    GraphicsSceneLG* gslg = new GraphicsSceneLG(ui->graphicsView->scene(), ui->chkRealWidths->isChecked());
    if(ui->layerChk->isChecked()) {
        c->render(gslg, ui->layersCmb->currentText().split("\t").at(0), ui->itemsSpin->value());
    } else {
        c->render(gslg, "", ui->itemsSpin->value());
    }
    delete gslg;
}

void MainWin::on_btnDump_clicked()
{
    QString f = QFileDialog::getSaveFileName(this, tr("Dumpfile name"), "", "SVG (*.svg)");
    if(!f.size()) { return; }
    QPainter p;
    QPaintDevice* pdev;
    QString layer = ui->layersCmb->currentText().split("\t").at(0);

    QSvgGenerator* g = new QSvgGenerator;
    g->setFileName(f);
    g->setTitle("ChipLib SVG");
    pdev = g;

    p.begin(pdev);
    if(ui->layerChk->isChecked()) {
        /* // GDLG renderer
        GDLG* g = new GDLG(c->boundingRect(layer), 14);
        c->render(g, layer, ui->itemsSpin->value());
        g->writeFile("/media/cavalierpc/augustin/school/SFP/chip.txt");
        delete g;
        */
        c->render(new PainterLG(&p, ui->chkRealWidths->isChecked()), layer, ui->itemsSpin->value());
    } else {
        c->render(new PainterLG(&p, ui->chkRealWidths->isChecked()), "", ui->itemsSpin->value());
    }
    p.end();
}
