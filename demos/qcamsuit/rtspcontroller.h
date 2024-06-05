#ifndef RTSPCONTROLLER_H
#define RTSPCONTROLLER_H

#include <QObject>

class RtspController : public QObject
{
    Q_OBJECT
public:
    explicit RtspController(QObject *parent = nullptr);

signals:
};

#endif // RTSPCONTROLLER_H
