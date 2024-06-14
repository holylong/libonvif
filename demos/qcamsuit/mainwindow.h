#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>
#include "onvifservice.h"

class QHBoxLayout;
class IconButtonGroup;
class QStackedWidget;
class MultiContentWidget;
class ImageDisplayWidget;
class AnalysisContentWidget;
class SettingsWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initLayout();
    void initMemo();

private:
    Ui::MainWindow *ui;

    QHBoxLayout *_mainLayout;

    QScopedPointer<OnvifService> _onvifService;
    IconButtonGroup     *_iconGroup;
    QStackedWidget      *_stackedWidget;
    MultiContentWidget  *_multiContentWidget;
    AnalysisContentWidget       *_analysisContentWidget;
    SettingsWidget             *_settingsWidget;

    // QSet<QString>  _uniqueList;
    QStringList       _urls;
};
#endif // MAINWINDOW_H
