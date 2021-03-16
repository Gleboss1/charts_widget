#ifndef UWATCH_H
#define UWATCH_H

#include <QMainWindow>
#include <QVector>
#include <QMessageBox>
#include <QDir>
#include <QDateTime>

#include "UWatchTab.h"
#include "UWatchChartOption.h"
#include "UWatchSeriesOption.h"


QT_BEGIN_NAMESPACE
namespace Ui { class UWatch; }
QT_END_NAMESPACE

using namespace QtCharts;

//////////////////////////////////////////////////////////////////////////////
// Отдельное окно для отображения графиков по заданному источнику данных
//////////////////////////////////////////////////////////////////////////////


class UWatch : public QMainWindow
{
    Q_OBJECT

public:
    UWatch(QWidget *parent = nullptr);
    ~UWatch();
    UWatchTab *getCurrentTab();

private slots:
    void on_actionCreate_tab_triggered();
    void on_actionSeries_option_triggered();
    void on_actionCharts_option_triggered();
    void on_tabWidget_tabCloseRequested(int index);

    void on_actionTake_screenshot_triggered();

private:
    QVector <UWatchTab*> tab;
    UWatchChartOption *chartOption;
    UWatchSeriesOption *seriesOption;

    void createTab();
    void deleteTab(int index);

    Ui::UWatch *ui;
};
#endif // UWATCH_H
