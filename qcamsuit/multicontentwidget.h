#ifndef MULTICONTENTWIDGET_H
#define MULTICONTENTWIDGET_H

#include <QWidget>

class ImageRendderer;

class MultiContentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MultiContentWidget(QWidget *parent = nullptr);

signals:

private:
    ImageRendderer  *_renderer;
};

#endif // MULTICONTENTWIDGET_H
