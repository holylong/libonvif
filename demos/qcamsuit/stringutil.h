#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <QStringList>
#include <QDebug>

class StringUtil{
public:
    static void Display(const QStringList& ss){
        for(QString s : ss)
            qDebug() << s;
    }

    static QStringList RepeatStringDel(const QStringList& list){
        // 将QStringList转换为QSet自动去重，然后再转换回QStringList
        QSet<QString> set(list.begin(), list.end());
        return QStringList(set.begin(), set.end());
    }

    static QVector<QString> RepeatStringDel(const QVector<QString>& list){
        QSet<QString> uniqueSet(list.begin(), list.end());
        return QVector<QString>(uniqueSet.begin(), uniqueSet.end());
    }
};

#endif // STRINGUTIL_H
