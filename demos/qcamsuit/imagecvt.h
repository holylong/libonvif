#ifndef IMAGECVT_H
#define IMAGECVT_H

#include <QObject>
#include <QImage>

class ImageCvt : public QObject
{
    Q_OBJECT
public:
    explicit ImageCvt(QObject *parent = nullptr);

    // 使用给定的宽度、高度和RGB缓冲区数据创建QImage
    static QImage createImageFromBuffer(int width, int height, uchar* rgbBufferData, int channels)
    {
        QImage image(width, height, QImage::Format_RGB888);
        if (image.isNull()) {
            qWarning("Failed to create QImage");
            return QImage();
        }

        // 注意：这里假设RGB数据是连续存储的，并且是RGB格式
        memcpy(image.bits(), rgbBufferData, width * height * channels);

        return image;
    }

signals:
};

#endif // IMAGECVT_H
