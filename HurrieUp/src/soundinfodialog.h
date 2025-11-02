#ifndef SOUNDINFODIALOG_H
#define SOUNDINFODIALOG_H

#include <QDialog>
#include <QString>

struct SoundInfo;

class SoundInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SoundInfoDialog(const SoundInfo &sound, QWidget *parent = nullptr);
    ~SoundInfoDialog();
};

#endif // SOUNDINFODIALOG_H
