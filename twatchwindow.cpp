#include "twatchwindow.h"
#include "ui_twatchwindow.h"

TWatchWindow::TWatchWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TWatchWindow)
{
    ui->setupUi(this);
    grid = new QGridLayout();
    ui->verticalLayout->addLayout(grid);
}

TWatchWindow::~TWatchWindow()
{
    delete ui;
}

void TWatchWindow::createSingleLayout()                                                //вспомогательная ф-ия создания одиночного графика
{
    updateGrid();
    ui->verticalLayout->addLayout(grid);
    createChart("одиночный график");
    grid->addWidget(chartView.last(), 1, 1);
}

void TWatchWindow::createRowLayout(int rowNumber)                                      //вспомогательная ф-ия создания горизонтальных графиков
{
    updateGrid();
    for(int i=1; i <= rowNumber;i++)
    {
        createChart(QString("график горизонтальный №%1").arg(QString::number(i)));
        grid->addWidget(chartView.last(),i, 1);
    }
}

void TWatchWindow::createColLayout(int colNumber)                                      //вспомогательная ф-ия создания вертикальных графиков
{
    updateGrid();
    for(int i=1; i <= colNumber;i++)
    {
        createChart(QString("график вертикальный №%1").arg(QString::number(i)));
        grid->addWidget(chartView.last(),1, i);
    }
}

void TWatchWindow::createGridLayout(int rowNumber, int colNumber)                      //вспомогательная ф-ия создания сетки графиков
{
    updateGrid();
    for(int i=1; i <= rowNumber;i++)
        for(int j=1; j <= colNumber; j++)
        {
            createChart(QString("график сетка №%1").arg(QString::number((i-1)*colNumber+j)));
            grid->addWidget(this->chartView.last(),i, j);
        }

}

void TWatchWindow::updateGrid()                                                        //создается сетка для графиков (работает через ж. но как то работает)
{
    if (!chartView.isEmpty()) chartView.clear();
    ui->verticalLayout->removeItem(grid);
    delete grid;
    ui->verticalLayout->update();
    this->repaint();
    grid = new QGridLayout();
    //grid->setSpacing(0);
    grid->setVerticalSpacing(0);
    grid->setHorizontalSpacing(0);
    ui->verticalLayout->addLayout(grid);
}

void TWatchWindow::createChart(QString chartName)                                      //создается график с дефолтными настрофками
{
    this->chartView.push_back(new TWatchChart());

    this->chartView.last()->chart->legend()->setVisible(true);
    this->chartView.last()->chart->legend()->setAlignment(Qt::AlignBottom);
    this->chartView.last()->chart->setTitle(chartName);
    this->chartView.last()->chartView->setChart(this->chartView.last()->chart);
}

void TWatchWindow::addSeriesToChart(QLineSeries* series, TWatchChart* chartView)       //добавить серию в график (хз зачем это)
{
    chartView->chart->addSeries(series);
    chartView->chart->createDefaultAxes();
}

void TWatchWindow::updateLayout(QString layoutMthod, int rowNumber, int colNumber)     //создаем расположение в зависимости от способа
{
    if(layoutMthod == "Один график") createSingleLayout();
    else if(layoutMthod == "Горизонтально") createRowLayout(rowNumber);
    else if(layoutMthod == "Вертикально") createColLayout(colNumber);
    else if(layoutMthod == "Сетка") createGridLayout(rowNumber,colNumber);
}



