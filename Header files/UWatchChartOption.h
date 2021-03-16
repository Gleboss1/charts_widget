#ifndef UWATCHCHARTOPTION_H
#define UWATCHCHARTOPTION_H

#include <QDialog>



namespace Ui {
class UWatchChartOption;
}

class UWatch;

//////////////////////////////////////////////////////////////////////////////
// Диалоговое окно для  настройки расположения графиков и их параметров
//////////////////////////////////////////////////////////////////////////////

class UWatchChartOption : public QDialog
{
    Q_OBJECT

public:
    explicit UWatchChartOption(QWidget *parent = nullptr);
    ~UWatchChartOption();

private slots:
    void on_chartLayot_CB_currentIndexChanged(int index);
    void on_createLayot_button_clicked();
    void on_allChartsList_currentRowChanged(int currentRow);
    void on_okButton_clicked();
    void on_applyButton_clicked();
    void on_closeButoon_clicked();

    void on_axisXtrackCB_stateChanged(int arg1);

private:
    Ui::UWatchChartOption *ui;
    UWatch *Watch;

    void updateChartList(); //обновить имена в лист графиков
    void updateLayoutBox();
    void updateParameters(int chartIndex);//обновить параметры выбранного в листе графика
    void createLayout(); //создать новое расположение
    void saveParameters(); //сохранить изменения


};

#endif // UWATCHCHARTOPTION_H
