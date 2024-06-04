#include "iconbuttongroup.h"
#include "iconbutton.h"
#include <QVBoxLayout>

IconButtonGroup::IconButtonGroup(QWidget *parent)
    : QWidget{parent}
{
    setStyleSheet("QWidget {background: red;}");
    initButtonGroup();
}


void IconButtonGroup::initButtonGroup()
{
    // setStyleSheet("QWidget {"
    //                 "border-left: 2px solid red;"
    //                 "border-right: 2px solid gray;"
    //                 "border-top: 2px solid gray;"
    //                 "border-bottom: 2px solid gray;"
    //               "}");
    _mainLayout = new QVBoxLayout();
    _mainLayout->setAlignment(Qt::AlignTop);
    setLayout(_mainLayout);

    // setFixedWidth(100);
    // setFixedHeight(50);
}


int IconButtonGroup::AppendButton(IconButton* bn)
{
    _arrBnt.append(bn);
    //bn->setFixedSize(80, 80);
    _mainLayout->addWidget(bn);
    return _arrBnt.size();
}
