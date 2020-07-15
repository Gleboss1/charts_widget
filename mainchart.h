#ifndef MAINCHART_H
#define MAINCHART_H

#include <QMainWindow>
#include <QVector>
#include <QRandomGenerator>

//классы графиков
#include "twatchwindow.h"
#include "ui_twatchwindow.h"
#include "twatchchart.h"
#include "options.h"
#include "ui_options.h"
#include "chartwindow.h"

//библиотеки QtCharts
#include <QValueAxis>
#include <QLineSeries>
#include <QChart>
#include <QChartView>

//что бы знал что они есть
class Options;
class TWatchWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainChart; }
QT_END_NAMESPACE
using namespace QtCharts;

class MainChart : public QMainWindow
{
    Q_OBJECT

public:
    MainChart(QWidget *parent = nullptr);
    ~MainChart();

private slots:
    void on_actionOptions_triggered();              //открыть окно опций
    void on_actionCreate_owm_window_triggered();    //график в новом окне
    void on_tabWidget_tabCloseRequested(int index); //закрыть вкладку
    void on_actionAdd_new_chart_triggered();        //новая вкладка

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainChart *ui;

    QVector <TWatchWindow*> tab;                    //вкладки
    QVector <QLineSeries*> series;                  //серии (данные)
    //ChartWindow *chart_wnd;                       //окно отдельного графика

    Options *option_wnd;                            //окно опций
    int currentTabIndex;
    void loadOptionSeries();
    void updateOptionSeries(QString seriesName);

    void createRandomSeries(int amount);
    void updateSeriesColor(int index);
    void updateSeriesLineType(int index);
    void updateSeriesName(int index);

public slots:
    void uploadOptions(); //обновить данные о серии
    void setAllSeries();
    void setAllCharts();
};
#endif // MAINCHART_H
