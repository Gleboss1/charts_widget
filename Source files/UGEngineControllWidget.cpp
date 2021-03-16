#include "UGEngineControllWidget.h"
#include "ui_UGEngineControllWidget.h"


#include <rdk_application.h>
#include <QSettings>
#include <QDebug>
#include <QDialog>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QMdiSubWindow>
#include <QCloseEvent>
#include <QThread>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QInputDialog>

/*int heheheCounter = 0;
void hehehe(){qDebug("hehehe %d", ++heheheCounter);}*/

UGEngineControllWidget::UGEngineControllWidget(QWidget *parent, RDK::UApplication *app) :
    QMainWindow(parent),
    ui(new Ui::UGEngineControllWidget)
{
    ui->setupUi(this);
    setAccessibleName("UGEngineControllWidget");

    application = app;

    if(application == NULL)
      QApplication::exit(-1);

    settings = NULL;
    propertyChanger = NULL;
    drawEngine = NULL;
    componentLinks = NULL;
    images = NULL;
    imagesWindow = NULL;
    channels = NULL;
    createConfigurationWizardWidget = NULL;
    createTestWidget = NULL;
    statusPanel = NULL;
    //videoAnalyticsSimpleWidget=NULL;
    graphWindowWidget=NULL;
    graphWindow=NULL;
    profilingWindow=NULL;
    profilingWindowWidget=NULL;
    watchFormWidget=NULL;
    watchWindow = NULL;

    settings = new USettingsReaderWidget(this);
    connect(settings, SIGNAL(readSetting()) , this, SLOT(readSettings()));
    connect(settings, SIGNAL(writeSetting()), this, SLOT(writeSettings()));

    propertyChanger = new UComponentPropertyChanger(this, application);
    ui->dockWidgetComponentsList->setWidget(propertyChanger);

    drawEngine = new UDrawEngineWidget(this, application);
    QMdiSubWindow *drawEngineSbWindow = new SubWindowCloseIgnore(ui->mdiArea, Qt::SubWindow);
    drawEngineSbWindow->setWidget(drawEngine);
    drawEngineSbWindow->show();
    drawEngineSbWindow->showMaximized();

    // связывание схемы модели и списка отображения компонентов модели
    //  схема -> список
    connect(propertyChanger->componentsList, SIGNAL(componentDoubleClick(QString)),
            drawEngine, SLOT(componentDoubleClick(QString)));
    connect(propertyChanger->componentsList, SIGNAL(componentSelected(QString)),
            drawEngine, SLOT(componentSingleClick(QString)));
    connect(propertyChanger->componentsList, SIGNAL(updateScheme(bool)),
            drawEngine, SLOT(updateScheme(bool)));

    //  список -> схема
    connect(drawEngine, SIGNAL(componentSelectedFromScheme(QString)),
            propertyChanger->componentsList, SLOT(componentSelectedFromScheme(QString)));
    connect(drawEngine, SIGNAL(componentDoubleClickFromScheme(QString)),
            propertyChanger->componentsList, SLOT(componentDoubleClickFromScheme(QString)));
    connect(drawEngine, SIGNAL(componentStapBackFromScheme()),
            propertyChanger->componentsList, SLOT(componentStapBackFromScheme()));
    connect(drawEngine, SIGNAL(updateComponentsListFromScheme()),
            propertyChanger->componentsList, SLOT(updateComponentsListFromScheme()));

    componentLinks = new UComponentLinksWidget(this, application);
    componentLinks->hide();

    // связывание схемы модели и окна отображения связей
    //  связи -> схема
    connect(componentLinks, SIGNAL(updateScheme(bool)), drawEngine, SLOT(updateScheme(bool)));

    //  схема -> связи
    // обнако слоты находятся в окне главного интерфейса, так как необходимо сначала создать
    // диалоговые окна (QDialog) для отображения виджета связей
    connect(drawEngine, SIGNAL(viewLinksFromScheme(QString)), this, SLOT(showLinksForSingleComponent(QString)));
    connect(drawEngine, SIGNAL(createLinksFromScheme(QString,QString)), this, SLOT(showLinksForTwoComponents(QString,QString)));
    connect(drawEngine, SIGNAL(switchLinksFromScheme(QString,QString)), this, SLOT(switchLinksForTwoComponents(QString,QString)));

    images = new UImagesWidget(this, application);
    images->hide();

    channels = new UCalculationChannelsWidget(this, application);
    QRect rect=channels->geometry();
    rect.setWidth(50);
    channels->setGeometry(rect);
    ui->dockWidgetChannels->setWidget(channels);

    logger = new ULoggerWidget(this, application);
    ui->dockWidgetLoger->setWidget(logger);

    graphWindowWidget = new UGraphWidget(this, application);
//    ui->dockWidgetGraph->setWidget(graphWindowWidget);
    graphWindowWidget->setWindowTitle("Graph");
       graphWindowWidget->hide();
    ui->dockWidgetGraph->hide();

    watchFormWidget= new UWatchFormWidget(this, application);
    watchFormWidget->setWindowTitle("Watches");
    watchFormWidget->hide();

    profilingWindowWidget = new UTableInfo(this, application);
    ui->dockWidgetProfiling->setWidget(profilingWindowWidget);
    profilingWindowWidget->setWindowTitle("Profiling");

    createConfigurationWizardWidget=new UCreateConfigurationWizardWidget(this, application);

    createTestWidget = new UCreateTestWidget(this, application);
    createTestWidget->hide();

    statusPanel = new UStatusPanel(this, application);
    ui->statusBar->addWidget(statusPanel, 1);
    connect(statusPanel, SIGNAL(saveConfig()), this, SLOT(actionSaveConfig()));
    connect(statusPanel, SIGNAL(setPropertyUpdateInterval(long)),
            propertyChanger->componentsList, SLOT(setUpdateInterval(long)));

    // GUI actions:

    // file menu actions:
    connect(ui->actionCreateConfig, SIGNAL(triggered(bool)), this, SLOT(actionCreateConfig()));
    connect(ui->actionLoadConfig, SIGNAL(triggered(bool)), this, SLOT(actionLoadConfig()));
    connect(ui->actionSaveConfig, SIGNAL(triggered(bool)), this, SLOT(actionSaveConfig()));
    connect(ui->actionCloseConfig, SIGNAL(triggered(bool)), this, SLOT(actionCloseConfig()));
    connect(ui->actionCopyConfig, SIGNAL(triggered(bool)), this, SLOT(actionCopyConfig()));

    connect(ui->actionConfigOptions, SIGNAL(triggered(bool)), this, SLOT(actionConfigOptions()));


    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(actionExit()));

    //chanels menu actions:
    connect(ui->actionAddNew, SIGNAL(triggered(bool)), this, SLOT(actionAddNew()));
    connect(ui->actionInsert, SIGNAL(triggered(bool)), this, SLOT(actionInsert()));
    connect(ui->actionDeleteLast, SIGNAL(triggered(bool)), this, SLOT(actionDeleteLast()));
    connect(ui->actionDeleteAll, SIGNAL(triggered(bool)), this, SLOT(actionDeleteAll()));
    connect(ui->actionClone, SIGNAL(triggered(bool)), this, SLOT(actionClone()));

    // calculate menu actions:
    connect(ui->actionStart, SIGNAL(triggered(bool)), this, SLOT(actionStart()));
    connect(ui->actionPause, SIGNAL(triggered(bool)), this, SLOT(actionPause()));
    connect(ui->actionReset, SIGNAL(triggered(bool)), this, SLOT(actionReset()));
    connect(ui->actionStep, SIGNAL(triggered(bool)), this, SLOT(actionStep()));
    connect(ui->actionRunNSteps, SIGNAL(triggered(bool)), this, SLOT(actionRunNSteps()));

    // window menu actions:
    connect(ui->actionImagesFromWindow, SIGNAL(triggered(bool)), this, SLOT(actionImages()));
    connect(ui->actionImagesFromNewWindow, SIGNAL(triggered(bool)), this, SLOT(actionNewImages()));
    connect(ui->actionComponentsControl, SIGNAL(triggered(bool)), this, SLOT(actionComponentsControl()));
    connect(ui->actionChannelsControl, SIGNAL(triggered(bool)), this, SLOT(actionChannelsControl()));
    connect(ui->actionLogger, SIGNAL(triggered(bool)), this, SLOT(actionLogger()));
    connect(ui->actionTestCreator, SIGNAL(triggered(bool)), this, SLOT(actionTestCreator()));
    connect(ui->actionWatchWindow, SIGNAL(triggered(bool)), this, SLOT(actionWatchWindow()));
    connect(ui->actionProfiling, SIGNAL(triggered(bool)), this, SLOT(actionProfiling()));
    connect(ui->actionWatchesFromNewWindow, SIGNAL(triggered(bool)), this, SLOT(actionNewWatches()));
    connect(ui->actionVASimpleSettings, SIGNAL(triggered(bool)), this, SIGNAL(showSimpleSettings()));
    //connect(ui->action, SIGNAL(triggered(bool)), this, SLOT(action)));

    readSettings();
}



UGEngineControllWidget::~UGEngineControllWidget()
{
    application->UnInit();
    delete ui;
}

/*void UGEngineControllWidget::setExternVideoAnalyticsSimpleWidget(UVideoAnalyticsSimpleSettingsWidget *externalWidget)
{
    if(externalWidget!=NULL)
    {
        videoAnalyticsSimpleWidget = externalWidget;
    }
}*/

void UGEngineControllWidget::showLinksForSingleComponent(QString componentName)
{
    componentLinks->initWidget(componentName);
    execDialogUVisualControllWidget(componentLinks);
}

void UGEngineControllWidget::showLinksForTwoComponents(QString firstComponentName, QString secondComponentName)
{
    componentLinks->initWidget(firstComponentName, secondComponentName);
    execDialogUVisualControllWidget(componentLinks);
}

void UGEngineControllWidget::switchLinksForTwoComponents(QString firstComponentName, QString secondComponentName)
{
    componentLinks->initWidget(firstComponentName, secondComponentName, 3);
    execDialogUVisualControllWidget(componentLinks);
}

// file menu actions

void UGEngineControllWidget::actionLoadConfig()
{
 QString default_path=QString::fromLocal8Bit((application->GetWorkDirectory()+"/../../Configs/").c_str());
 QDir path1(default_path);
 if(!path1.exists(default_path))
 {
  default_path=QString::fromLocal8Bit((application->GetWorkDirectory()+"/../../../Configs/").c_str());
  QDir path2(default_path);
  if(!path2.exists(default_path))
  {
   default_path=QString::fromLocal8Bit(application->GetWorkDirectory().c_str());
  }
 }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open config file"), default_path, tr("*.ini"));

    if (fileName.isEmpty())
      return;
    try
    {
      application->OpenProject(fileName.toLocal8Bit().constData());

      this->setWindowTitle("project: " + fileName);

      /*QStringList list = configFileName.split("/");
      list.pop_back();*/

      RDK::UIVisualControllerStorage::UpdateInterface(true);
      //drawEngine->updateScheme(true);
    }
    catch(RDK::UException& e)
    {
      QMessageBox::critical(this,"Error at load project", QString(e.what()), QMessageBox::Ok);
    }
    catch(std::exception& e)
    {
      QMessageBox::critical(this,"Error at load project", QString(e.what()), QMessageBox::Ok);
    }
}

void UGEngineControllWidget::actionCreateConfig()
{
 if(application->GetProjectOpenFlag())
 {
  QMessageBox::StandardButton reply = QMessageBox::question(this, "Warning", "Another configuration is open. Close?", QMessageBox::Save|QMessageBox::Close|QMessageBox::Cancel);
  if (reply == QMessageBox::Save)
  {
   application->SaveProject();
   application->CloseProject();
  }
  else
  if(reply == QMessageBox::Close)
  {
   application->CloseProject();
  }
  else
   return;
 }

 createConfigurationWizardWidget->show();
}

void UGEngineControllWidget::actionSaveConfig()
{
    application->SaveProject();
}

void UGEngineControllWidget::actionCloseConfig()
{
 try
 {
  application->PauseChannel(-1);
  application->CloseProject();
 }
 catch(RDK::UException& e)
 {
  QMessageBox::critical(this,"Error at load project", QString(e.what()), QMessageBox::Ok);
 }
 catch(std::exception& e)
 {
  QMessageBox::critical(this,"Error at load project", QString(e.what()), QMessageBox::Ok);
 }
}

void UGEngineControllWidget::actionCopyConfig()
{
 try
 {
 QString default_path=QString::fromLocal8Bit((application->GetWorkDirectory()+"/../../Configs/").c_str());
 QDir path1(default_path);
 if(!path1.exists(default_path))
 {
  default_path=QString::fromLocal8Bit((application->GetWorkDirectory()+"/../../../Configs/").c_str());
  QDir path2(default_path);
  if(!path2.exists(default_path))
  {
   default_path=QString::fromLocal8Bit(application->GetWorkDirectory().c_str());
  }
 }

 QString res_path=QFileDialog::getExistingDirectory(this, tr("Create project directory"), default_path, QFileDialog::ShowDirsOnly);
 res_path+="/";

 application->CopyProject(res_path.toLocal8Bit().constData());

 QMessageBox::StandardButton reply = QMessageBox::question(this, "Info", "Current configuration has been copied to selected destination. Switch to new destination? If you select NO we continue work with previous configuration.", QMessageBox::Yes|QMessageBox::No);
 if(reply == QMessageBox::Yes)
 {
  std::string project_file_name=application->GetProjectFileName();
  actionCloseConfig();
  std::string open_file_name=res_path.toLocal8Bit().constData();
  open_file_name+=project_file_name;
  application->OpenProject(open_file_name);
  this->setWindowTitle(QString("project: ") + open_file_name.c_str());
  RDK::UIVisualControllerStorage::UpdateInterface(true);
 }

 }
 catch(RDK::UException& e)
 {
   QMessageBox::critical(this,"Error at load project", QString(e.what()), QMessageBox::Ok);
 }
 catch(std::exception& e)
 {
   QMessageBox::critical(this,"Error at load project", QString(e.what()), QMessageBox::Ok);
 }
}

void UGEngineControllWidget::actionExit()
{
  QApplication::quit();
}

void UGEngineControllWidget::actionConfigOptions()
{
 createConfigurationWizardWidget->show();
}

//chanels menu actions:
void UGEngineControllWidget::actionAddNew()
{
    if(!application->GetProjectOpenFlag())
     return;

    pauseChannel(-1);
    application->SetNumChannels(application->GetNumChannels()+1);
    RDK::UIVisualControllerStorage::UpdateInterface(true);
}

void UGEngineControllWidget::actionInsert()
{
    if(!application->GetProjectOpenFlag())
     return;
    pauseChannel(-1);
    int i=Core_GetSelectedChannelIndex();
    application->InsertChannel(i);
    //application->AddChannel(ChannelsStringGrid->Row);
    //RDK::UIVisualControllerStorage::UpdateInterface(true);
    RDK::UIVisualControllerStorage::UpdateInterface(true);
}

void UGEngineControllWidget::actionDeleteLast()
{
    /*if(!application->GetProjectOpenFlag())
     return;
    application->SetNumChannels(Core_GetNumChannels()-1);
    RDK::UIVisualControllerStorage::UpdateInterface(true);*/
    pauseChannel(-1);
    application->DeleteChannel(application->GetNumChannels()-1);
    RDK::UIVisualControllerStorage::UpdateInterface(true);

}

void UGEngineControllWidget::actionDeleteAll()
{
    if(!application->GetProjectOpenFlag())
     return;
    pauseChannel(-1);
    application->SetNumChannels(1);
    //RDK::UIVisualControllerStorage::UpdateInterface(true);
}

void UGEngineControllWidget::actionClone()
{
    pauseChannel(-1);
    int cloned_id=application->GetNumChannels();
    application->CloneChannel(Core_GetSelectedChannelIndex(), cloned_id);
    RDK::UIVisualControllerStorage::UpdateInterface(true);

}

// calculate menu actions

void UGEngineControllWidget::actionReloadParameters()
{
    application->ReloadParameters();
}
//int ts; // костыль
void UGEngineControllWidget::actionStart()
{
  try
  {
    startChannel(-1);
    //ts = startTimer(150); // костыль
//    ui->statusBar->showMessage("Calculation in progress");
  }
  catch(RDK::UException& e)
  {
    QMessageBox::critical(this,"UError at start calculation", QString(e.what()), QMessageBox::Ok);
  }
  catch(std::exception& e)
  {
    QMessageBox::critical(this,"Error at start calculation", QString(e.what()), QMessageBox::Ok);
  }
}

void UGEngineControllWidget::actionPause()
{
    pauseChannel(-1);
    //killTimer(ts); // костыль
//    ui->statusBar->showMessage("Calculation at pause");
}

void UGEngineControllWidget::actionReset()
{
    resetChannel(-1);
//    ui->statusBar->showMessage("Calculation reseted", 5000);
}

void UGEngineControllWidget::actionStep()
{
  calcOneStepChannel(-1);
}

void UGEngineControllWidget::actionRunNSteps()
{
  bool ok;
  QString text = QInputDialog::getText(this, tr("Run several steps"),
                                       tr("Enter steps number: "), QLineEdit::Normal,
                                       "", &ok);
  unsigned int stepsCounter = text.toUInt();
  if (ok && stepsCounter > 0)
  {
    for(unsigned int i = 0; i < stepsCounter; ++i)
      application->StepChannel(-1);
  }
}

// window menu action

void UGEngineControllWidget::actionImages()
{
    if(!imagesWindow)
    {
        imagesWindow = new QMainWindow(this);
        imagesWindow->setCentralWidget(images);
    }
    imagesWindow->resize(images->size());
    images->show();
    imagesWindow->showNormal();
    imagesWindow->activateWindow();
}

void UGEngineControllWidget::actionNewImages()
{
 addImagesWidged();
}

void UGEngineControllWidget::actionComponentsControl()
{
    ui->dockWidgetComponentsList->show();
}

void UGEngineControllWidget::actionChannelsControl()
{
  ui->dockWidgetChannels->show();
}

void UGEngineControllWidget::actionLogger()
{
  ui->dockWidgetLoger->show();
}

void UGEngineControllWidget::actionTestCreator()
{
  execDialogUVisualControllWidget(createTestWidget);
}

void UGEngineControllWidget::actionWatchWindow()
{
    if(watchWindow != NULL) watchWindow->show();
    else
    {
        watchWindow = new UWatch(this);
        watchWindow->setWindowTitle("Watch window");
        watchWindow->show();
    }
    /*
 if(!graphWindow )
 {
     graphWindow = new QMainWindow(this);
     graphWindow->setWindowTitle("Watches");
     //graphWindow->setCentralWidget(graphWindowWidget);
     //graphWindowWidget->show();
     graphWindow->setCentralWidget(watchFormWidget);
     watchFormWidget->show();
 }

    if (!graphWindow->isVisible())
    {
        graphWindow->resize(watchFormWidget->size());
//        graphWindow->resize(graphWindowWidget->size());
        graphWindow->setWindowTitle("");
        graphWindow->show();
        graphWindow->showNormal();
    }

    graphWindow->activateWindow();
    //отобразить *graphWindowWidget
//    ui->dockWidgetGraph->show();*/
}

void UGEngineControllWidget::actionProfiling()
{
    if (!ui->dockWidgetProfiling->isVisible())
    {
        ui->dockWidgetProfiling->show();
    }
    /*if(!profilingWindow)
    {
        profilingWindow = new QMainWindow(this);
        profilingWindow->setCentralWidget(profilingWindowWidget);
    }
    profilingWindow->resize(profilingWindowWidget->size());
    profilingWindow->setWindowTitle("Profiling");
    profilingWindow->show();
    profilingWindow->showNormal();
    profilingWindow->activateWindow();*/

    //отобразить *graphWindowWidget
//    ui->dockWidgetGraph->show();
}

void UGEngineControllWidget::actionNewWatches()
{
    addWatchesWidged();
}


/*void UGEngineControllWidget::actionVASimpleSettings()
{
    if(videoAnalyticsSimpleWidget)
    {
        videoAnalyticsSimpleWidget->show();
    }
}*/

void UGEngineControllWidget::startChannel(int chanelIndex)
{
    if(!application->GetProjectOpenFlag())
        return;

    application->StartChannel(chanelIndex);
}

void UGEngineControllWidget::pauseChannel(int chanelIndex)
{
    if(!application->GetProjectOpenFlag())
        return;

    application->PauseChannel(chanelIndex);
}

void UGEngineControllWidget::resetChannel(int chanelIndex)
{
    if(!application->GetProjectOpenFlag())
        return;

    application->ResetChannel(chanelIndex);
}

void UGEngineControllWidget::calcOneStepChannel(int chanelIndex)
{
    if(!application->GetProjectOpenFlag())
        return;

    application->StepChannel(chanelIndex);
}

/// Добавляет новый виджет в imagesVector
void UGEngineControllWidget::addImagesWidged()
{
 USubTabDescriptionImages descr;
 UImagesWidget *new_images = new UImagesWidget(this, application);
 descr.Name=QString("Images ")+RDK::sntoa(int(imagesVector.size()+1)).c_str();
 descr.Images=new_images;

 new_images->setWindowTitle(descr.Name);

 QMdiSubWindow *imagesSbWindow = new QMdiSubWindow(ui->mdiArea, Qt::SubWindow);
 imagesSbWindow->setWidget(new_images);
 imagesSbWindow->setAttribute(Qt::WA_DeleteOnClose);
 imagesSbWindow->show();
 imagesSbWindow->showMaximized();
 descr.SubWindow=imagesSbWindow;

 imagesVector.push_back(descr);
}

/// Удаляет виджет из imagesVector по имени
void UGEngineControllWidget::delImagesWidged(size_t index)
{
 if(index>=imagesVector.size())
  return;
 imagesVector.erase(imagesVector.begin()+index);
}


/// Добавляет новый виджет отображения графиков
void UGEngineControllWidget::addWatchesWidged()
{
    USubTabDescriptionWatches descr;
    UGraphWidget *new_watches = new UGraphWidget(this, application);
    descr.Name=QString("Watches ")+RDK::sntoa(int(imagesVector.size()+1)).c_str();
    descr.Watches=new_watches;

    new_watches->setWindowTitle(descr.Name);

    QMdiSubWindow *imagesSbWindow = new QMdiSubWindow(ui->mdiArea, Qt::SubWindow);
    imagesSbWindow->setWidget(new_watches);
    imagesSbWindow->setAttribute(Qt::WA_DeleteOnClose);
    imagesSbWindow->show();
    imagesSbWindow->showMaximized();
    descr.SubWindow=imagesSbWindow;

    watchesVector.push_back(descr);
}

/// Удаляет виджет отображения графиков
void UGEngineControllWidget::delWatchesWidged(size_t index)
{
    if(index>=watchesVector.size())
     return;
    watchesVector.erase(watchesVector.begin()+index);
}


void UGEngineControllWidget::execDialogUVisualControllWidget(UVisualControllerWidget *widget)
{
    QWidget *widgetOldParent = widget->parentWidget();

    QDialog dialogWindow(this);
    QHBoxLayout *lay = new QHBoxLayout(&dialogWindow);
    lay->setMargin(0);
    lay->addWidget(widget);
    dialogWindow.resize(widget->size());
    dialogWindow.setLayout(lay);
    dialogWindow.setWindowTitle(widget->accessibleName());
    connect(&dialogWindow, SIGNAL(rejected()), widget, SLOT(unInit()));
    connect(widget, SIGNAL(closeWindow()), &dialogWindow, SLOT(reject()));
    widget->show();
    dialogWindow.exec();
    widget->hide();
    widget->setParent(widgetOldParent);
}

void UGEngineControllWidget::writeSettings()
{
    if(!application) return;

    QSettings settings(QString::fromLocal8Bit(
                         application->GetProjectPath().c_str())+"settings.qt",
                       QSettings::IniFormat);
    settings.beginGroup(accessibleName());

    settings.setValue("geometry", saveGeometry());
    settings.setValue("state",    saveState());

    if(imagesWindow)
    {
      settings.setValue("ImagesGeometry", imagesWindow->saveGeometry());
      settings.setValue("ImagesState",    imagesWindow->saveState());
    }

    settings.endGroup();
}

void UGEngineControllWidget::readSettings()
{
    if(!application) return;

    QSettings settings(QString::fromLocal8Bit(
                         application->GetProjectPath().c_str())+"settings.qt",
                       QSettings::IniFormat);
    settings.beginGroup(accessibleName());

    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());

    if(!imagesWindow)
    {
        imagesWindow = new QMainWindow(this);
        imagesWindow->setCentralWidget(images);
    }
    imagesWindow->resize(images->size());
    imagesWindow->restoreGeometry(settings.value("ImagesGeometry").toByteArray());
    imagesWindow->restoreState(settings.value("ImagesState").toByteArray());

    settings.endGroup();
}

void UGEngineControllWidget::on_mdiArea_destroyed(QObject *arg1)
{
 for(size_t i=0;i<imagesVector.size();i++)
 {
  if(imagesVector[i].SubWindow == arg1)
  {
   delImagesWidged(i);
   break;
  }
 }
}

void UGEngineControllWidget::showChannelsWidget (void)
{
    channels->show();
}

void UGEngineControllWidget::closeEvent(QCloseEvent *event)
{
 application->PauseChannel(-1);
 application->CloseProject();
 application->UnInit();
 event->accept();
 //   if (maybeSave()) {
 //       writeSettings();
 //       event->accept();
 //   } else {
 //       event->ignore();
 //   }
}
