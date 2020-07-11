#ifndef TWATCHSERIES_H
#define TWATCHSERIES_H

#include <QString>
#include <QPen>

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

class TWatchSeries
{
public:
    TWatchSeries();
    QLineSeries *series = new QLineSeries();
    QString name;
    QPen pen;
    QColor color;
    bool legendVisibility;
    //добавить line type

};

#endif // TWATCHSERIES_H
