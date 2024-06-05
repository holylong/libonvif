#include "onvifservice.h"
#include <QtConcurrent>

ONVIFDevice* ONVIFDevice::instance = nullptr;
OnvifService::OnvifService(QObject *parent) : QObject(parent), timer(new QTimer(this))
{
    connect(timer, &QTimer::timeout, this, &OnvifService::checkSystemMessages);

    _onvifClt = new ONVIFDevice();
}

void OnvifService::Start()
{
    timer->start(3000); // 3 seconds interval
}

void OnvifService::Stop()
{
    timer->stop();
}

void OnvifService::checkSystemMessages()
{
    // 这里是模拟检查系统消息的逻辑，实际应用中你需要替换为真正的检查逻辑
    QtConcurrent::run([this]() {
        _onvifClt->detectDevices();
        std::vector<std::string> arrUrls = _onvifClt->getURIs();
        if (arrUrls.size() > 0) {
            // 假设我们这里只是简单地发送两个空字符串作为示例
            emit onNewCamSignal("New Message", "Details");
        }else{
            qDebug() << "==>>> No Camera";
        }
    });
}
