#ifndef UWATCHSERIE_H
#define UWATCHSERIE_H

#include <QLineSeries>
#include <QString>

class UWatchSerie: public QtCharts::QLineSeries
{
public:
    UWatchSerie();
    //~UWatchSerie();

    //данные об источнике данных
    int indexChannel;      //Индекс канала
    QString nameComponent; //Имя компонента
    QString nameProperty;  //Имя свойства
    QString typeProperty;  //Тип компонента
    //координаты элемента матрицы
    int Jx;
    int Jy;
};

#endif // UWATCHSERIE_H
