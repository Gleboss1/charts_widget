#include "twatchchart.h"
#include "ui_twatchchart.h"

TWatchChart::TWatchChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TWatchChart)
{
    ui->setupUi(this);
    chart = new QChart();
    chartView = new QChartView();
    ui->verticalLayout->addWidget(chartView);
}

TWatchChart::~TWatchChart()
{
    delete ui;
}
