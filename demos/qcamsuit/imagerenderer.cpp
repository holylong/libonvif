#include "imagerenderer.h"
#include <QRandomGenerator>
#include <QActionGroup>
#include <QMouseEvent>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QAction>

ImageRenderer::ImageRenderer(QWidget *parent)
    : QOpenGLWidget{parent}
{
    qDebug() << __FUNCTION__;
    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, &QWidget::customContextMenuRequested, this, &ImageRenderer::createRandomMenu);
}

ImageRenderer::~ImageRenderer()
{
    if (_texture != nullptr) {
        _texture->destroy();
        delete _texture;
    }
}

void ImageRenderer::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::RightButton){
        event->accept();
        createRandomMenu(event->globalPos());
    }else{
        QOpenGLWidget::mousePressEvent(event);
    }
}

void ImageRenderer::setImage(const QImage& image)
{
    if (image.isNull()) return;

    if (_texture != nullptr) {
        // _texture->destroy();
        // delete _texture
        // _texture = nullptr;
        _texture->bind();
        _texture->setData(image.mirrored().convertToFormat(QImage::Format_RGBA8888));
    }else{
        _texture = new QOpenGLTexture(image.convertToFormat(QImage::Format_RGBA8888));
    }
    _imageSize = image.size();
    update(); // 请求重绘
}

void ImageRenderer::initializeGL()
{
    initializeOpenGLFunctions();
}

void ImageRenderer::paintGL()
{
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(_texture != nullptr && !_imageSize.isEmpty()){
        _texture->bind();

        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rgbFrame.cols, rgbFrame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbFrame.data);

        glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2d(0, 0);
        glTexCoord2d(1, 1); glVertex2d(_imageSize.width(), 0);
        glTexCoord2d(1, 0); glVertex2d(_imageSize.width(), _imageSize.height());
        glTexCoord2d(0, 0); glVertex2d(0, _imageSize.height());
        glEnd();
        _texture->release();
    }
}

void ImageRenderer::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void ImageRenderer::onImageSet(const QImage& image)
{
    qDebug() << __FUNCTION__;
    setImage(image);
}

void ImageRenderer::handleMenuItemTriggered()
{
    qDebug() << "Selected:";
}

void ImageRenderer::createRandomMenu(const QPoint& pos)
{
    _menu = new QMenu();

    for(int i = 0; i < 5; ++i){
        QString randomText = QStringLiteral("Menu %1").arg(i+1);
        QAction *menuItem = new QAction(randomText, this);

        _menu->addAction(menuItem);

        QMenu *sbuMenu = new QMenu(randomText + " SubMenu", this);
        for(int j = 0; j < 3; j++){
            QString subItemText = QStringLiteral("Sub Item %1").arg(j+1);
            QAction *subAction = new QAction(subItemText, this);
            QObject::connect(subAction, &QAction::triggered, this, &ImageRenderer::handleMenuItemTriggered);
            sbuMenu->addAction(subAction);
        }
    }
    _menu->exec(pos);
    delete _menu;
}
