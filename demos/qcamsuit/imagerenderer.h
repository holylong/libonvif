#ifndef IMAGEDISPLAYWIDGET_H
#define IMAGEDISPLAYWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QMenu>
#include <QImage>
#include <QOpenGLTexture>
#include <QAction>
#include <QOpenGLFunctions>
#include <QtConcurrent>

#include "rtspcontroller.h"

class ImageRenderer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ImageRenderer(QWidget *parent = nullptr);
    ~ImageRenderer();
    void setImage(const QImage& image);

    QString getUrl() const {return _url;}
    void setUrl(const QString& url){
        qDebug() << "rtsp url:" << url;
        _url = url;
        if(_controller == nullptr){
            _controller = new RtspController();
            QObject::connect(_controller, &RtspController::onFrame, this, &ImageRenderer::onImageSet);
        }

        _controller->stopRtsp();
        QtConcurrent::run([this]() {
            _controller->openRtsp(_url);
        });
    }

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
    QString        _url{""};
    RtspController *_controller{nullptr};
    void createRandomMenu(const QPoint& pos);
};

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QImage>

class TextureWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit TextureWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent), texture(nullptr) {}
    ~TextureWidget() { delete texture; }

    QString getUrl() const {return _url;}
    void setUrl(const QString& url){
        qDebug() << "rtsp url:" << url;
        _url = url;
        if(_controller == nullptr){
            _controller = new RtspController();
            QObject::connect(_controller, &RtspController::onFrame, this, &TextureWidget::onImageSet);
        }

        _controller->stopRtsp();
        QtConcurrent::run([this]() {
            _controller->openRtsp(_url);
        });
    }

protected:
    void initializeGL() override
    {
        initializeOpenGLFunctions();

        // 初始化着色器
        shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/res/shaders/vertex.glsl");
        shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/res/shaders/fragment.glsl");
        shaderProgram.link();

        // 初始化纹理
        texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        texture->create();
        texture->setMinificationFilter(QOpenGLTexture::Linear);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
        texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);

        // 配置顶点缓冲区和顶点数组对象
        GLfloat vertices[] = {
            // 顶点位置     // 纹理坐标
            -1.0f, -1.0f,   0.0f, 0.0f,
            1.0f, -1.0f,   1.0f, 0.0f,
            -1.0f,  1.0f,   0.0f, 1.0f,
            1.0f,  1.0f,   1.0f, 1.0f,
        };

        vbo.create();
        vbo.bind();
        vbo.allocate(vertices, sizeof(vertices));

        vao.create();
        vao.bind();

        shaderProgram.enableAttributeArray(0);
        shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));

        shaderProgram.enableAttributeArray(1);
        shaderProgram.setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));

        vao.release();
        vbo.release();
    }

    void resizeGL(int w, int h) override
    {
        glViewport(0, 0, w, h);
    }

    void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.bind();
        texture->bind();

        vao.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        vao.release();

        texture->release();
        shaderProgram.release();
    }

public slots:
    void updateTexture(const QImage &newImage)
    {
        if (texture)
        {
            // 检查纹理尺寸是否与图像一致，如果不一致则需要重新创建纹理
            QSize textureSize = QSize(texture->width(), texture->height());
            qDebug() << "texture width:" << textureSize.width() << " height:" << texture->height();
            if (textureSize.width() != newImage.width() || textureSize.height() != newImage.height())
            {
                qDebug() << "recreate texture";
                // 先删除旧纹理
                texture->destroy();
                delete texture;
                texture = nullptr;

                // 重新创建纹理并设置属性
                texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
                texture->create();
                texture->setMinificationFilter(QOpenGLTexture::Linear);
                texture->setMagnificationFilter(QOpenGLTexture::Linear);
                texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
                texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);
            }

            texture->bind();
            // texture->setData(newImage.mirrored().convertToFormat(QImage::Format_ARGB32));
            texture->setData(newImage.mirrored());
            update();
        }
    }

    void onImageSet(const QImage& image){
        updateTexture(image);
    }

private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLTexture *texture;
    RtspController *_controller{nullptr};
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;
    QString        _url{""};
};

#endif // IMAGEDISPLAYWIDGET_H
