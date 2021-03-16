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
// �����, ����������� ���� ������� � ����� ��� ����������� ���������
//////////////////////////////////////////////////////////////////////////////

class UWatchTab : public UVisualControllerWidget
{
    Q_OBJECT

public:
    explicit UWatchTab(QWidget *parent = nullptr, RDK::UApplication* app = NULL);
    ~UWatchTab();

    ///�������� ������������ ��������
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

    ///��������� ���� ��� ������ ��������� ������
    void createSelectionDialog(int chartIndex);

    ///������� ���������� �������, ��
    int UpdateIntervalMs = 30;
    void saveUpdateInterval(int newInterval);

    //������ ���������� ����� � ��������
    int getColNumber();
    int getRowNumber();

private:
    void createGraph();
    void createSplitterGrid(int rowNumber);
    void deleteGraph(int index);
    void deleteAllGraph();

    int tabColNumber=0;
    int tabRowNumber=0;

    ///������ �������� �� ����� �������
    QVector <UWatchChart*> graph;


    QSplitter *colSplitter;              //colSplitter
    QVector <QSplitter*> rowSplitter;    // |----..---rowSplitter[0]
                                         // |----..---
                                         // |----..---
                                         // ...
                                         // |----..---rowSplitter[n]

    Ui::UWatchTab *ui;

    ///��������� �������
    ///��� ������� ������� - ���������� � ����, ����� ������� �� ��������� ��������� ������
    ///�� ������ ������� �� ���� ������
    virtual void AUpdateInterface();

public slots:
    void createSelectionDialogSlot(int index);

};

#endif // UWATCHTAB_H
