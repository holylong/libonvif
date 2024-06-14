#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "iconbutton.h"
#include "iconbuttongroup.h"
#include "onvifservice.h"
#include "multicontentwidget.h"
#include "analysiscontentwidget.h"
#include "settingswidget.h"
#include "stringutil.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CamSuit");
    setWindowIcon(QIcon(":/res/camera_zi.svg"));
    _urls.clear();
    initMemo();
    initLayout();
}

void MainWindow::initMemo()
{
    _onvifService.reset(new OnvifService());
    _onvifService->Start();

    QObject::connect(_onvifService.get(), &OnvifService::onNewCamSignalList, this, [=](const QStringList& urls){
        _urls = StringUtil::RepeatStringDel(urls);
        StringUtil::Display(_urls);
        _multiContentWidget->setUrls(_urls.toVector());
    });
}

void MainWindow::initLayout()
{
    _mainLayout = new QHBoxLayout(ui->centralwidget);
    _stackedWidget = new QStackedWidget(ui->centralwidget);
    _analysisContentWidget = new AnalysisContentWidget(_stackedWidget);
    _stackedWidget->addWidget(_analysisContentWidget);
    _multiContentWidget = new MultiContentWidget(_stackedWidget);
    _stackedWidget->addWidget(_multiContentWidget);
    _settingsWidget = new SettingsWidget(_stackedWidget);
    _stackedWidget->addWidget(_settingsWidget);

    _iconGroup = new IconButtonGroup();
    IconButton *iconCamera = new IconButton("Camera", ":/res/camera_zi.svg");
    IconButton *iconSettings = new IconButton("Settings", ":/res/settings_zi.svg");
    IconButton *iconSplitCamera = new IconButton("All Camera", ":/res/split_zi.svg");
    _iconGroup->AppendButton(iconCamera);
    _iconGroup->AppendButton(iconSplitCamera);
    _iconGroup->AppendButton(iconSettings);
    _iconGroup->setFixedWidth(150);
    QObject::connect(iconCamera, &IconButton::clicked, this, [=]{
        qDebug() << "camera switch";

        iconCamera->setClickTheme();
        iconSplitCamera->resetClickTheme();
        iconSettings->resetClickTheme();
        iconCamera->setFixedWidth(150);
        _stackedWidget->setCurrentIndex(0);
    });

    QObject::connect(iconSplitCamera, &IconButton::clicked, this, [=]{
        qDebug() << "all camera switch";
        iconSplitCamera->setClickTheme();
        iconCamera->resetClickTheme();
        iconSettings->resetClickTheme();
        iconSplitCamera->setFixedWidth(150);
        _stackedWidget->setCurrentIndex(1);
    });

    QObject::connect(iconSettings, &IconButton::clicked, this, [=]{
        qDebug() << "settings switch";
        iconSettings->setClickTheme();
        iconSplitCamera->resetClickTheme();
        iconCamera->resetClickTheme();
        iconSettings->setFixedWidth(150);
        _stackedWidget->setCurrentIndex(2);
    });

    ui->centralwidget->layout()->addWidget(_iconGroup);
    ui->centralwidget->layout()->addWidget(_stackedWidget);

    _stackedWidget->setCurrentIndex(1);
    iconSplitCamera->setClickTheme();
    iconCamera->resetClickTheme();
    iconSettings->resetClickTheme();
    iconSplitCamera->setFixedWidth(150);
}

MainWindow::~MainWindow()
{
    _onvifService->Stop();
    delete ui;
}
