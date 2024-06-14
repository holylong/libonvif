#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);

    struct Settings {
        QString theme;
        bool notifications;
        QString videoQuality;
        QString photoQuality;
        QString layout;
    };

    Settings getSettings() const;

private:
    QGroupBox* createThemeGroup();
    QGroupBox* createNotificationsGroup();
    QGroupBox* createVideoQualityGroup();
    QGroupBox* createPhotoQualityGroup();
    QGroupBox* createLayoutGroup();

    QComboBox *themeCombo;
    QCheckBox *notificationsCheckbox;
    QComboBox *videoQualityCombo;
    QComboBox *photoQualityCombo;
    QComboBox *layoutCombo;
};

#endif // SETTINGSWIDGET_H
