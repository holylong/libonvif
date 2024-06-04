#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QObject>
#include <QFile>
#include <QIODevice>
#include <QApplication>

class StyleManager : public QObject{
    Q_OBJECT
    public:
    StyleManager(QObject* parent = nullptr):QObject(parent){}

    static StyleManager* Instance(){
        static StyleManager instance;
        return &instance;
    }

    bool loadStyleSheet(const QString& path){
        QFile file(path);
        if(file.exists()){
            file.open(QIODevice::Text | QIODevice::ReadOnly);
            if(file.isOpen()){
                QByteArray bytes = file.readAll();
                qApp->setStyleSheet(QString(bytes));
                file.close();
                return true;
            }
            return false;
        }
        return false;
    }

};

#endif // STYLEMANAGER_H
