#include "mainwindow.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QCoreApplication>
#include <QSettings>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Hide the main window, as this is a tray application
    setWindowFlags(Qt::Tool);
    hide();

    settingsDialog = new SettingsDialog(this);
    aboutDialog = new AboutDialog(this);
    
    loadSettings(); // Load settings first to get paths and volumes
    loadSoundFiles();
    createTrayIcon();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::playSounds);
    
    pauseTimer = new QTimer(this);
    pauseTimer->setSingleShot(true);
    connect(pauseTimer, &QTimer::timeout, this, &MainWindow::resume);

    ringPlayer = new QMediaPlayer(this);
    ringAudioOutput = new QAudioOutput(this);
    ringPlayer->setAudioOutput(ringAudioOutput);
    connect(ringPlayer, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString){
        qDebug() << "Ring player error:" << error << errorString;
    });

    voicePlayer = new QMediaPlayer(this);
    voiceAudioOutput = new QAudioOutput(this);
    voicePlayer->setAudioOutput(voiceAudioOutput);
    connect(voicePlayer, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString){
        qDebug() << "Voice player error:" << error << errorString;
    });

    completePlayer = new QMediaPlayer(this);
    completeAudioOutput = new QAudioOutput(this);
    completePlayer->setAudioOutput(completeAudioOutput);
    connect(completePlayer, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString){
        qDebug() << "Complete player error:" << error << errorString;
    });

    updateSoundSources(); // Set initial sound sources

    timer->start(intervalMinutes * 60 * 1000);
    
    // Play sounds immediately on startup for testing
    QTimer::singleShot(2000, this, &MainWindow::playSounds);
    
    qDebug() << "MainWindow initialized. Timer started with interval:" << intervalMinutes << "minutes.";
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::updateSoundSources()
{
    qDebug() << "Updating sound sources. Mode:" << (isCompleteMode ? "Complete" : "Combination");
    
    // Set volume for all players
    completeAudioOutput->setVolume(volume / 100.0);
    ringAudioOutput->setVolume(volume / 100.0);
    voiceAudioOutput->setVolume(volume / 100.0);
    
    qDebug() << "Complete paths:" << completePaths;
    qDebug() << "Ring paths:" << ringPaths;
    qDebug() << "Voice paths:" << voicePaths;
}

void MainWindow::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    // You should create an icon for your application and add it to resources
    // For now, we use a standard icon
    trayIcon->setIcon(QIcon(":/resources/icon.png"));
    trayIcon->setToolTip(tr("HurrieUp"));

    pauseMenu = new QMenu(tr("Pause"), this);
    QList<int> pauseMinutes = {30, 60, 120, 180, 360, 540, 720};
    for (int min : pauseMinutes) {
        QAction *pauseAction = new QAction(tr("%1 minutes").arg(min), this);
        connect(pauseAction, &QAction::triggered, this, [this, min](){ pause(min); });
        pauseMenu->addAction(pauseAction);
    }

    resumeAction = new QAction(tr("Resume"), this);
    connect(resumeAction, &QAction::triggered, this, &MainWindow::resume);
    resumeAction->setEnabled(false);

    settingsAction = new QAction(tr("Settings"), this);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::showSettings);

    aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);

    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addMenu(pauseMenu);
    trayIconMenu->addAction(resumeAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(settingsAction);
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        showSettings();
    }
}

void MainWindow::showSettings()
{
    settingsDialog->setInterval(intervalMinutes);
    settingsDialog->setVolume(volume);
    settingsDialog->setSoundGap(soundGap);
    settingsDialog->setIsCompleteMode(isCompleteMode);
    settingsDialog->setCompletePaths(completePaths);
    settingsDialog->setRingPaths(ringPaths);
    settingsDialog->setVoicePaths(voicePaths);

    qDebug() << "Opening settings dialog. Mode:" << (isCompleteMode ? "Complete" : "Combination");
    qDebug() << "Complete:" << completePaths << "Ring:" << ringPaths << "Voice:" << voicePaths;

    if (settingsDialog->exec() == QDialog::Accepted)
    {
        intervalMinutes = settingsDialog->interval();
        volume = settingsDialog->volume();
        soundGap = settingsDialog->soundGap();
        isCompleteMode = settingsDialog->isCompleteMode();
        completePaths = settingsDialog->completePaths();
        ringPaths = settingsDialog->ringPaths();
        voicePaths = settingsDialog->voicePaths();

        qDebug() << "Settings accepted. Mode:" << (isCompleteMode ? "Complete" : "Combination");
        qDebug() << "New Complete:" << completePaths << "New Ring:" << ringPaths << "New Voice:" << voicePaths;

        timer->setInterval(intervalMinutes * 60 * 1000);
        updateSoundSources(); // Update sound sources when settings change
        saveSettings();
    }
}

void MainWindow::showAbout()
{
    aboutDialog->exec();
}

void MainWindow::playSounds()
{
    qDebug() << "playSounds called. Mode:" << (isCompleteMode ? "Complete" : "Combination");
    
    if (isCompleteMode) {
        if (!completePaths.isEmpty()) {
            // Pick a random sound from selected complete sounds
            int randomIndex = QRandomGenerator::global()->bounded(completePaths.size());
            QString selectedPath = completePaths[randomIndex];
            qDebug() << "Playing random complete sound:" << selectedPath;
            completePlayer->setSource(QUrl(selectedPath));
            completePlayer->play();
        } else {
            qDebug() << "No complete sounds selected!";
        }
    } else {
        if (!ringPaths.isEmpty() && !voicePaths.isEmpty()) {
            // Pick random ring and voice sounds
            int ringIndex = QRandomGenerator::global()->bounded(ringPaths.size());
            int voiceIndex = QRandomGenerator::global()->bounded(voicePaths.size());
            
            QString selectedRing = ringPaths[ringIndex];
            QString selectedVoice = voicePaths[voiceIndex];
            
            qDebug() << "Playing random ring sound:" << selectedRing;
            ringPlayer->setSource(QUrl(selectedRing));
            ringPlayer->play();

            // Play voice after a delay
            QTimer::singleShot(soundGap, this, [this, selectedVoice](){
                qDebug() << "Playing random voice after" << soundGap << "ms delay:" << selectedVoice;
                voicePlayer->setSource(QUrl(selectedVoice));
                voicePlayer->play();
            });
        } else {
            qDebug() << "No ring or voice sounds selected!";
        }
    }
}

void MainWindow::pause(int minutes)
{
    timer->stop();
    resumeAction->setEnabled(true);
    trayIcon->setToolTip(tr("HurrieUp (Paused)"));

    if (minutes > 0) {
        pauseTimer->start(minutes * 60 * 1000);
    }
}

void MainWindow::resume()
{
    if (pauseTimer->isActive()) {
        pauseTimer->stop();
    }
    timer->start(intervalMinutes * 60 * 1000);
    resumeAction->setEnabled(false);
    trayIcon->setToolTip(tr("HurrieUp"));
}

void MainWindow::loadSoundFiles()
{
    QList<SoundInfo> rings, voices, completes;
    loadBundledSounds(":/resources/ring/bundled_rings.json", rings);
    loadBundledSounds(":/resources/voice/bundled_voices.json", voices);
    loadBundledSounds(":/resources/complete/bundled_complete_sounds.json", completes);
    settingsDialog->populateSoundLists(rings, voices, completes);
}

void MainWindow::loadBundledSounds(const QString &jsonPath, QList<SoundInfo> &soundList)
{
    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open sound file:" << jsonPath;
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject json = doc.object();

    // Determine the base path for sound files
    QString basePath;
    if (jsonPath.contains("ring")) {
        basePath = "qrc:/resources/ring/";
    } else if (jsonPath.contains("voice")) {
        basePath = "qrc:/resources/voice/";
    } else if (jsonPath.contains("complete")) {
        basePath = "qrc:/resources/complete/";
    }

    // Parse the JSON structure
    for (const QString& key : json.keys()) {
        QJsonArray soundArray = json[key].toArray();
        for (const QJsonValue &value : soundArray) {
            QJsonObject soundObj = value.toObject();
            SoundInfo info;
            info.name = soundObj["name"].toString();
            info.description = soundObj["description"].toString();
            info.author = soundObj["author"].toString();
            info.license = soundObj["license"].toString();
            info.path = basePath + soundObj["file"].toString();
            info.isBundled = true;
            soundList.append(info);
            qDebug() << "Loaded sound:" << info.name << "->" << info.path;
        }
    }
}


void MainWindow::loadSettings()
{
    QSettings settings("YourCompany", "HurrieUp");
    intervalMinutes = settings.value("interval", 10).toInt();
    volume = settings.value("volume", 85).toInt();
    soundGap = settings.value("soundGap", 1000).toInt();
    isCompleteMode = settings.value("isCompleteMode", false).toBool();
    
    // Load QStringList, with fallback to old single-value format for backward compatibility
    completePaths = settings.value("completePaths").toStringList();
    if (completePaths.isEmpty()) {
        QString oldPath = settings.value("completePath", "qrc:/resources/complete/elechurry.mp3").toString();
        if (!oldPath.isEmpty()) {
            completePaths << oldPath;
        }
    }
    
    ringPaths = settings.value("ringPaths").toStringList();
    if (ringPaths.isEmpty()) {
        QString oldPath = settings.value("ringPath", "qrc:/resources/ring/new_message.wav").toString();
        if (!oldPath.isEmpty()) {
            ringPaths << oldPath;
        }
    }
    
    voicePaths = settings.value("voicePaths").toStringList();
    if (voicePaths.isEmpty()) {
        QString oldPath = settings.value("voicePath", "qrc:/resources/voice/clovahurry_enhanced.wav").toString();
        if (!oldPath.isEmpty()) {
            voicePaths << oldPath;
        }
    }
    
    qDebug() << "Settings loaded - Mode:" << (isCompleteMode ? "Complete" : "Combination");
    qDebug() << "Complete:" << completePaths << "Ring:" << ringPaths << "Voice:" << voicePaths;
}

void MainWindow::saveSettings()
{
    QSettings settings("YourCompany", "HurrieUp");
    settings.setValue("interval", intervalMinutes);
    settings.setValue("volume", volume);
    settings.setValue("soundGap", soundGap);
    settings.setValue("isCompleteMode", isCompleteMode);
    settings.setValue("completePaths", completePaths);
    settings.setValue("ringPaths", ringPaths);
    settings.setValue("voicePaths", voicePaths);
}
