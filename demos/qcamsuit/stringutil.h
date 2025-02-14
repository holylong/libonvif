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
    
    template<typename Container>
    static Container RepeatStringDel(const Container& list) {
        QSet<typename Container::value_type> uniqueSet(list.begin(), list.end());
        return Container(uniqueSet.begin(), uniqueSet.end());
    }
};

#endif // STRINGUTIL_H
