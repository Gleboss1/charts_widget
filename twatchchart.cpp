#include "twatchchart.h"
#include "ui_twatchchart.h"

#include <QScrollBar>

TWatchChart::TWatchChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TWatchChart)
{
    ui->setupUi(this);
    chart = new QChart();
    chartView = new QChartView();
    ui->verticalLayout->addWidget(chartView);

    QScrollBar *horizontalScrolBar = new QScrollBar(Qt::Horizontal,this);
    ui->verticalLayout->addWidget(horizontalScrolBar);

    axisX = new QValueAxis();
    axisY = new QValueAxis();
}

TWatchChart::~TWatchChart()
{
    delete ui;
}

void TWatchChart::wheelEvent(QWheelEvent *event)
{
    int degrees = event->delta();
    //прокрутка колесиком - сдвиг по оси Y, колесико + Ctrl = масштабирование
    if(isCtrlPressed)
    {
        if(degrees>0)
        {
           if(axisY->min()>0 && axisY->max()>0)axisY->setRange(axisY->min()*0.9,axisY->max()*1.1);
           else if(axisY->min()<0 && axisY->max()>0)axisY->setRange(axisY->min()*1.1,axisY->max()*1.1);
           else if(axisY->min()<0 && axisY->max()<0)axisY->setRange(axisY->min()*1.1,axisY->max()*0.9);
        }
        else
        {
           if(axisY->min()>0 && axisY->max()>0)axisY->setRange(axisY->min()*1.1,axisY->max()*0.9);
           else if(axisY->min()<0 && axisY->max()>0)axisY->setRange(axisY->min()*0.9,axisY->max()*0.9);
           else if(axisY->min()<0 && axisY->max()<0)axisY->setRange(axisY->min()*0.9,axisY->max()*1.1);
        }
    }
    else
    {
        if(degrees>0)
           axisY->setRange(axisY->min()+0.2,axisY->max()+0.2);
        else
           axisY->setRange(axisY->min()-0.2,axisY->max()-0.2);
    }

}

void TWatchChart::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == 16777249) isCtrlPressed = true;
}

void TWatchChart::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == 16777249) isCtrlPressed = false;
}
