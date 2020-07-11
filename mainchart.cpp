#include "mainchart.h"
#include "ui_mainchart.h"




#include <math.h>

MainChart::MainChart(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainChart)
{
    ui->setupUi(this);

    tab.push_back(new TWatchWindow());

    ui->tabWidget->addTab(tab.last(), "tab 1");

    //создаем 2 серии (синусоиды)
    createRandomSeries(2);

 // Создаём график и добаляем
    //tab.last()->chartView.push_back(new TWatchChart()); //потому что создаем один график по дефолту
    tab.last()->chartView.last()->chart->addSeries(series.last()->series);
    //tab.last()->chartView.last()->chart->addSeries(series.first()->series);
    tab.last()->chartView.last()->chart->legend()->setVisible(true);
    tab.last()->chartView.last()->chart->legend()->setAlignment(Qt::AlignBottom);
    tab.last()->chartView.last()->chart->createDefaultAxes();
    tab.last()->chartView.last()->chart->setTitle("это дефолтный график");
    tab.last()->chartView.last()->chartView->setChart(tab.last()->chartView.last()->chart);

 //добавление нового графика в текущий tab
    tab.last()->chartView.push_back(new TWatchChart());
    //tab.last()->chartView.last()->chart->addSeries(series.last()->series);
    tab.last()->chartView.last()->chart->addSeries(series.first()->series);
    tab.last()->chartView.last()->chart->legend()->setVisible(true);
    tab.last()->chartView.last()->chart->legend()->setAlignment(Qt::AlignTop);
    tab.last()->chartView.last()->chart->createDefaultAxes();
    tab.last()->chartView.last()->chart->setTitle("Синусоида №2");
    tab.last()->chartView.last()->chartView->setChart(tab.last()->chartView.last()->chart);

    tab.last()->grid->addWidget(tab.last()->chartView.last(),2, 1);

/*
   // Настройка осей графика
    chart1->axisX->setTitleText("х");
    chart1->axisX->setTickCount(1);
    chart1->axisX->setRange(0, 100);
    chart1->chart->addAxis( chart1->axisX, Qt::AlignBottom);
    sin1->series->attachAxis( chart1->axisX);

    chart1->axisY->setTitleText("y");
    chart1->axisY->setLabelFormat("%g");
    chart1->axisY->setTickCount(1);
    chart1->axisY->setRange(-2, 2);
    chart1->chart->addAxis( chart1->axisY, Qt::AlignLeft);
    sin1->series->attachAxis( chart1->axisY);

    */
    // Устанавливаем график в представление
   // tab.last()->chartView.last()->chartView->setChart(tab.last()->chartView.last()->chart);
   // tab.last()->chartView->


}

MainChart::~MainChart()
{
    delete ui;
}


void MainChart::on_actionOptions_triggered() //открытие окна опций
{
    //создаем окно опций, копируем туда все серии и делаем связь между окном опций и главным виджетом
    option_wnd = new Options(this);
    option_wnd->seriesOption = series;
    option_wnd->updateSeriesName();
    option_wnd->show();
    connect(this->option_wnd, SIGNAL(saveData()), this, SLOT(updateOptions()));
    connect(this->option_wnd, SIGNAL(saveLayout(QString, int, int)), tab.last(), SLOT(updateLayout(QString, int, int)));
}

void MainChart::on_actionCreate_owm_window_triggered() //открытие графика в новом окне
{
    //chart_wnd = new ChartWindow();
    //chart_wnd->show();
}



void MainChart::on_tabWidget_tabCloseRequested(int index) //закрытие вкладки
{
    ui->tabWidget->removeTab(index);//поправить
}

void MainChart::on_actionAdd_new_chart_triggered() //создание новой вкладки
{
    tab.push_back(new TWatchWindow());
    ui->tabWidget->addTab(tab.last(), QString("Tab %0").arg(ui->tabWidget->count()+1));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainChart::createRandomSeries(int amount)
{
    for(int i=0;i<amount; i++)
    {
        series.push_back(new TWatchSeries());
        if(i == 0 ) series.last()->pen.setColor(QColor::fromRgb(255,0,0));
        else if(i == 1 ) series.last()->pen.setColor(QColor::fromRgb(0,0,255));
        series.last()->pen.setWidth(QRandomGenerator::global()->bounded(1, 5));
        if(i == 0 ) series.last()->pen.setStyle(Qt::SolidLine);
        else if(i == 1 ) series.last()->pen.setStyle(Qt::DashLine);
        series.last()->series->setPen(series.last()->pen);
        series.last()->series->setName(QString("sinus %1").arg(i));

        //double k = QRandomGenerator::global()->bounded(1, 2);
        for(double j=0; j<10; j+=0.01)
             series.last()->series->append(j, (i+1)*sin((i+1)*j));
    }
}

void MainChart::updateOptions()
{
    //поиск изменяемой серии по имени
    QString name = option_wnd->ui->currentSeriesChoise_CB->currentText();
    int index;
    for (index=0;index<series.count();index++)
        if (name == series[index]->series->name()) break;

    //меняем свойства выбранной серии
    series[index]->pen.setWidth(option_wnd->ui->seriesWidth->text().toInt());

    series[index]->series->setName(option_wnd->ui->currentSeriesName->text());
    //if (series[index]->series->name() != option_wnd->ui->currentSeriesName->text()) updateSeriesName(index);
    updateSeriesColor(index);
    updateSeriesLineType(index);
    series[index]->series->setPen(series[index]->pen);

    //перерисовываем график
    tab.last()->chartView.last()->chart->update();

    //тут костыль (передалеть)
    option_wnd->seriesOption[index] = series[index];
    option_wnd->updateCurrentSeriesName(index);
    //option_wnd->updateSeriesName();



}


void MainChart::updateSeriesColor(int index)
{
    QString color = option_wnd->ui->currentSeriesColor_CB->currentText();

    if(color == "красный")series[index]->pen.setColor(QColor::fromRgb(255,0,0));
    else if(color == "зеленый")series[index]->pen.setColor(QColor::fromRgb(0,255,0));
    else if(color == "синий")series[index]->pen.setColor(QColor::fromRgb(0,0,255));
    else if(color == "черный")series[index]->pen.setColor(QColor::fromRgb(0,0,0));
    else if(color == "оранжевый")series[index]->pen.setColor(QColor::fromRgb(255,180,2));
}

void MainChart::updateSeriesLineType(int index)
{
    QString lineStyle = option_wnd->ui->currentSeriesLineType_CB->currentText();

    if (lineStyle == "сплошная") series[index]->pen.setStyle(Qt::SolidLine);
    else if (lineStyle == "точки") series[index]->pen.setStyle(Qt::DotLine);
    else if (lineStyle == "пунктир") series[index]->pen.setStyle(Qt::DashLine);
    else if (lineStyle == "пунктир с точкой") series[index]->pen.setStyle(Qt::DashDotLine);
}

void MainChart::updateSeriesName(int index)
{
    series[index]->series->setName(option_wnd->ui->currentSeriesName->text());
    option_wnd->updateCurrentSeriesName(index);
}
