#ifndef ONVIFSERVICE_H
#define ONVIFSERVICE_H

#include <QObject>
#include <QTimer>

class OnvifService : public QObject
{
    Q_OBJECT
public:
    explicit OnvifService(QObject *parent = nullptr);

    void Start();
    void Stop();

signals:
    void onNewCamSignal(QString, QString);

private slots:
    void checkSystemMessages();

private:
    QTimer *timer;
};

#endif // ONVIFSERVICE_H
