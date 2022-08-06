#include "mainwindow.h"
#include "ui_mainwindow.h"
#define RECT_SIZE 30
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    map.resize(18*18,1);
    grd = new Grid(18,18,&map);
    start = {1,2};
    end = {17,13};
    recalc();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{

    QPainter p(this);
    p.fillRect(0,0,this->width(),this->height(),Qt::white);
    double size = RECT_SIZE;
    double xp=size,yp=size;
    for (size_t i=0;i<map.size();++i){
        int avlbl = map[i];
        auto pos = grd->toPolar(i);
        switch (avlbl) {
        case  1: p.setBrush(QColor("#c8faf8")); break;
        case  0: p.setBrush(QColor("#f2b9b6")); break;
        default: p.setBrush(QColor("#8a8888")); break;
        }
        p.drawRect(xp*std::get<0>(pos),
                   yp*std::get<1>(pos),
                   size,size);

    }
    p.setBrush(QColor("#00fc11"));
    p.drawEllipse(start.first*xp+size/2,start.second*yp+size/2,size/2,size/2);
    p.setBrush(QColor("#006eff"));
    p.drawEllipse(end.first*xp+size/2,end.second*yp+size/2,size/2,size/2);
    p.setBrush(QColor("#eeff00"));
    for (size_t i=0;i<way.size();++i){
        auto pos = grd->toPolar(way[i]);

        p.drawEllipse(xp*std::get<0>(pos),
                   yp*std::get<1>(pos),
                   size/2,size/2);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{

    std::pair<size_t,size_t> cord = {(e->pos().x())/RECT_SIZE,(e->pos().y())/RECT_SIZE};
    qInfo()<<cord<<e->pos();
    if (cord.first>grd->getWidth() || cord.second>grd->getHeight())
        return;
    if (e->modifiers() & Qt::ControlModifier){
        start = cord;
        //старт
    }
    else if (e->modifiers() & Qt::ShiftModifier){
        end = cord;
        //end
    }
    else {
        int index = grd->fromPolar(std::make_tuple(cord.first,cord.second));
        map[index] = map[index]?0:1;
    }
    recalc();
}



void MainWindow::recalc()
{
    way.clear();
    grd->aStar(start,end,way);
    repaint();
}

void MainWindow::on_actionTest_triggered()
{

    for (auto &i : map){
        i= randomizer.generate()%2;
    }
    repaint();
}
