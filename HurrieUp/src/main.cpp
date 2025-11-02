#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Create translator on heap to ensure it persists
    QTranslator *translator = new QTranslator(&a);
    QTranslator *qtTranslator = new QTranslator(&a);
    
    // Load Qt's own translations first
    QString qtTranslationPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
    QString locale = QLocale::system().name(); // e.g., "ko_KR", "en_US", "ja_JP"
    
    qDebug() << "System locale:" << locale;
    qDebug() << "Qt translations path:" << qtTranslationPath;
    
    // Load Qt base translations
    if (qtTranslator->load("qt_" + locale, qtTranslationPath)) {
        a.installTranslator(qtTranslator);
        qDebug() << "Loaded Qt translation for:" << locale;
    }
    
    // Try to load our application translation file
    QString translationPath = QString(":/translations/%1.qm").arg(locale);
    qDebug() << "Trying to load:" << translationPath;
    
    if (translator->load(translationPath)) {
        a.installTranslator(translator);
        qDebug() << "✓ Loaded translation:" << translationPath;
    } else {
        // Try loading with just language code (e.g., "ko" from "ko_KR")
        QString langCode = locale.left(2);
        QString fallbackPath;
        
        if (langCode == "ko") {
            fallbackPath = ":/translations/ko_KR.qm";
        } else if (langCode == "ja") {
            fallbackPath = ":/translations/ja_JP.qm";
        } else {
            fallbackPath = ":/translations/en_US.qm";
        }
        
        qDebug() << "Trying fallback path:" << fallbackPath;
        if (translator->load(fallbackPath)) {
            a.installTranslator(translator);
            qDebug() << "✓ Loaded fallback translation:" << fallbackPath;
        } else {
            qDebug() << "✗ Failed to load translation. Using default (English).";
            qDebug() << "Attempted paths:" << translationPath << "and" << fallbackPath;
        }
    }
    
    MainWindow w;
    // We don't show the main window, it just manages the application logic
    // w.show(); 
    return a.exec();
}
