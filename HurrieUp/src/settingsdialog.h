#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QCheckBox>
#include <QVBoxLayout>

namespace Ui {
class SettingsDialog;
}

struct SoundInfo {
    QString name;
    QString description;
    QString path;
    QString author;
    QString license;
    bool isBundled = true;  // true for bundled sounds, false for user-added
    
    bool operator==(const SoundInfo &other) const {
        return path == other.path;
    }
};

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    void setInterval(int minutes);
    int interval() const;

    void setVolume(int volume);
    int volume() const;

    void setSoundGap(int ms);
    int soundGap() const;

    void setRingPaths(const QStringList &paths);
    QStringList ringPaths() const;

    void setVoicePaths(const QStringList &paths);
    QStringList voicePaths() const;

    void setCompletePaths(const QStringList &paths);
    QStringList completePaths() const;

    void setIsCompleteMode(bool isComplete);
    bool isCompleteMode() const;

    void populateSoundLists(const QList<SoundInfo> &rings, const QList<SoundInfo> &voices, const QList<SoundInfo> &completes);

    void loadUserSounds();
    void saveUserSounds();

private slots:
    void onModeChanged();
    void onPreviewClicked();
    void onAddRingClicked();
    void onAddVoiceClicked();
    void onAddCompleteClicked();

private:
    void updateUIForMode();
    void stopPreview();
    void createSoundCheckboxList(QVBoxLayout *layout, const QList<SoundInfo> &sounds, QList<QCheckBox*> &checkboxes);
    QString copyToUserSoundsFolder(const QString &sourcePath, const QString &category);
    void addUserSound(const QString &category);
    void refreshSoundLists();
    void showSoundInfo(const SoundInfo &sound);

    Ui::SettingsDialog *ui;
    QList<SoundInfo> ringList;
    QList<SoundInfo> voiceList;
    QList<SoundInfo> completeList;
    
    QList<SoundInfo> userRings;
    QList<SoundInfo> userVoices;
    QList<SoundInfo> userCompletes;
    
    QList<QCheckBox*> ringCheckboxes;
    QList<QCheckBox*> voiceCheckboxes;
    QList<QCheckBox*> completeCheckboxes;
    
    QMediaPlayer *previewPlayer1;
    QAudioOutput *previewAudioOutput1;
    QMediaPlayer *previewPlayer2;
    QAudioOutput *previewAudioOutput2;
};

#endif // SETTINGSDIALOG_H
