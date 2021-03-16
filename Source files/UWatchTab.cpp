#include "UWatchTab.h"
#include "ui_UWatchTab.h"



UWatchTab::UWatchTab(QWidget *parent, RDK::UApplication* app) :
    UVisualControllerWidget(parent, app),
    ui(new Ui::UWatchTab)
{
    ui->setupUi(this);
    colSplitter = nullptr;
    //создаем один график на вкладке
    createSingleLayout();

    //время обновления графика
    UpdateInterval = UpdateIntervalMs;
}

UWatchTab::~UWatchTab()
{
    delete ui;
}

void UWatchTab::createGraph()
{
    //создание 1 графика
    graph.push_back(new UWatchChart(this));
    graph.last()->setChartIndex(graph.count()-1);

    connect(graph.last(), SIGNAL(addSerieSignal(int)), this, SLOT(createSelectionDialogSlot(int)));
}

void UWatchTab::deleteGraph(int index)
{
    delete graph[index];
    graph.remove(index);
}


void UWatchTab::AUpdateInterface()
{
    for (int graphIndex=0; graphIndex < graph.count(); graphIndex++)
    {
        double y;
        double x;
        for (int serieIndex=0; serieIndex < graph[graphIndex]->countSeries(); serieIndex++)
        {
            // обратимся к ядру, возьмем матрицу, компонент и имя свойства
            RDK::UELockPtr<RDK::UNet> model=RDK::GetModelLock<RDK::UNet>();

            RDK::UEPtr<RDK::UContainer> component=model->GetComponentL(graph[graphIndex]->getSerie(serieIndex)->nameComponent.toStdString(),true);
            if(!component)
                continue;

            RDK::MDMatrix<double>* m=component->AccessPropertyData<RDK::MDMatrix<double> >(graph[graphIndex]->getSerie(serieIndex)->nameProperty.toStdString());
            if(!m)
                continue;

            if(m->GetCols()<=graph[graphIndex]->getSerie(serieIndex)->Jx || graph[graphIndex]->getSerie(serieIndex)->Jy)
                continue;

            //вычисляем координаты точки
            y=(*m)(graph[graphIndex]->getSerie(serieIndex)->Jy,graph[graphIndex]->getSerie(serieIndex)->Jx);
            x=Model_GetDoubleRealTime();

            // и добавим ее в график
            graph[graphIndex]->addDataToSerie(serieIndex, x, y);
        }
        //смещаем max оси Х вслед за временем
        if (x > graph[graphIndex]->axisXrange)
        {
            graph[graphIndex]->setAxisXmax(x+0.1);
            //и min если позволено
            if(graph[graphIndex]->isAxisXtrackable)graph[graphIndex]->setAxisXmin(x-graph[graphIndex]->axisXrange);
        }

    }
}

void UWatchTab::createSelectionDialogSlot(int index)
{
    createSelectionDialog(index);
}

void UWatchTab::createSplitterGrid(int rowNumber)
{
    //создаем вертикальный контейнер, в котором располагаются горизонтальные

    colSplitter = new QSplitter(this);
    colSplitter->setOrientation(Qt::Vertical);

    for (int i = 0; i < rowNumber; ++i)
    {
        rowSplitter.push_back(new QSplitter(this));
        rowSplitter.last()->setOrientation(Qt::Horizontal);
        colSplitter->addWidget(rowSplitter.last());
    }
    ui->horizontalLayout->addWidget(colSplitter);
}

void UWatchTab::deleteAllGraph()
{
    //удаляем все графики
    for (int i = tabRowNumber-1; i >= 0; --i)
    {
        for (int j = tabColNumber-1; j >= 0; --j)
        {
            delete graph.takeLast();
        }
        delete rowSplitter.takeLast();
    }
    //удаляем расположение
    if (colSplitter !=nullptr)
    {
        ui->horizontalLayout->removeWidget(colSplitter);
        delete colSplitter;
    }
}


void UWatchTab::createSingleLayout()
{
   deleteAllGraph();
   tabColNumber=1;
   tabRowNumber=1;

   createGraph();
   graph.last()->setChartTitle("Single graph");

   createSplitterGrid(1);
   rowSplitter.last()->addWidget(graph.last());
   layoutMode = 0;
}

void UWatchTab::createRowLayout(int rowNumber)
{
    deleteAllGraph();
    tabColNumber=1;
    tabRowNumber=rowNumber;

    createSplitterGrid(rowNumber);
    for(int i=0; i < rowNumber;i++)
    {
        createGraph();
        graph.last()->setChartTitle(QString("Horizontal graph %1").arg(QString::number(i)));
        rowSplitter[i]->addWidget(graph.last());
    }
    layoutMode = 2;
}

void UWatchTab::createColLayout(int colNumber)
{
    deleteAllGraph();

    tabColNumber=colNumber;
    tabRowNumber=1;

    createSplitterGrid(1);

    for(int i=0; i < colNumber;i++)
    {
        createGraph();
        graph.last()->setChartTitle(QString("Vertical graph %1").arg(QString::number(i)));
        rowSplitter[0]->addWidget(graph.last());
    }
    layoutMode = 1;
}

void UWatchTab::createGridLayout(int rowNumber, int colNumber)
{

    deleteAllGraph();

    tabColNumber=colNumber;
    tabRowNumber=rowNumber;

    createSplitterGrid(rowNumber);
    for(int i=0; i < rowNumber;i++)
    {
        for(int j=0; j < colNumber; j++)
        {
            createGraph();
            graph.last()->setChartTitle(QString("Grid graph %1").arg(QString::number((i)*colNumber+j+1)));

            rowSplitter[i]->addWidget(graph.last());
        }
    }
    layoutMode = 3;
}

UWatchChart *UWatchTab::getChart(int index)
{
    return graph[index];
}


int UWatchTab::countGraphs()
{
    return graph.count();
}

void UWatchTab::createSelectionDialog(int chartIndex)
{
    int channelIndex=0;
    QString componentName;
    QString componentProperty;

    //почему то не рабоатет если раскоментить(
    //if(!application)
    //    return;

    //создаем окно для выбора источника данных
    UComponentPropertySelectionWidget dialog(this, 3,application);
    dialog.show();
    if (dialog.exec())
    {
         channelIndex = dialog.componentsList->getSelectedChannelIndex();
         componentName = dialog.componentsList->getSelectedComponentLongName();
         componentProperty = dialog.componentsList->getSelectedPropertyName();
    }

    //проверяем что у выбран не пустой элемент (если нет модели)
    if(!componentName.isEmpty() && !componentProperty.isEmpty())
    {
        //создаем серию для выбранного источника
        graph[chartIndex]->createSerie(channelIndex, componentName, componentProperty, "type", 0, 0);
    }
}

void UWatchTab::saveUpdateInterval(int newInterval)
{
    UpdateIntervalMs = newInterval;
    UpdateInterval = newInterval;
}

int UWatchTab::getColNumber()
{
    return tabColNumber;
}

int UWatchTab::getRowNumber()
{
    return tabRowNumber;
}
