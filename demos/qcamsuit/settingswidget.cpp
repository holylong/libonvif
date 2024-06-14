#include "settingswidget.h"
#include <QVBoxLayout>
#include <QLabel>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(createThemeGroup());
    mainLayout->addWidget(createNotificationsGroup());
    mainLayout->addWidget(createVideoQualityGroup());
    mainLayout->addWidget(createPhotoQualityGroup());
    mainLayout->addWidget(createLayoutGroup());
    mainLayout->addStretch(1);

    setLayout(mainLayout);
    setWindowTitle("Settings");
    // setFixedSize(400, 600);
}

QGroupBox* SettingsWidget::createThemeGroup()
{
    QGroupBox *groupBox = new QGroupBox("Theme");

    QVBoxLayout *layout = new QVBoxLayout;
    themeCombo = new QComboBox;
    themeCombo->addItems({"Light", "Dark", "System Default"});

    QLabel *stLabel = new QLabel("Select Theme:");
    stLabel->setFixedWidth(105);
    layout->addWidget(stLabel);
    layout->addWidget(themeCombo);
    groupBox->setLayout(layout);

    return groupBox;
}

QGroupBox* SettingsWidget::createNotificationsGroup()
{
    QGroupBox *groupBox = new QGroupBox("Notifications");

    QVBoxLayout *layout = new QVBoxLayout;
    notificationsCheckbox = new QCheckBox("Enable Notifications");

    layout->addWidget(notificationsCheckbox);
    groupBox->setLayout(layout);

    return groupBox;
}

QGroupBox* SettingsWidget::createVideoQualityGroup()
{
    QGroupBox *groupBox = new QGroupBox("Video Quality");

    QVBoxLayout *layout = new QVBoxLayout;
    videoQualityCombo = new QComboBox;
    videoQualityCombo->addItems({"1080p", "720p", "480p"});

    QLabel *svqLabel = new QLabel("Select Video Quality:");
    svqLabel->setFixedWidth(155);
    layout->addWidget(svqLabel);
    layout->addWidget(videoQualityCombo);
    groupBox->setLayout(layout);

    return groupBox;
}

QGroupBox* SettingsWidget::createPhotoQualityGroup()
{
    QGroupBox *groupBox = new QGroupBox("Photo Quality");

    QVBoxLayout *layout = new QVBoxLayout;
    photoQualityCombo = new QComboBox;
    photoQualityCombo->addItems({"High", "Medium", "Low"});

    QLabel *spqLabel = new QLabel("Select Photo Quality:");
    spqLabel->setFixedWidth(155);
    layout->addWidget(spqLabel);
    layout->addWidget(photoQualityCombo);
    groupBox->setLayout(layout);

    return groupBox;
}

QGroupBox* SettingsWidget::createLayoutGroup()
{
    QGroupBox *groupBox = new QGroupBox("Multi-Screen Layout");

    QVBoxLayout *layout = new QVBoxLayout;
    layoutCombo = new QComboBox;
    layoutCombo->addItems({"Single Screen", "Split Screen", "Grid Layout"});

    QLabel *slLabel = new QLabel("Select Layout:");
    slLabel->setFixedWidth(105);
    layout->addWidget(slLabel);
    layout->addWidget(layoutCombo);
    groupBox->setLayout(layout);

    return groupBox;
}

SettingsWidget::Settings SettingsWidget::getSettings() const
{
    Settings settings;
    settings.theme = themeCombo->currentText();
    settings.notifications = notificationsCheckbox->isChecked();
    settings.videoQuality = videoQualityCombo->currentText();
    settings.photoQuality = photoQualityCombo->currentText();
    settings.layout = layoutCombo->currentText();
    return settings;
}
