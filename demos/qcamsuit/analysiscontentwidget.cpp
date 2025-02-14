#include "analysiscontentwidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>

AnalysisContentWidget::AnalysisContentWidget(QWidget *parent)
    : QWidget(parent),
      videoWidget(new QVideoWidget(this)),
      mediaPlayer(new QMediaPlayer(this)),
      progressSlider(new QSlider(Qt::Horizontal, this)),
      statusLabel(new QLabel("00:00", this)),
      keyFrameListWidget(new QListWidget(this)),
      playButton(new QPushButton("播放", this)),
      pauseButton(new QPushButton("暂停", this)),
      fastForwardButton(new QPushButton("快进", this)),
      rewindButton(new QPushButton("快退", this)),
      speedComboBox(new QComboBox(this))
{
    createUI();

    mediaPlayer->setVideoOutput(videoWidget);
    connect(playButton, &QPushButton::clicked, this, &AnalysisContentWidget::play);
    connect(pauseButton, &QPushButton::clicked, this, &AnalysisContentWidget::pause);
    connect(fastForwardButton, &QPushButton::clicked, this, &AnalysisContentWidget::fastForward);
    connect(rewindButton, &QPushButton::clicked, this, &AnalysisContentWidget::rewind);
    connect(speedComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AnalysisContentWidget::changeSpeed);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, progressSlider, &QSlider::setValue);
    connect(progressSlider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);

    speedComboBox->addItems({"0.5x", "1x", "1.5x", "2x"});
    // mediaPlayer->setSource(QUrl::fromLocalFile("path_to_your_video_file"));  // 请替换为实际的视频文件路径
    // mediaPlayer->setMedia(QUrl::fromLocalFile("/Users/me/Music/coolsong.mp3"));
    mediaPlayer->setSource(QUrl::fromLocalFile("/Users/me/Music/coolsong.mp3"));
}

AnalysisContentWidget::~AnalysisContentWidget()
{
}

void AnalysisContentWidget::createUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout1 = new QVBoxLayout();
    layout1->addWidget(centralWidget);
    setLayout(layout1);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *topLayout = new QHBoxLayout();
    QVBoxLayout *rightLayout = new QVBoxLayout();
    QVBoxLayout *bottomLayout = new QVBoxLayout();

    // Video and controls layout
    topLayout->addWidget(videoWidget);
    topLayout->addLayout(rightLayout);

    // Video control buttons
    rightLayout->addWidget(playButton);
    rightLayout->addWidget(pauseButton);
    rightLayout->addWidget(fastForwardButton);
    rightLayout->addWidget(rewindButton);
    rightLayout->addWidget(new QLabel("播放速度", this));
    rightLayout->addWidget(speedComboBox);

    // Bottom layout for video timeline and key frames
    bottomLayout->addWidget(progressSlider);
    keyFrameListWidget->setFixedHeight(60);
    bottomLayout->addWidget(keyFrameListWidget);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(bottomLayout);

    setWindowTitle("视频裁剪");
    setFixedSize(800, 600);
}

void AnalysisContentWidget::play()
{
    mediaPlayer->play();
}

void AnalysisContentWidget::pause()
{
    mediaPlayer->pause();
}

void AnalysisContentWidget::fastForward()
{
    mediaPlayer->setPosition(mediaPlayer->position() + 10000);  // 快进10秒
}

void AnalysisContentWidget::rewind()
{
    mediaPlayer->setPosition(mediaPlayer->position() - 10000);  // 快退10秒
}

void AnalysisContentWidget::changeSpeed(int index)
{
    switch (index)
    {
    case 0:
        mediaPlayer->setPlaybackRate(0.5);
        break;
    case 1:
        mediaPlayer->setPlaybackRate(1.0);
        break;
    case 2:
        mediaPlayer->setPlaybackRate(1.5);
        break;
    case 3:
        mediaPlayer->setPlaybackRate(2.0);
        break;
    default:
        mediaPlayer->setPlaybackRate(1.0);
        break;
    }
}

