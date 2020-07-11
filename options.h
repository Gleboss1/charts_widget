#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QVector>
#include <QString>

//классы графиков
#include "twatchwindow.h"
#include "twatchchart.h"
#include "twatchseries.h"
#include "mainchart.h"


//библиотеки QtCharts
#include <QValueAxis>
#include <QLineSeries>
#include <QValueAxis>
#include <QChart>
#include <QChartView>
#include <QtCharts/QLegend>
#include <QPen>
#include <QColor>



using namespace QtCharts;


namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();
    Ui::Options *ui;
    QVector <TWatchSeries*> seriesOption;
    void updateSeriesName();
    void updateCurrentSeriesName(int index);
private:
    void updateSeriesData(QString name);
    void updateSeriesColor(int index);
    void updateSeriesLineType(int index);


private slots:
    void on_currentSeriesChoise_CB_currentTextChanged(const QString &arg1);
    void on_okButton_clicked();
    void on_applyButton_clicked();

    void on_closeButton_clicked();

    void on_chartLayot_CB_currentTextChanged(const QString &arg1);

    void on_createLayot_button_clicked();

signals:
    void saveData();
    void saveLayout(QString layoutMthod, int rowNumber, int colNumber);

};


#endif // OPTIONS_H
