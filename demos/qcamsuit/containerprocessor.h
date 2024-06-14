#ifndef CONTAINERPROCESSOR_H
#define CONTAINERPROCESSOR_H

#include <QObject>
#include <QSet>

class ContainerProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ContainerProcessor(QObject *parent = nullptr);

    static QSet<QString> combineString(QSet<QString>& dst, const QSet<QString> org){
        return dst.unite(org);
    }

    static QStringList vectorToStringList(const std::vector<std::string>& vecStr) {
        QStringList strList;
        for(const std::string& s : vecStr) {
            strList << QString::fromStdString(s);
        }
        return strList;
    }

signals:
};

#endif // CONTAINERPROCESSOR_H
