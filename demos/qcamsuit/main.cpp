#include "mainwindow.h"
#include "stylemanager.h"
#include <QApplication>
#include <QLocale>
#include <QIcon>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/res/camera_zi.svg"));
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "qcamsuit_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    StyleManager::Instance()->loadStyleSheet(":/res/basictheme.qss");

    MainWindow w;
    w.show();
    return a.exec();
}
