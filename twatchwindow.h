#ifndef TWATCHWINDOW_H
#define TWATCHWINDOW_H

#include <QString>
#include <QVector>
#include <QWidget>

#include "options.h"
#include "chartwindow.h"
#include "twatchchart.h"
#include "ui_twatchchart.h"


//библиотеки QtCharts
#include <QValueAxis>
#include <QLineSeries>
#include <QChart>
#include <QChartView>

using namespace QtCharts;

namespace Ui {
class TWatchWindow;
}

class TWatchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TWatchWindow(QWidget *parent = nullptr);
    ~TWatchWindow();

    QVector <TWatchChart*> chartView;
    QVector <QVector <QLineSeries*>> series;


    QGridLayout *grid;
    Ui::TWatchWindow *ui;

     void createChart(QString chartName);
     void addSeriesToChart(QLineSeries* series, TWatchChart* chartView);
private:
    void createSingleLayout();
    void createRowLayout(int rowNumber);
    void createColLayout(int colNumber);
    void createGridLayout(int rowNumber, int colNumber);
    void updateGrid();

signals:
    void updateChartList();
    //void sendAllCharts(QVector <TWatchChart*> chartView);

public slots:
    void updateLayout(QString layoutMthod, int rowNumber, int colNumber);
};

#endif // TWATCHWINDOW_H
