#include "UWatch.h"
#include "ui_UWatch.h"

UWatch::UWatch(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UWatch)
{
    ui->setupUi(this);
    //создаем первую вкладку
    createTab();
}

UWatch::~UWatch()
{
    delete ui;
}

UWatchTab *UWatch::getCurrentTab()
{
    return tab[ui->tabWidget->currentIndex()];
}


void UWatch::on_actionCreate_tab_triggered()
{
    createTab();
}

void UWatch::on_actionSeries_option_triggered()
{
    seriesOption = new UWatchSeriesOption(this);
    seriesOption->setWindowTitle("Series option");
    //seriesOption->setModal(true);
    seriesOption->show();

    //UWatchSeriesOption seriesOption(this);
    //seriesOption.setModal(true);
    //seriesOption.show();
    //seriesOption.exec();
}

void UWatch::on_actionCharts_option_triggered()
{
    chartOption = new UWatchChartOption(this);
    chartOption->setModal(true);
    chartOption->setWindowTitle("Charts option");
    chartOption->show();
    //chartOption->exec();

}

void UWatch::createTab()
{
    //создаем каждую новую вкладку с именем tab + номер
    tab.push_back(new UWatchTab(this));
    ui->tabWidget->addTab(tab.last(), QString("tab %0").arg(ui->tabWidget->count()+1));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void UWatch::deleteTab(int index)
{
    //спрашиваем юзера точно ли он уверен в закрытие вкладки
    QMessageBox messageBox;
    messageBox.setText("Are you sure you want to close the tab?");
    messageBox.setInformativeText("All data will be lost");
    messageBox.setWindowTitle("Closing tab");
    messageBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    messageBox.setIcon(QMessageBox::Question);

    if(messageBox.exec() == QMessageBox::Yes)
    {
        ui->tabWidget->removeTab(index);
        delete tab[index];
        tab.remove(index);
    }
    else return;
}

void UWatch::on_tabWidget_tabCloseRequested(int index)
{
    deleteTab(index);
}

void UWatch::on_actionTake_screenshot_triggered()
{
    QPixmap screenShot;
    screenShot = getCurrentTab()->grab(); //захватываем только текущую вкладку
    QString currentDate = QDateTime::currentDateTime().toString("dd-MM-yy HH-mm"); //текущая дата

    //работа с путем к папке screenshot
    QDir dir = QDir::current();
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();

    if(!tab.count())
    {
        //если нет вкладок, вежливо просим ее слздать
        QMessageBox messageBox;
        messageBox.setText("Ohhh, smth goes wrong...");
        messageBox.setInformativeText("Create tab and try again");
        messageBox.setWindowTitle("Screenshot");
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setStandardButtons(QMessageBox::Cancel);
        messageBox.exec();
        return;
    }

    //проверяем, есть ли папка screenshots
    //если нет, то создаем
    if(dir.cd("screenshots"))
    {
        std::cout<<"screenshot folder is exist"<<std::endl;
    }
    else {
         std::cout<<"screenshot folder is not exist"<<std::endl;
         std::cout<<"screenshot folder creating"<<std::endl;
         dir.mkdir("screenshots");
         dir.cd("screenshots");
    }

    if(screenShot.save((dir.path()+"/screenshots/screenshot " + currentDate + ".jpeg")))
    {
        //говорим что все хорошо и где натйи скриншот
        //std::cout<<(dir.path()+"/screenshots/screenshot " + currentDate + ".jpeg").toStdString()<<std::endl;
        std::cout<<"screenshot taken succes"<<std::endl;
        QMessageBox messageBox;
        messageBox.setText("Screenshot taken successfully!");
        messageBox.setInformativeText("saved in folder \"screenshots\"");
        messageBox.setWindowTitle("Screenshot");
        messageBox.setIcon(QMessageBox::Information);
        messageBox.setStandardButtons(QMessageBox::Cancel);
        messageBox.exec();
    }
    else std::cout<<"screenshot taken not succes"<<std::endl;  //что-то пошло не так

}
