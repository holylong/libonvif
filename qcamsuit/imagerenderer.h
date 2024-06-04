#ifndef IMAGEDISPLAYWIDGET_H
#define IMAGEDISPLAYWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QMenu>
#include <QImage>
#include <QOpenGLTexture>
#include <QAction>
#include <QOpenGLFunctions>

class ImageRenderer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ImageRenderer(QWidget *parent = nullptr);
    ~ImageRenderer();
    void setImage(const QImage& image);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void handleMenuItemTriggered();
    void onImageSet(const QImage& image);

signals:

private:
    QOpenGLTexture *_texture = nullptr;
    QSize          _imageSize;
    QMenu          *_menu;
    void createRandomMenu(const QPoint& pos);
};

#endif // IMAGEDISPLAYWIDGET_H
