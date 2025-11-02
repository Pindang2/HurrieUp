#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class QLabel;
class QPushButton;

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private slots:
    void openLicenseUrl();
    void openGithubUrl();

private:
    void setupUI();
    
    QLabel *titleLabel;
    QLabel *versionLabel;
    QLabel *authorLabel;
    QLabel *descriptionLabel;
    QPushButton *licenseButton;
    QPushButton *githubButton;
};

#endif // ABOUTDIALOG_H
