#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QWidget>

namespace Ui {
class ChartWindow;
}

class ChartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWindow(QWidget *parent = nullptr);
    ~ChartWindow();


private:
    Ui::ChartWindow *ui;
};

#endif // CHARTWINDOW_H
