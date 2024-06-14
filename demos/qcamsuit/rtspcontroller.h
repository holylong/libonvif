#ifndef RTSPCONTROLLER_H
#define RTSPCONTROLLER_H

#include <QObject>
#include <QThread>

class RtspController : public QObject
{
    Q_OBJECT
public:
    explicit RtspController(QObject *parent = nullptr);

    bool openRtsp(const QString& url);
    void stopRtsp(){_bRunning = false;while(_bOver){QThread::sleep(1);}}

    bool Close(void* ctx);

    QImage createImageFromBuffer(int width, int height, uchar* rgbBufferData, int channels);

    static void freeAvMemory(void *data);

    static void custom_log_callback(void *ptr, int level, const char *fmt, va_list vl);

signals:
    void onFrame(const QImage& frame);

private:
    bool      _bRunning{true};
    bool      _bOver{false};
};

#endif // RTSPCONTROLLER_H
