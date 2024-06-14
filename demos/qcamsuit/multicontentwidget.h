#ifndef MULTICONTENTWIDGET_H
#define MULTICONTENTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include "imagerenderer.h"

// class ImageRenderer;

class MultiContentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MultiContentWidget(QWidget *parent = nullptr);

    void setGridSize(int rows, int cols);
    void setUrls(const QVector<QString>& ruls);

    std::pair<int, int> getOptimalDivision(int n);

private slots:
    void adjustViewSize(ImageRenderer *view) {
        // 根据父窗口大小动态调整每个监控画面的大小
        qDebug() << _numRows << " " << _numCols;
        int columnWidth = width() / _numRows;
        int rowHeight = height() / _numCols;
        view->setFixedSize(columnWidth, rowHeight);
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        if(!ignoreResizeEvent){
            // 当父窗口大小变化时，触发每个监控画面的大小调整
            for (int i = 0; i < layout()->count(); ++i) {
                QWidget *widget = layout()->itemAt(i)->widget();
                adjustViewSize(static_cast<ImageRenderer*>(widget));
            }
        }
        QWidget::resizeEvent(event);
    }

signals:
    void resized(QResizeEvent *event);
private:
    QVector<TextureWidget*>  _renderers;
    std::vector<QLabel*> _cameraViews;

    bool ignoreResizeEvent = true;

    void updateLayout();
    void clearLayout();

    QGridLayout     *_layout;
    QVector<QString> _urls;

    int              _numRows{1};
    int              _numCols{1};
};

#endif // MULTICONTENTWIDGET_H
