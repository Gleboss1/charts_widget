#ifndef UWATCHCHARTOPTION_H
#define UWATCHCHARTOPTION_H

#include <QDialog>



namespace Ui {
class UWatchChartOption;
}

class UWatch;

//////////////////////////////////////////////////////////////////////////////
// ���������� ���� ���  ��������� ������������ �������� � �� ����������
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

    void updateChartList(); //�������� ����� � ���� ��������
    void updateLayoutBox();
    void updateParameters(int chartIndex);//�������� ��������� ���������� � ����� �������
    void createLayout(); //������� ����� ������������
    void saveParameters(); //��������� ���������


};

#endif // UWATCHCHARTOPTION_H
