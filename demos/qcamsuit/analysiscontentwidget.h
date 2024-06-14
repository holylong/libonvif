#ifndef ANALYSISCONTENTWIDGET_H
#define ANALYSISCONTENTWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QComboBox>
#include <QMediaPlayer>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QListWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QVideoWidget>

class AnalysisContentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnalysisContentWidget(QWidget *parent = nullptr);
    ~AnalysisContentWidget();

private slots:
    void play();
    void pause();
    void fastForward();
    void rewind();
    void changeSpeed(int index);

private:
    QVideoWidget *videoWidget;
    QMediaPlayer *mediaPlayer;
    QSlider *progressSlider;
    QLabel *statusLabel;
    QListWidget *keyFrameListWidget;

    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *fastForwardButton;
    QPushButton *rewindButton;
    QComboBox *speedComboBox;

    void createUI();

private:
    QTreeWidget *_treeWidget;
};

#endif // ANALYSISCONTENTWIDGET_H
