#ifndef UWATCHTAB_H
#define UWATCHTAB_H

#include <iostream>

#include <QWidget>
#include <QString>
#include <QVector>
#include <QSplitter>

#include "UWatchChart.h"
#include "UVisualControllerWidget.h"
#include "UComponentPropertySelectionWidget.h"

using namespace QtCharts;

namespace Ui {
class UWatchTab;
}

class UWatchChart;
//////////////////////////////////////////////////////////////////////////////
// Класс, реализующий одну вкладку с одним или несколькими графиками
//////////////////////////////////////////////////////////////////////////////

class UWatchTab : public UVisualControllerWidget
{
    Q_OBJECT

public:
    explicit UWatchTab(QWidget *parent = nullptr, RDK::UApplication* app = NULL);
    ~UWatchTab();

    ///создание расположений графиков
    void createSingleLayout();
    void createRowLayout(int rowNumber);
    void createColLayout(int colNumber);
    void createGridLayout(int rowNumber, int colNumber);

    /// 0 - SingleLayout
    /// 1 - ColLayout
    /// 2 - RowLayout
    /// 3 - GridLayout
    int layoutMode;


    UWatchChart *getChart(int index);
    int countGraphs();

    ///вызывется окно для выбора источника данных
    void createSelectionDialog(int chartIndex);

    ///частота обновления графика, мс
    int UpdateIntervalMs = 30;
    void saveUpdateInterval(int newInterval);

    //узнать количество строк и столбцов
    int getColNumber();
    int getRowNumber();

private:
    void createGraph();
    void createSplitterGrid(int rowNumber);
    void deleteGraph(int index);
    void deleteAllGraph();

    int tabColNumber=0;
    int tabRowNumber=0;

    ///вектор графиков на одной вкладке
    QVector <UWatchChart*> graph;


    QSplitter *colSplitter;              //colSplitter
    QVector <QSplitter*> rowSplitter;    // |----..---rowSplitter[0]
                                         // |----..---
                                         // |----..---
                                         // ...
                                         // |----..---rowSplitter[n]

    Ui::UWatchTab *ui;

    ///Обновляет графики
    ///Для каждого графика - обращаемся к ядру, берем матрицу по заданному источнику данных
    ///Ее данные положим на этот график
    virtual void AUpdateInterface();

public slots:
    void createSelectionDialogSlot(int index);

};

#endif // UWATCHTAB_H
