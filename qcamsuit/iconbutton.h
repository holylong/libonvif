#ifndef _ICONBUTTON_H
#define _ICONBUTTON_H

#include <QApplication>
#include <QVBoxLayout>
#include <QToolButton>
#include <QIcon>
#include <QColor>
#include <QPalette>

class IconButton : public QToolButton {
    Q_OBJECT

public:
    IconButton(const QString &text, const QString &iconPath, QWidget *parent = nullptr)
        : QToolButton(parent) {

        // 设置图标
        setIcon(QIcon(iconPath));
        setIconSize(QSize(32, 32)); // 设置图标大小

        // 设置文本和图标的位置
        setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        //        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        //        setAttribute(Qt::WA_TranslucentBackground, true);

        setStyleSheet("QToolButton {background: transparent;}");

        // layout()->setAlignment(Qt::AlignHCenter);

        QPalette pal = palette();
        pal.setColor(QPalette::Button, QColor(0, 0, 0, 0)); // 使用RGBA，最后一个参数为透明度
        setAutoFillBackground(true);
        setPalette(pal);

           setStyleSheet("QToolButton {"
                             "border: none;"
                             "border-radius: 6px;"
                             "background-color: transparent;"
                             "}"
                             "QToolButton::hover {"
                             "background-color: #e5e5e5;"
                             "color: #000000;"
                             "}"
                             "QToolButton::pressed {"
                             "background-color: #cccccc;"
                             "}"
                             "QToolButton::checked {"
                             "background-color: #a1a1a1;"
                             "}");

        // 设置文本
        setText(text);

        // 连接点击事件
        connect(this, &QToolButton::clicked, this, &IconButton::handleClick);
    }

    void setWidthExpanding(){
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    }

    void setClickTheme(){
        QPalette pal = palette();
        pal.setColor(QPalette::Button, QColor(155, 0, 0, 100)); // 使用RGBA，最后一个参数为透明度
        setAutoFillBackground(true);
        setPalette(pal);

        setStyleSheet("QToolButton {"
                      "border-left: 2px solid gray;"
                      "border-top: 2px solid gray;"
                      "border-bottom: 2px solid gray;"
                      "border-radius: 6px;"
                      "background-color: transparent;"
                      "}"
                      "QToolButton::hover {"
                      "background-color: #e5e5e5;"
                      "color: #000000;"
                      "}"
                      "QToolButton::pressed {"
                      "background-color: #cccccc;"
                      "}"
                      "QToolButton::checked {"
                      "background-color: #a1a1a1;"
                      "}");
    }

    void resetClickTheme(){
        QPalette pal = palette();
        pal.setColor(QPalette::Button, QColor(0, 0, 0, 0)); // 使用RGBA，最后一个参数为透明度
        setAutoFillBackground(true);
        setPalette(pal);

        setStyleSheet("QToolButton {"
                      "border: none;"
                      "border-radius: 6px;"
                      "background-color: transparent;"
                      "}"
                      "QToolButton::hover {"
                      "background-color: #e5e5e5;"
                      "color: #000000;"
                      "}"
                      "QToolButton::pressed {"
                      "background-color: #cccccc;"
                      "}"
                      "QToolButton::checked {"
                      "background-color: #a1a1a1;"
                      "}");
    }

signals:
    void clicked(); // 自定义点击信号，可以在外部连接到这个信号

private slots:
    void handleClick() {
        emit clicked();
    }
};

#endif
