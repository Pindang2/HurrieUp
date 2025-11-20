#include "aboutdialog.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QDesktopServices>
#include <QUrl>
#include <QSpacerItem>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle(tr("About HurrieUp"));
    setFixedSize(480, 520);
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(15);

    // Title
    titleLabel = new QLabel("HurrieUp!", this);
    QFont titleFont;
    titleFont.setPointSize(48);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Version
    versionLabel = new QLabel("Community Edition Beta 1.0.1\nBy Pindang2", this);
    QFont versionFont;
    versionFont.setPointSize(16);
    versionFont.setBold(true);
    versionLabel->setFont(versionFont);
    versionLabel->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(versionLabel);

    // Author
    // authorLabel = new QLabel("By Pindang2", this);
    // QFont authorFont;
    // authorFont.setPointSize(16);
    // authorFont.setBold(true);
    // authorLabel->setFont(authorFont);
    // authorLabel->setAlignment(Qt::AlignLeft);
    // mainLayout->addWidget(authorLabel);

    mainLayout->addSpacing(10);

    // Description text
    descriptionLabel = new QLabel(this);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setTextFormat(Qt::RichText);
    descriptionLabel->setOpenExternalLinks(true);
    
    QString descriptionText = tr(
        "<p>이 소프트웨어는 LGPL 라이선스 하에 배포되는<br/>"
        "오픈소스 소프트웨어입니다.</p>"
        "<p>기본적으로 개조개작/상업적 이용은 가능하나<br/>"
        "일부 포함된 보이스 또는 사운드는 그렇지 않으므로<br/>"
        "방송 또는 온라인 컨텐츠 제작에는 SUPERTONE의<br/>"
        "보이스를 포함하지 않도록 주의해 주십시오.</p>"
        "<p>직접 녹음하거나 방송에서 사용하기 괜찮은 보이스<br/>"
        "파일을 작접 추가하여 사용에 주시기 바랍니다.<br/>"
        "이 외의 사용상 주의사항은 <a href='https://github.com/Pindang2/HurrieUp'>여기</a>를 읽어 주십시오.</p>"
    );
    
    QFont descFont;
    descFont.setPointSize(11);
    descriptionLabel->setFont(descFont);
    descriptionLabel->setText(descriptionText);
    descriptionLabel->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(descriptionLabel);

    mainLayout->addStretch();

    // Buttons layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    licenseButton = new QPushButton(tr("오픈소스 라이선스"), this);
    QFont buttonFont;
    buttonFont.setPointSize(12);
    buttonFont.setBold(true);
    licenseButton->setFont(buttonFont);
    licenseButton->setMinimumHeight(50);
    licenseButton->setCursor(Qt::PointingHandCursor);
    // licenseButton->setStyleSheet(
    //     "QPushButton {"
    //     "    background-color: rgba(240, 240, 240, 0.15);"
    //     "    border: 1px solid rgba(0, 0, 0, 0.2);"
    //     "    border-radius: 10px;"
    //     "    padding: 10px;"
    //     "}"
    //     "QPushButton:hover {"
    //     "    background-color: rgba(240, 240, 240, 0.2);"
    //     "}"
    //     "QPushButton:pressed {"
    //     "    background-color: rgba(240, 240, 240, 0.1);"
    //     "}"
    // );
    connect(licenseButton, &QPushButton::clicked, this, &AboutDialog::openLicenseUrl);

    githubButton = new QPushButton("Github", this);
    githubButton->setFont(buttonFont);
    githubButton->setMinimumHeight(50);
    githubButton->setCursor(Qt::PointingHandCursor);
    // githubButton->setStyleSheet(
    //     "QPushButton {"
    //     "    background-color: white;"
    //     "    border: 2px solid black;"
    //     "    border-radius: 10px;"
    //     "    padding: 10px;"
    //     "}"
    //     "QPushButton:hover {"
    //     "    background-color: #f0f0f0;"
    //     "}"
    //     "QPushButton:pressed {"
    //     "    background-color: #e0e0e0;"
    //     "}"
    // );
    connect(githubButton, &QPushButton::clicked, this, &AboutDialog::openGithubUrl);

    buttonLayout->addWidget(licenseButton);
    buttonLayout->addWidget(githubButton);

    mainLayout->addLayout(buttonLayout);
}

void AboutDialog::openLicenseUrl()
{
    QDesktopServices::openUrl(QUrl("https://scripts.pindang2.com/hurrieup/osl"));
}

void AboutDialog::openGithubUrl()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Pindang2/HurrieUp"));
}

