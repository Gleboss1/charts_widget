#include "twatchwindow.h"
#include "ui_twatchwindow.h"

TWatchWindow::TWatchWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TWatchWindow)
{
    ui->setupUi(this);
    grid = new QGridLayout();
    ui->centralWidget->addLayout(grid);
    this->chartView.push_back(new TWatchChart());
    grid->addWidget(this->chartView.last(),1, 1);
}

TWatchWindow::~TWatchWindow()
{
    delete ui;
}

void TWatchWindow::createSingleLayout()
{
    grid->removeWidget(chartView.last());
    grid->removeWidget(chartView.first());
    /*ui->centralWidget->removeItem(grid);
    delete grid;
    grid = new QGridLayout();
    ui->centralWidget->addLayout(grid);*/
    grid->addWidget(this->chartView.first(),1, 1);
    //this->update();
    //this->chartView.removeLast();
}

void TWatchWindow::createRowLayout(int rowNumber)
{
    delete grid;
    grid = new QGridLayout();
    ui->centralWidget->addLayout(grid);
    for(int i=1; i <= rowNumber;i++)
        grid->addWidget(this->chartView.last(),i, 1);
}

void TWatchWindow::createColLayout(int colNumber)
{
    delete grid;
    grid = new QGridLayout();
    ui->centralWidget->addLayout(grid);
    for(int i=1; i <= colNumber;i++)
        grid->addWidget(this->chartView.last(),1, i);
}

void TWatchWindow::createGridLayout(int rowNumber, int colNumber)
{
    delete grid;
    grid = new QGridLayout();
    ui->centralWidget->addLayout(grid);
    for(int i=1; i <= rowNumber;i++)
        for(int j=1; j <= colNumber; j++)
        grid->addWidget(this->chartView.last(),i, j);
}

void TWatchWindow::updateLayout(QString layoutMthod, int rowNumber, int colNumber)
{
    if(layoutMthod == "Один график") createSingleLayout();
    else if(layoutMthod == "Вертикально") createRowLayout(rowNumber);
    else if(layoutMthod == "Горизонтально") createColLayout(colNumber);
    else if(layoutMthod == "Сетка") createGridLayout(rowNumber,colNumber);
}

