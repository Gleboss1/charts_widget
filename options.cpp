#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);

}

Options::~Options()
{
    delete ui;
}

void Options::updateSeriesData(QString name)
{
    //ищем индекс текущей серии по имени
    int index;
    for (index=0;index<seriesOption.count();index++)
        if (name == seriesOption[index]->series->name()) break;

    //обновляем данные о данной серии
    ui->currentSeriesName->setText(seriesOption[index]->series->name());
    ui->seriesWidth->setText(QString::number(seriesOption[index]->series->pen().width()));
    updateSeriesColor(index);
    updateSeriesLineType(index);

}

void Options::updateSeriesColor(int index)
{
   if(seriesOption[index]->series->pen().color() == QColor::fromRgb(255,0,0)) ui->currentSeriesColor_CB->setCurrentText("красный");
   else if(seriesOption[index]->series->pen().color() == QColor::fromRgb(0,255,0)) ui->currentSeriesColor_CB->setCurrentText("зеленый");
   else if(seriesOption[index]->series->pen().color() == QColor::fromRgb(0,0,255)) ui->currentSeriesColor_CB->setCurrentText("синий");
   else if(seriesOption[index]->series->pen().color() == QColor::fromRgb(0,0,0)) ui->currentSeriesColor_CB->setCurrentText("черный");
   else if(seriesOption[index]->series->pen().color() == QColor::fromRgb(255,180,2)) ui->currentSeriesColor_CB->setCurrentText("оранжевый");
}

void Options::updateSeriesLineType(int index)
{
   if(seriesOption[index]->series->pen().style() == Qt::SolidLine) ui->currentSeriesLineType_CB->setCurrentText("сплошная");
   else if(seriesOption[index]->series->pen().style() == Qt::DashLine) ui->currentSeriesLineType_CB->setCurrentText("пунктир");
   else if(seriesOption[index]->series->pen().style() == Qt::DotLine) ui->currentSeriesLineType_CB->setCurrentText("точки");
   else if(seriesOption[index]->series->pen().style() == Qt::DashDotLine) ui->currentSeriesLineType_CB->setCurrentText("пунктир с точкой");
}

void Options::updateSeriesName()
{
    //ui->currentSeriesChoise_CB->clear();
    for (int i=0;i<seriesOption.count();i++) {
        ui->currentSeriesChoise_CB->addItem(seriesOption[i]->series->name());
        ui->allSeries->addItem(seriesOption[i]->series->name());
    }
    updateSeriesData(ui->currentSeriesChoise_CB->currentText());
}

void Options::updateCurrentSeriesName(int index)
{
    ui->currentSeriesChoise_CB->addItem(seriesOption[index]->series->name());
    ui->currentSeriesChoise_CB->removeItem(index);
    ui->currentSeriesChoise_CB->setCurrentText(seriesOption[index]->series->name());
}



void Options::on_currentSeriesChoise_CB_currentTextChanged(const QString &arg1)
{
    //при изменение выбранной серии обновляем изначально показывающиеся данные
    updateSeriesData(arg1);
}

void Options::on_okButton_clicked()
{
    //отправляем данные и закрываем
    emit saveData();
    this->close();
    this->destroy();
}

void Options::on_applyButton_clicked()
{
    //просто сохраняем данные
    emit saveData();
}

void Options::on_closeButton_clicked()
{
    //закрываем окно
    this->close();
    this->destroy();
}

void Options::on_chartLayot_CB_currentTextChanged(const QString &arg1)
{
   // включение и выключние spinBox в зависимости от выбранного режима
    if (arg1 == "Один график") {ui->chartRowNumber_spin->setDisabled(true);ui->chartColNumber_spin->setDisabled(true);}
    else if (arg1 == "Вертикально") {ui->chartColNumber_spin->setEnabled(true);ui->chartRowNumber_spin->setDisabled(true);}
    else if (arg1 == "Горизонтально") {ui->chartRowNumber_spin->setEnabled(true);ui->chartColNumber_spin->setDisabled(true);}
    else if (arg1 == "Сетка") {ui->chartRowNumber_spin->setEnabled(true);ui->chartColNumber_spin->setEnabled(true);}
}

void Options::on_createLayot_button_clicked()
{
    QString layoutMethod = ui->chartLayot_CB->currentText();
    int rowNumber = ui->chartRowNumber_spin->value();
    int colNumber = ui->chartColNumber_spin->value();
    emit saveLayout(layoutMethod, rowNumber, colNumber);
}
