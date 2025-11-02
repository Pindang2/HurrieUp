#include "soundinfodialog.h"
#include "settingsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDateTime>

SoundInfoDialog::SoundInfoDialog(const SoundInfo &sound, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Sound Information"));
    setMinimumWidth(400);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Sound name
    QLabel *nameLabel = new QLabel(QString("<b>%1: </b>%2").arg(tr("Name"), sound.name));
    nameLabel->setWordWrap(true);
    mainLayout->addWidget(nameLabel);
    
    // Description
    QLabel *descLabel = new QLabel(QString("<b>%1: </b>%2").arg(tr("Description"), sound.description));
    descLabel->setWordWrap(true);
    mainLayout->addWidget(descLabel);

    // If sound is bundled, add author and license info
    if (sound.isBundled) {
        if (!sound.author.isEmpty()) {
            QLabel *authorLabel = new QLabel(QString("<b>%1: </b>%2").arg(tr("Author"), sound.author));
            authorLabel->setWordWrap(true);
            mainLayout->addWidget(authorLabel);
        }

        if (!sound.license.isEmpty()) {
            QLabel *licenseLabel = new QLabel(QString("<b>%1: </b>%2").arg(tr("License"), sound.license));
            licenseLabel->setWordWrap(true);
            mainLayout->addWidget(licenseLabel);
        }
    } else {
        // Only show file info for user-added sounds
        // File path
        QLabel *pathLabel = new QLabel(QString("<b>%1: </b>%2").arg(tr("File Path"), sound.path));
        pathLabel->setWordWrap(true);
        pathLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        mainLayout->addWidget(pathLabel);
        
        // File info
        QFileInfo fileInfo(sound.path);
        if (fileInfo.exists()) {
            // File size
            qint64 sizeBytes = fileInfo.size();
            QString sizeStr;
            if (sizeBytes < 1024) {
                sizeStr = QString("%1 B").arg(sizeBytes);
            } else if (sizeBytes < 1024 * 1024) {
                sizeStr = QString("%1 KB").arg(sizeBytes / 1024.0, 0, 'f', 2);
            } else {
                sizeStr = QString("%1 MB").arg(sizeBytes / (1024.0 * 1024.0), 0, 'f', 2);
            }
            QLabel *sizeLabel = new QLabel(QString("<b>%1: </b>%2").arg(tr("File Size"), sizeStr));
            mainLayout->addWidget(sizeLabel);
            
            // Last modified date
            QDateTime modified = fileInfo.lastModified();
            QLabel *dateLabel = new QLabel(QString("<b>%1: </b>%2").arg(tr("Last Modified"), modified.toString("yyyy-MM-dd hh:mm:ss")));
            mainLayout->addWidget(dateLabel);
            
            // File format
            QString format = fileInfo.suffix().toUpper();
            QLabel *formatLabel = new QLabel(QString("<b>%1: </b>%2").arg(tr("Format"), format));
            mainLayout->addWidget(formatLabel);
        } else {
            QLabel *errorLabel = new QLabel(tr("⚠ File not found"));
            errorLabel->setStyleSheet("QLabel { color: red; }");
            mainLayout->addWidget(errorLabel);
        }
    }
    
    // Sound type
    QString typeStr = sound.isBundled ? tr("Bundled") : tr("User-added");
    QLabel *typeLabel = new QLabel(QString("<b>%1: </b>%2").arg(tr("Type"), typeStr));
    mainLayout->addWidget(typeLabel);
    
    mainLayout->addSpacing(10);
    
    // Close button
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    QPushButton *closeButton = new QPushButton(tr("Close"));
    closeButton->setMinimumWidth(80);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addLayout(buttonLayout);
}

SoundInfoDialog::~SoundInfoDialog()
{
}
