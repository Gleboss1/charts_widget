#include "mainchart.h"
#include "ui_mainchart.h"

#include <math.h>

MainChart::MainChart(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainChart)
{
    ui->setupUi(this);

    //создаем первую вкладку
    tab.push_back(new TWatchWindow());
    ui->tabWidget->addTab(tab.last(), "Вкладка 1");

    //создаем 3 серии (синусоиды)
    createRandomSeries(3);
}

MainChart::~MainChart()
{
    delete ui;
}

void MainChart::setAllSeries()                              //слот. копирует серии в опции
{
    option_wnd->allSeries = series;
}

void MainChart::setAllCharts()                              //слот. копирует графики в опции
{
    option_wnd->charts = tab[currentTabIndex]->chartView;
}

void MainChart::on_actionOptions_triggered()                //открытие окна опций
{
    //создаем окно опций, копируем туда все серии и делаем конекты между окном опций и главным виджетом
    option_wnd = new Options(this);

    // /*эта часть должна быть в конструкторе опций, но там она не работает!
    // копируем туда все серии и все данные о графиках
    option_wnd->allSeries = series;
    option_wnd->charts = tab[currentTabIndex]->chartView;
    option_wnd->updateSeriesName();
    option_wnd->updateChartsData();
    //*/

    connect(this->option_wnd, SIGNAL(saveData()), this, SLOT(uploadOptions()));
    connect(this->option_wnd, SIGNAL(saveLayout(QString, int, int)), tab[currentTabIndex], SLOT(updateLayout(QString, int, int)));

    //connect(this->option_wnd, SIGNAL(getAllSeries()), this, SLOT(setAllSeries()));
    connect(this->option_wnd, SIGNAL(getAllCharts()), this, SLOT(setAllCharts()));
}

void MainChart::on_actionCreate_owm_window_triggered()      //открытие графика в новом окне
{
    //chart_wnd = new ChartWindow();
    //chart_wnd->show();
}

void MainChart::on_tabWidget_tabCloseRequested(int index)   //закрытие вкладки
{
    ui->tabWidget->removeTab(index);//поправить
    tab.remove(index);
}

void MainChart::on_actionAdd_new_chart_triggered()          //создание новой вкладки
{
    tab.push_back(new TWatchWindow());
    ui->tabWidget->addTab(tab.last(), QString("Вкладка %0").arg(ui->tabWidget->count()+1));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainChart::createRandomSeries(int amount)              //создание нескольких рандомных серий-синусоид
{
    for(int i=0;i<amount; i++)
    {
        series.push_back(new QLineSeries());
        QPen *pen = new QPen();

        pen->setWidth(QRandomGenerator::global()->bounded(1, 5));

        if(i == 0 ) pen->setColor(QColor::fromRgb(255,0,0));
        else if(i == 1 ) pen->setColor(QColor::fromRgb(0,255,0));
        else if(i == 2 ) pen->setColor(QColor::fromRgb(0,0,255));

        if(i == 0 ) pen->setStyle(Qt::SolidLine);
        else if(i == 1 ) pen->setStyle(Qt::DashLine);
        else if(i == 2 ) pen->setStyle(Qt::DashDotLine);
        series.last()->setPen(*pen);

        series.last()->setName(QString("sinus %1").arg(i));

        for(double j=0; j<10; j+=0.01)
             series.last()->append(j, (i+1)*sin((i+1)*j));
    }
}

void MainChart::uploadOptions()                             //выгружаем измененные серии и графики из опций
{
    //выгружаем данные о сериях и графиках с измененными опциями
    series = option_wnd->allSeries;
    tab[currentTabIndex]->chartView = option_wnd->charts;

    //перерисовываем графики
    for (int i =0; i < tab[currentTabIndex]->chartView.count(); i++)
        tab[currentTabIndex]->chartView[i]->chart->update();
}

void MainChart::updateSeriesName(int index)                 //что-то странное...
{
    series[index]->setName(option_wnd->ui->currentSeriesName->text());
    option_wnd->updateCurrentSeriesName(index);
}

void MainChart::on_tabWidget_currentChanged(int index)      //изменяем индекс текущей вкладки
{
    currentTabIndex = index;
}
