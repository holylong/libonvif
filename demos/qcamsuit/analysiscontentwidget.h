#ifndef ANALYSISCONTENTWIDGET_H
#define ANALYSISCONTENTWIDGET_H

#include <QWidget>
#include <QTreeWidget>

class AnalysisContentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnalysisContentWidget(QWidget *parent = nullptr);

signals:

private:
    QTreeWidget *_treeWidget;
};

#endif // ANALYSISCONTENTWIDGET_H
