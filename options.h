#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QVector>
#include <QString>
#include <QMessageBox>

//классы графиков
#include "twatchwindow.h"
#include "twatchchart.h"
#include "mainchart.h"


//библиотеки QtCharts
#include <QValueAxis>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QtCharts/QLegend>
#include <QPen>
#include <QColor>


using namespace QtCharts;
namespace Ui {
class Options;
}

class TWatchWindow;

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();

    Ui::Options *ui;
    QVector <QLineSeries*> allSeries;           //копия серий
    QVector <TWatchChart*> charts;              //копия графиков



    void updateSeriesName();
    void updateCurrentSeriesName(int index);
    void updateChartsData();
private:
    void updateSeriesData(QString name);
    void updateSeriesColor(int index);
    void updateSeriesLineType(int index);

    int countSerieIndex(QString serieName);
    void updateChartSeriesList(int chartIndex);

    void saveSeriesData();
    void saveChartsData();

    void saveSeriesColor(QPen *pen);
    void saveSeriesLineType(QPen *pen);


private slots:
    void on_currentSeriesChoise_CB_currentTextChanged(const QString &arg1);
    void on_okButton_clicked();
    void on_applyButton_clicked();
    void on_closeButton_clicked();
    void on_chartLayot_CB_currentTextChanged(const QString &arg1);
    void on_createLayot_button_clicked();
    void on_addSerieToChart_clicked();
    void on_removeSerieFromChart_clicked();
    void on_allCharts_currentRowChanged(int currentRow);

signals:
    void saveData();
    void saveLayout(QString layoutMthod, int rowNumber, int colNumber);
    void getAllSeries();
    void getAllCharts();
    void getChartsSeries();
};

#endif // OPTIONS_H
