#ifndef UWATCHCHART_H
#define UWATCHCHART_H

#include <QWidget>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QMenu>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>

#include <QWheelEvent>
#include <QKeyEvent>

#include <QString>
#include <QVector>

#include <QValueAxis>
#include "UWatchSerie.h"
#include <QChart>
#include <QChartView>
//#include <UWatchTab.h>


namespace Ui {
class UWatchChart;
}
using namespace QtCharts;

class UWatchTab;
//////////////////////////////////////////////////////////////////////////////
// Один отдельно взятый график с одной или несколькими сериями данных
//////////////////////////////////////////////////////////////////////////////

class UWatchChart : public QWidget
{
    Q_OBJECT

public:
    explicit UWatchChart(QWidget *parent = nullptr);
    ~UWatchChart();


    //get'ы
    QString getChartTitle();

    QString getAxisXName();
    QString getAxisYName();
    double  getAxisXmin();
    double  getAxisXmax();
    double  getAxisYmin();
    double  getAxisYmax();

    UWatchSerie *getSerie(int index); //
    QString      getSerieName(int serieIndex);
    QColor       getSerieColor(int serieIndex);
    int          getSerieWidth(int serieIndex);
    Qt::PenStyle getSerieLineType(int serieIndex);



    //set'ы
    void setChartTitle(QString title);
    void setChartIndex(int index);

    void setAxisXname(QString name);
    void setAxisYname(QString name);
    void setAxisXmin(double value);
    void setAxisXmax(double value);
    void setAxisYmin(double value);
    void setAxisYmax(double value);

    void setSerieName(int serieIndex, QString name);
    void setSerieColor(int serieIndex, int colorIndex);
    void setSerieLineType(int serieIndex, Qt::PenStyle lineType);
    void setSerieWidth(int serieIndex, int width);
    void setSerieStyle(int serieIndex, QColor color, int width, Qt::PenStyle lineType);

    //действия с сериями
    void createSerie(int channelIndex, const QString componentName,
                     const QString propertyName, const QString type, int jx, int jy);
    void deleteSerie(int serieIndex);
    void addDataToSerie(int serieIndex, double x, double y);
    int  countSeries();

    //работа с динамикой осей
    int axisXrange = 5;
    bool isAxisXtrackable = true;   //будет ли "поле зрения" бежать за временем
    bool isAxisYzoomable = true;    //зум по оси У (ctrl+крокрутка)
    bool isAxisYscrollable = true;  //скролл оси У

    //доступные цвета для серий
    const QColor defaultColors[15]={Qt::red, Qt::darkRed, Qt::yellow, Qt::darkYellow, Qt::green, Qt::darkGreen, Qt::cyan, Qt::darkCyan,
                                   Qt::blue, Qt::darkBlue, Qt::magenta,Qt::darkMagenta, Qt::gray, Qt::darkGray, Qt::black};

private:
    Ui::UWatchChart *ui;

    //все график, оси, скороллбар и их расположение
    QVBoxLayout *verticalLayout;
    QScrollBar *horizontalScrolBar;
    QChartView *chartView;
    QChart *chart;
    QValueAxis *axisX;
    QValueAxis *axisY;


    //серии + данные об источнике данных
    QVector <UWatchSerie*> series;

    bool isCtrlPressed = false;
    int chartIndex; //что бы график знал какой он по счету в векторе графиков


private slots:
    //скролл и зум по оси Y
    void wheelEvent(QWheelEvent * event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void slotCustomMenuRequested(QPoint pos);
    void addSeriesSlot();
    void chartOptionSlot();
    void saveToJpegSlot();
signals:
    void addSerieSignal(int someIndex);
};

#endif // UWATCHCHART_H
