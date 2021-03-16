#ifndef UWATCHSERIESOPTION_H
#define UWATCHSERIESOPTION_H

#include <QDialog>
#include <QStyle>
#include <QPen>
#include <QColor>
#include <QChart>

#include "UVisualControllerWidget.h"



namespace Ui {
class UWatchSeriesOption;
}
using namespace QtCharts;

class UWatch;

//////////////////////////////////////////////////////////////////////
// ���� �������� ������ ������ � ����� ��� ����������� ������� ������
//////////////////////////////////////////////////////////////////////

class UWatchSeriesOption : public QDialog
{
    Q_OBJECT

public:
    explicit UWatchSeriesOption(QWidget *parent = nullptr);
    ~UWatchSeriesOption();


private slots:
    void on_okButton_clicked();
    void on_applyButton_clicked();
    void on_closeButton_clicked();
    void on_addSerieButton_clicked();
    void on_removeSerieButton_clicked();
    void on_graphsList_currentRowChanged(int currentRow);
    void on_graphsSeriesList_currentRowChanged(int currentRow);

private:
    Ui::UWatchSeriesOption *ui;
    UWatch *Watch;

    void loadGraphsName();//��������� ����� ��������
    void updateGraphsSeries(int currentChartIndex); //�������� ����� ����� ���������� ������� � ����� �����

    void updateSeriesProperties(int currentSerieIndex); //�������� �������� ��� ��������� �����
    void updateSeriesNameProp(int currentChartIndex, int currentSerieIndex);
    void updateSeriesColorProp(int currentChartIndex, int currentSerieIndex);
    void updateSeriesWidthProp(int currentChartIndex, int currentSerieIndex);
    void updateSeriesLineTypeProp(int currentChartIndex,int currentSerieIndex);

    //��������� ���������
    void saveParemeters();

    //������� ������ � ����� �����
    const QColor defaultColors[15]={Qt::red, Qt::darkRed, Qt::yellow, Qt::darkYellow, Qt::green,
                                    Qt::darkGreen, Qt::cyan, Qt::darkCyan, Qt::blue, Qt::darkBlue,
                                    Qt::magenta,Qt::darkMagenta, Qt::gray, Qt::darkGray, Qt::black};

    const Qt::PenStyle defaultLineStyle[4] = {Qt::SolidLine, Qt::DotLine, Qt::DashLine, Qt::DashDotLine};
};

#endif // UWATCHSERIESOPTION_H
