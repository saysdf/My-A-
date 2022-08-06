#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QRandomGenerator>
#include "Grid.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void recalc();
private slots:


    void on_actionTest_triggered();

private:
    Grid * grd;
    std::vector<int> map;
    std::pair<int,int> start;
    std::pair<int,int> end;
    std::vector<int> way;
    QRandomGenerator randomizer;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
