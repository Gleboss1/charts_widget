#ifndef TWATCHCHART_H
#define TWATCHCHART_H

#include <QWidget>
#include <QGridLayout>

#include <QString>

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
class TWatchChart;
}

class TWatchChart : public QWidget
{
    Q_OBJECT

public:
    explicit TWatchChart(QWidget *parent = nullptr);
    ~TWatchChart();

    QChartView *chartView;
    QChart *chart;
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();

    Ui::TWatchChart *ui;
};

#endif // TWATCHCHART_H
