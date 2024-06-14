#ifndef ONVIFSERVICE_H
#define ONVIFSERVICE_H

#include <QObject>
#include <QTimer>

#include <onvif_cxx.hpp>

class OnvifService : public QObject
{
    Q_OBJECT
public:
    explicit OnvifService(QObject *parent = nullptr);

    void Start();
    void Stop();

signals:
    void onNewCamSignal(QString, QString);
    void onNewCamSignalList(const QStringList&);

private slots:
    void checkSystemMessages();

private:
    QTimer *timer;

    ONVIFDevice* _onvifClt;
};

#endif // ONVIFSERVICE_H
