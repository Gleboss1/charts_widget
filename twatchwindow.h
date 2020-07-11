#ifndef TWATCHWINDOW_H
#define TWATCHWINDOW_H

#include <QString>
#include <QVector>
#include <QWidget>

#include "options.h"
#include "chartwindow.h"


#include "twatchchart.h"
#include "ui_twatchchart.h"
#include "twatchseries.h"

//библиотеки QtCharts
#include <QValueAxis>
#include <QLineSeries>
#include <QValueAxis>
#include <QChart>
#include <QChartView>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>

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

    //QChartView *chartView;
    QVector <TWatchChart*> chartView;

    QGridLayout *grid;
    Ui::TWatchWindow *ui;
private:
    void createSingleLayout();
    void createRowLayout(int rowNumber);
    void createColLayout(int colNumber);
    void createGridLayout(int rowNumber, int colNumber);

public slots:
    void updateLayout(QString layoutMthod, int rowNumber, int colNumber);
};

#endif // TWATCHWINDOW_H
