#include "UWatchSeriesOption.h"
#include "ui_UWatchSeriesOption.h"

#include "UWatch.h"


UWatchSeriesOption::UWatchSeriesOption(QWidget *parent) :
   QDialog(parent),
    ui(new Ui::UWatchSeriesOption)
{
    ui->setupUi(this);
    Watch = dynamic_cast<UWatch*>(parent);

    loadGraphsName();
}

UWatchSeriesOption::~UWatchSeriesOption()
{
    delete ui;
}

void UWatchSeriesOption::on_okButton_clicked()
{
   saveParemeters();
   close();
   destroy();
   delete this;
}

void UWatchSeriesOption::on_applyButton_clicked()
{
    saveParemeters();
    int currentChartIndex = ui->graphsList->currentRow();
    updateGraphsSeries(currentChartIndex);
}

void UWatchSeriesOption::on_closeButton_clicked()
{
    close();
    destroy();
    delete this;
}

void UWatchSeriesOption::on_addSerieButton_clicked()
{
    int currentChartIndex = ui->graphsList->currentRow();

    //вызываем окно для добавления новой серии и обновляем лист серий для данного графика
    Watch->getCurrentTab()->createSelectionDialog(currentChartIndex);
    updateGraphsSeries(currentChartIndex);
}

void UWatchSeriesOption::on_removeSerieButton_clicked()
{
    //удаляем в выбранном графике выбранную среию
    int currentChartIndex = ui->graphsList->currentRow();
    int currentSerieIndex = ui->graphsSeriesList->currentRow();
    Watch->getCurrentTab()->getChart(currentChartIndex)->deleteSerie(currentSerieIndex);
    updateGraphsSeries(currentChartIndex);
}

void UWatchSeriesOption::on_graphsList_currentRowChanged(int currentRow)
{
    updateGraphsSeries(currentRow);
}

void UWatchSeriesOption::on_graphsSeriesList_currentRowChanged(int currentRow)
{
    updateSeriesProperties(currentRow);
}

void UWatchSeriesOption::loadGraphsName()
{
    //загружаем имена графиков
    for (int i = 0; i < Watch->getCurrentTab()->countGraphs(); i++)
    {
        ui->graphsList->addItem(Watch->getCurrentTab()->getChart(i)->getChartTitle());
    }
    if (ui->graphsList->count()) ui->graphsList->setCurrentRow(0);
}

void UWatchSeriesOption::updateGraphsSeries(int currentChartIndex)
{
    //загружаем имена серий выбранного графика
    ui->graphsSeriesList->clear();
    for (int i = 0; i < Watch->getCurrentTab()->getChart(currentChartIndex)->countSeries(); i++)
    {
        ui->graphsSeriesList->addItem(Watch->getCurrentTab()->getChart(currentChartIndex)->getSerieName(i));
    }
    if (ui->graphsSeriesList->count())  ui->graphsSeriesList->setCurrentRow(0);
}

void UWatchSeriesOption::updateSeriesProperties(int currentSerieIndex)
{
    //загружаем свойства выбранной серии
    if(currentSerieIndex == -1) return;
    int currentChartIndex = ui->graphsList->currentRow();

    updateSeriesNameProp(currentChartIndex, currentSerieIndex);
    updateSeriesColorProp(currentChartIndex, currentSerieIndex);
    updateSeriesWidthProp(currentChartIndex, currentSerieIndex);
    updateSeriesLineTypeProp(currentChartIndex, currentSerieIndex);
}

void UWatchSeriesOption::updateSeriesNameProp(int currentChartIndex, int currentSerieIndex)
{
    QString name = Watch->getCurrentTab()->getChart(currentChartIndex)->getSerieName(currentSerieIndex);
    ui->serieName->setText(name);
}

void UWatchSeriesOption::updateSeriesColorProp(int currentChartIndex, int currentSerieIndex)
{
    const QColor color = Watch->getCurrentTab()->getChart(currentChartIndex)->getSerieColor(currentSerieIndex);
    for (int i = 0; i < 15; ++i)
        if (color == defaultColors[i])
        {
            ui->serieColor->setCurrentIndex(i);
            return;
        }
    ui->serieColor->setCurrentIndex(16);
}

void UWatchSeriesOption::updateSeriesWidthProp(int currentChartIndex, int currentSerieIndex)
{
    int width = Watch->getCurrentTab()->getChart(currentChartIndex)->getSerieWidth(currentSerieIndex);
    if (width<=6) ui->serieLneWidth->setCurrentIndex(width-1);
    else ui->serieLneWidth->setCurrentIndex(5); //6+
}

void UWatchSeriesOption::updateSeriesLineTypeProp(int currentChartIndex, int currentSerieIndex)
{
     Qt::PenStyle lineStyle =  Watch->getCurrentTab()->getChart(currentChartIndex)->getSerieLineType(currentSerieIndex);
     for (int i = 0; i < 4; ++i)
        if (lineStyle == defaultLineStyle[i]) ui->serieLineType->setCurrentIndex(i);
}

void UWatchSeriesOption::saveParemeters()
{
    //сохранение параметров
    int currentChartIndex = ui->graphsList->currentRow();
    int currentSerieIndex = ui->graphsSeriesList->currentRow();

    if(currentChartIndex == -1) return;
    if(currentSerieIndex == -1) return;

    QString name = ui->serieName->text();
    int colorIndex = ui->serieColor->currentIndex();
    int width = ui->serieLneWidth->currentIndex();
    int lineTypeIndex = ui->serieLineType->currentIndex();

    Watch->getCurrentTab()->getChart(currentChartIndex)->setSerieName(currentSerieIndex, name);
    Watch->getCurrentTab()->getChart(currentChartIndex)->setSerieStyle(currentSerieIndex, defaultColors[colorIndex], width+1, defaultLineStyle[lineTypeIndex]);

}
