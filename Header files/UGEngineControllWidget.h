#ifndef UGENGINECONTROLLWIDGET_H
#define UGENGINECONTROLLWIDGET_H

#include <QMainWindow>
#include <QSettings>
#include <QMdiSubWindow>

#include <rdk_application.h>

#include "UVisualControllerWidget.h"
#include "UComponentsListWidget.h"
#include "UDrawEngineWidget.h"
#include "UComponentLinksWidget.h"
#include "UImagesWidget.h"
#include "UCalculationChannelsWidget.h"
#include "UEngineControlQt.h"
#include "ULoggerWidget.h"
#include "UComponentPropertyChanger.h"
#include "UCreateConfigurationWizardWidget.h"
#include "UCreateTestWidget.h"
#include "UStatusPanel.h"
#include "USettingsReaderWidget.h"
#include "UGraphWidget.h"
#include "UTableInfo.h"
#include "UWatchFormWidget.h"
#include"UWatch.h"
//////////////////////////
//#include "UVideoAnalyticsSimpleSettingsWidget.h"

namespace Ui {
class UGEngineControllWidget;
}

struct USubTabDescription
{
/// ��� �������
QString Name;

QMdiSubWindow *SubWindow;

USubTabDescription(void)
    : SubWindow(NULL)
{}

};

struct USubTabDescriptionImages: public USubTabDescription
{
UImagesWidget* Images;

USubTabDescriptionImages(void)
    : USubTabDescription(),Images(NULL)
{};
};

struct USubTabDescriptionWatches: public USubTabDescription
{
UGraphWidget* Watches;

USubTabDescriptionWatches(void)
    : USubTabDescription(),Watches(NULL)
{};
};

/// UGEngineControllWidget class - ������� ���� ����������
///
/// �������� ��������� �� ��� ������ ���� � ��������� �� � ������� �������� � ������.

class UGEngineControllWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit UGEngineControllWidget(QWidget *parent = 0, RDK::UApplication *app = NULL);
    virtual ~UGEngineControllWidget();

    ///���������� ������ � ������������� �������
    void showChannelsWidget(void);

    //void setExternVideoAnalyticsSimpleWidget(UVideoAnalyticsSimpleSettingsWidget *externalWidget);

signals:
    void showSimpleSettings();

public slots:
    // settings
    void readSettings();
    void writeSettings();

    void showLinksForSingleComponent(QString componentName);
    void showLinksForTwoComponents(QString firstComponentName, QString secondComponentName);
    void switchLinksForTwoComponents(QString firstComponentName, QString secondComponentName);


    // actions:

    // file menu
    void actionLoadConfig();
    void actionCreateConfig();
    void actionSaveConfig();
    void actionCloseConfig();
    void actionCopyConfig();
    void actionExit();
    void actionConfigOptions();

    //chanels menu
    void actionAddNew();
    void actionInsert();
    void actionDeleteLast();
    void actionDeleteAll();
    void actionClone();


    // calculate menu
    void actionReloadParameters();
    void actionStart();
    void actionPause();
    void actionReset();
    void actionStep();
    void actionRunNSteps();

    // window menu
    void actionImages();
    void actionNewImages();
    void actionComponentsControl();
    void actionChannelsControl();
    void actionLogger();
    void actionTestCreator();
    void actionWatchWindow();///<-----------
    void actionNewWatches();
    void actionProfiling();
    //void actionVASimpleSettings();

private slots:
    void on_mdiArea_destroyed(QObject *arg1);

    void closeEvent(QCloseEvent *event);

private:
    // data
    Ui::UGEngineControllWidget *ui;

    // widgets
    USettingsReaderWidget *settings;
    UComponentPropertyChanger *propertyChanger;
    UDrawEngineWidget *drawEngine;
    UComponentLinksWidget *componentLinks;
    UImagesWidget *images;
    QMainWindow *imagesWindow;
    QMainWindow *graphWindow;
    QMainWindow *profilingWindow;
    UCalculationChannelsWidget *channels;
    ULoggerWidget *logger;
    UCreateConfigurationWizardWidget *createConfigurationWizardWidget;
    UCreateTestWidget *createTestWidget;
    UStatusPanel *statusPanel;
    UGraphWidget *graphWindowWidget;
    UWatchFormWidget *watchFormWidget;
    UTableInfo *profilingWindowWidget;
    UWatch *watchWindow;
    //UVideoAnalyticsSimpleSettingsWidget *videoAnalyticsSimpleWidget;

    /// ������ �������� ����������� ��������
    std::vector<USubTabDescriptionImages> imagesVector;

    /// ������ �������� ����������� ��������
    std::vector<USubTabDescriptionWatches> watchesVector;


    /// �������� ������ ����������
    RDK::UApplication *application;

    // methods

    ///if chanelIndex == -1 ��������� ��� ������ �������
    void startChannel(int chanelIndex);

    ///if chanelIndex == -1 ������������� ��� ������ �������
    void pauseChannel(int chanelIndex);

    ///if chanelIndex == -1 ���������� ��� ������ �������
    void resetChannel(int chanelIndex);

    ///if chanelIndex == -1 ������ ��� �� ��� ������ �������
    void calcOneStepChannel(int chanelIndex);

    ///������� � �������� ���������� ���� ��� ���������� UVisualControllerWidget
    void execDialogUVisualControllWidget(UVisualControllerWidget* widget);

    /// ��������� ����� ������ ����������� ��������
    void addImagesWidged();

    /// ������� ������ ����������� ��������
    void delImagesWidged(size_t index);

    /// ��������� ����� ������ ����������� ��������
    void addWatchesWidged();

    /// ������� ������ ����������� ��������
    void delWatchesWidged(size_t index);

};

/// �� ������������� QMdiSubwindow ��� ����������� ����� ������
class SubWindowCloseIgnore: public QMdiSubWindow
{
public:
    explicit SubWindowCloseIgnore(QWidget *parent = 0, Qt::WindowFlags flags = 0):QMdiSubWindow(parent, flags){}
protected:
    void closeEvent(QCloseEvent *event){event->ignore();}
};

/*
/// �� ������������� QMdiSubwindow ��� ����������� ����� ������
class SubWindowCloseExt: public QMdiSubWindow
{
public:
    explicit SubWindowCloseIgnore(UGEngineControllWidget* owner, QWidget *parent = 0, Qt::WindowFlags flags = 0):QMdiSubWindow(parent, flags), Owner(owner){}
protected:
    void closeEvent(QCloseEvent *event)
    {
     if(Owner)
      Owner->
    }

private:
UGEngineControllWidget* Owner;
};*/


#endif // UGENGINECONTROLLWIDGET_H
