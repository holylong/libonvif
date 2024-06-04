#ifndef ICONBUTTONGROUP_H
#define ICONBUTTONGROUP_H

#include <QWidget>
#include <QVector>
#include <QDebug>

class IconButton;
class QVBoxLayout;

class IconButtonGroup : public QWidget
{
    Q_OBJECT
public:
    explicit IconButtonGroup(QWidget *parent = nullptr);

    int AppendButton(IconButton* bn);

    void setClickTheme(){
        qDebug() << __FUNCTION__;
        QPalette pal = palette();
        pal.setColor(QPalette::Window, QColor(155, 155, 0, 100)); // 使用RGBA，最后一个参数为透明度
        setAutoFillBackground(true);
        setPalette(pal);
    }

    void resetClickTheme(){
        QPalette pal = palette();
        pal.setColor(QPalette::Window, QColor(0, 0, 0, 0)); // 使用RGBA，最后一个参数为透明度
        setAutoFillBackground(true);
        setPalette(pal);
    }
private:
    void initButtonGroup();
signals:

private:
    QVector<IconButton*> _arrBnt;
    QVBoxLayout *_mainLayout;
};

#endif // ICONBUTTONGROUP_H
