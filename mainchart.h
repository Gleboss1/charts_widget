#ifndef MAINCHART_H
#define MAINCHART_H

#include <QMainWindow>
#include <QVector>
#include <QRandomGenerator>

#include "options.h"
#include "ui_options.h"
#include "ui_twatchwindow.h"
#include "chartwindow.h"

//классы графиков
#include "twatchwindow.h"
#include "twatchchart.h"
#include "twatchseries.h"

//библиотеки QtCharts
#include <QValueAxis>
#include <QLineSeries>
#include <QValueAxis>
#include <QChart>
#include <QChartView>
#include <QtCharts/QLegend>

using namespace QtCharts;

class Options;
class TWatchWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainChart; }
QT_END_NAMESPACE

class MainChart : public QMainWindow
{
    Q_OBJECT

public:
    MainChart(QWidget *parent = nullptr);
    ~MainChart();

private slots:
    void on_actionOptions_triggered();

    void on_actionCreate_owm_window_triggered();


    void on_tabWidget_tabCloseRequested(int index);

    void on_actionAdd_new_chart_triggered();


private:
    Ui::MainChart *ui;

    QVector <TWatchWindow*> tab;
    QVector <TWatchSeries*> series;

    Options *option_wnd;    //окно опций
    //ChartWindow *chart_wnd; //окно отдельного графика

    void loadOptionSeries();
    void updateOptionSeries(QString seriesName);

    void createRandomSeries(int amount);
    void updateSeriesColor(int index);
    void updateSeriesLineType(int index);
    void updateSeriesName(int index);
public slots:
    void updateOptions(); //обновить данные о серии
    //void updateLayout();


};
#endif // MAINCHART_H
