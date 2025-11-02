#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "settingsdialog.h"
#include "aboutdialog.h"

class QAction;
class QMenu;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showSettings();
    void showAbout();
    void playSounds();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void pause(int minutes);
    void resume();
    void updateSoundSources();

private:
    void createTrayIcon();
    void loadSettings();
    void saveSettings();
    void loadSoundFiles();
    void loadBundledSounds(const QString &jsonPath, QList<SoundInfo> &soundList);

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QMenu *pauseMenu;
    QAction *resumeAction;
    QAction *settingsAction;
    QAction *aboutAction;
    QAction *quitAction;

    SettingsDialog *settingsDialog;
    AboutDialog *aboutDialog;

    QTimer *timer;
    QTimer *pauseTimer;
    QMediaPlayer *ringPlayer;
    QAudioOutput *ringAudioOutput;
    QMediaPlayer *voicePlayer;
    QAudioOutput *voiceAudioOutput;
    QMediaPlayer *completePlayer;
    QAudioOutput *completeAudioOutput;

    // Settings
    int intervalMinutes = 10;
    bool isCompleteMode = false;
    QStringList completePaths;
    QStringList ringPaths;
    QStringList voicePaths;
    int volume = 85;
    int soundGap = 1000;
};

#endif // MAINWINDOW_H
