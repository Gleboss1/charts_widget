#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    /*не работает((
    emit getAllSeries();
    emit getAllCharts();
    emit getChartsSeries();
    */
    //заполняем все ячейки согласно данным о сериях и графиках
    show();
}

Options::~Options()                                            //закрываем, уничтожаем, освобождаем память
{
    this->close();
    this->destroy();
    delete ui;
}

void Options::updateSeriesData(QString name)                    //Обновляет все поля ввода для серии name
{
    //ищем индекс текущей серии по имени
    int index = countSerieIndex(name);

    //обновляем данные о данной серии
    ui->currentSeriesName->setText(allSeries[index]->name());
    ui->seriesWidth->setText(QString::number(allSeries[index]->pen().width()));
    updateSeriesColor(index);
    updateSeriesLineType(index);
}

void Options::updateSeriesColor(int index)                      //вспомогательная ф-ия обновления цвета
{
   if(allSeries[index]->pen().color() == QColor::fromRgb(255,0,0)) ui->currentSeriesColor_CB->setCurrentText("красный");
   else if(allSeries[index]->pen().color() == QColor::fromRgb(0,255,0)) ui->currentSeriesColor_CB->setCurrentText("зеленый");
   else if(allSeries[index]->pen().color() == QColor::fromRgb(0,0,255)) ui->currentSeriesColor_CB->setCurrentText("синий");
   else if(allSeries[index]->pen().color() == QColor::fromRgb(0,0,0)) ui->currentSeriesColor_CB->setCurrentText("черный");
   else if(allSeries[index]->pen().color() == QColor::fromRgb(255,180,2)) ui->currentSeriesColor_CB->setCurrentText("оранжевый");
}

void Options::updateSeriesLineType(int index)                   //вспомогательная ф-ия обновления типа линии
{
   if(allSeries[index]->pen().style() == Qt::SolidLine) ui->currentSeriesLineType_CB->setCurrentText("сплошная");
   else if(allSeries[index]->pen().style() == Qt::DashLine) ui->currentSeriesLineType_CB->setCurrentText("пунктир");
   else if(allSeries[index]->pen().style() == Qt::DotLine) ui->currentSeriesLineType_CB->setCurrentText("точки");
   else if(allSeries[index]->pen().style() == Qt::DashDotLine) ui->currentSeriesLineType_CB->setCurrentText("пунктир с точкой");
}

int Options::countSerieIndex(QString serieName)                 //ищет номер серии в векторе серий по имени serieName
{
    for (int index=0;index<allSeries.count();index++)
        if (serieName == allSeries[index]->name()) return index;
}

void Options::updateChartSeriesList(int chartIndex)             //обновляет список серий, принадлежащих графику
{
    ui->chartSeries->clear();
    for (int i = 0; i < charts[chartIndex]->chart->series().count(); ++i)
       ui->chartSeries->addItem(charts[chartIndex]->chart->series()[i]->name());
    ui->chartSeries->setCurrentRow(0);
}

void Options::saveSeriesData()                                  //сохраняет все изменения полей ввода для серии
{
    //поиск изменяемой серии по имени
    QString name = ui->currentSeriesChoise_CB->currentText();
    int index = countSerieIndex(name);

    //меняем свойства выбранной серии
    QPen *pen = new QPen();
    pen->setWidth(ui->seriesWidth->text().toInt());

    saveSeriesColor(pen);
    saveSeriesLineType(pen);

    allSeries[index]->setPen(*pen);

    allSeries[index]->setName(ui->currentSeriesName->text());
}

void Options::saveSeriesColor(QPen *pen)                        //вспомогательная ф-ия сохранения цвета
{
    QString color = ui->currentSeriesColor_CB->currentText();

    if(color == "красный")pen->setColor(QColor::fromRgb(255,0,0));
    else if(color == "зеленый")pen->setColor(QColor::fromRgb(0,255,0));
    else if(color == "синий")pen->setColor(QColor::fromRgb(0,0,255));
    else if(color == "черный")pen->setColor(QColor::fromRgb(0,0,0));
    else if(color == "оранжевый")pen->setColor(QColor::fromRgb(255,180,2));
}

void Options::saveSeriesLineType(QPen *pen)                     //вспомогательная ф-ия сохранения типа линии
{
    QString lineStyle = ui->currentSeriesLineType_CB->currentText();

    if (lineStyle == "сплошная") pen->setStyle(Qt::SolidLine);
    else if (lineStyle == "точки") pen->setStyle(Qt::DotLine);
    else if (lineStyle == "пунктир") pen->setStyle(Qt::DashLine);
    else if (lineStyle == "пунктир с точкой") pen->setStyle(Qt::DashDotLine);
}

void Options::saveChartsData()                                  //сохраняет все изменения графиков (РЕАЛИЗОВАТЬ)
{
    ;
}

void Options::updateSeriesName()                                //обновление имен серий (на случай изменения имени)
{
    ui->currentSeriesChoise_CB->clear();
    ui->allSeries->clear();

    for (int i=0; i<allSeries.count();i++) {
        ui->currentSeriesChoise_CB->addItem(allSeries[i]->name());
        ui->allSeries->addItem(allSeries[i]->name());
    }
    ui->allSeries->setCurrentRow(0);
    //ui->testlabel->setText(QString::number(allSeries.count()));
   // updateSeriesData(ui->currentSeriesChoise_CB->currentText());
}

void Options::updateCurrentSeriesName(int index)                //что-то странное...
{
    ui->currentSeriesChoise_CB->addItem(allSeries[index]->name());
    ui->currentSeriesChoise_CB->removeItem(index);
    ui->currentSeriesChoise_CB->setCurrentText(allSeries[index]->name());
}

void Options::updateChartsData()                                //обновляет поля ввода для графиков
{
    ui->allCharts->clear();
    for (int i = 0; i < charts.count(); i++)
    {
        ui->allCharts->addItem(charts[i]->chartView->chart()->title());
    }
    //добавить другие настройки
}

void Options::on_currentSeriesChoise_CB_currentTextChanged(const QString &arg1)
{
    //при изменение выбранной серии обновляем изначально показывающиеся данные
    updateSeriesData(arg1);
}

void Options::on_okButton_clicked()                             // ОК - сохраняем данные, отправляем и закрываем
{
    saveSeriesData();
    saveChartsData();
    emit saveData();
    this->~Options();
}

void Options::on_applyButton_clicked()                          // Применить - сохраняем данные и отправляем перерисовывать
{
    //обновляем параметры серии графиков внутри окна опций
    saveSeriesData();
    saveChartsData();

    //посылаем сигнал выгрузить обновленные данные
    emit saveData();
    updateSeriesName();
}

void Options::on_closeButton_clicked()                          //тупо закрываем и ничего не сохраняем
{
    //закрываем окно
    this->~Options();
}

void Options::on_chartLayot_CB_currentTextChanged(const QString &arg1) //включение и выключение SpinBox ов для расположения графиков
{
   // включение и выключние spinBox в зависимости от выбранного режима
    if (arg1 == "Один график") {ui->chartRowNumber_spin->setDisabled(true);ui->chartColNumber_spin->setDisabled(true);}
    else if (arg1 == "Вертикально") {ui->chartColNumber_spin->setEnabled(true);ui->chartRowNumber_spin->setDisabled(true);}
    else if (arg1 == "Горизонтально") {ui->chartRowNumber_spin->setEnabled(true);ui->chartColNumber_spin->setDisabled(true);}
    else if (arg1 == "Сетка") {ui->chartRowNumber_spin->setEnabled(true);ui->chartColNumber_spin->setEnabled(true);}
}

void Options::on_createLayot_button_clicked()                    //изменяем расположение графиков
{
    QString layoutMethod = ui->chartLayot_CB->currentText();
    int rowNumber = ui->chartRowNumber_spin->value();
    int colNumber = ui->chartColNumber_spin->value();
    emit saveLayout(layoutMethod, rowNumber, colNumber);         //отправляем перерисовывать расположение

    charts.clear();                                              //копируем новые графики и обновляем
    emit getAllCharts();
    updateChartsData();
    ui->allCharts->setCurrentRow(0);
}

void Options::on_addSerieToChart_clicked()                       //добавить серию в график
{
    //проверочки
    if(!ui->allCharts->count())
    {
        QMessageBox::critical(this, "Ошибочка", "Нет доступных графиков, создайте расположение");
        return;
    }
    else if(!ui->allSeries->count())
    {
        QMessageBox::critical(this, "Ошибочка", "Нет доступных серий, создайте новые серии");
        return;
    }

    int serieIndex = countSerieIndex(ui->allSeries->currentItem()->text());
    int chartIndex = ui->allCharts->currentRow();

    //добавляем выбранную серию в график и добавляем в лист серий графика
    charts[chartIndex]->chart->addSeries(allSeries[serieIndex]);
    ui->chartSeries->addItem(charts[chartIndex]->chart->series().last()->name());
    ui->chartSeries->setCurrentRow(0);

    //проверяем, есть ли у графика оси, если нет - создаем
    if(charts[chartIndex]->chart->axes().isEmpty())
    {
        charts[chartIndex]->axisX->setRange(0, 10);
        charts[chartIndex]->axisY->setRange(-3, 3);

        charts[chartIndex]->chart->addAxis(charts[chartIndex]->axisX,Qt::AlignBottom);
        charts[chartIndex]->chart->addAxis(charts[chartIndex]->axisY,Qt::AlignLeft);
    }

    //привязываем оси серию к осям
    allSeries[serieIndex]->attachAxis(charts[chartIndex]->axisX);
    allSeries[serieIndex]->attachAxis(charts[chartIndex]->axisY);
}

void Options::on_removeSerieFromChart_clicked()                  //удалить серию из графика
{
    // проверочка на наличие серий в графике
    if(!ui->chartSeries->count())
    {
        QMessageBox::critical(this, "Ошибочка", "Нет серий в графике");
        return;
    }

    int serieIndex = countSerieIndex(ui->chartSeries->currentItem()->text());
    int chartIndex = ui->allCharts->currentRow();

    //удаляем серию, очищаем лист серий и заполняем заново
    charts[chartIndex]->chart->removeSeries(allSeries[serieIndex]);

    updateChartSeriesList(chartIndex);
}

void Options::on_allCharts_currentRowChanged(int currentRow)     //что бы показывать текущие серии для графика и настройки для него
{
    updateChartSeriesList(currentRow);
}
