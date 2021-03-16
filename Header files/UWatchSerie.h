#ifndef UWATCHSERIE_H
#define UWATCHSERIE_H

#include <QLineSeries>
#include <QString>

class UWatchSerie: public QtCharts::QLineSeries
{
public:
    UWatchSerie();
    //~UWatchSerie();

    //������ �� ��������� ������
    int indexChannel;      //������ ������
    QString nameComponent; //��� ����������
    QString nameProperty;  //��� ��������
    QString typeProperty;  //��� ����������
    //���������� �������� �������
    int Jx;
    int Jy;
};

#endif // UWATCHSERIE_H
