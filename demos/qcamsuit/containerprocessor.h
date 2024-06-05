#ifndef CONTAINERPROCESSOR_H
#define CONTAINERPROCESSOR_H

#include <QObject>

class ContainerProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ContainerProcessor(QObject *parent = nullptr);

signals:
};

#endif // CONTAINERPROCESSOR_H
