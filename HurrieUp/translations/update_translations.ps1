# HurrieUp Translation Update Script
# This script updates the translation files with new strings from the source code

$QT_BIN = "D:/_DEV/_QT_SDK/6.9.3/msvc2022_64/bin"
$PROJECT_DIR = Split-Path -Parent $PSScriptRoot
$TRANSLATIONS_DIR = "$PROJECT_DIR/translations"

Write-Host "Updating translation files..." -ForegroundColor Green

# Run lupdate to extract translatable strings
& "$QT_BIN/lupdate.exe" `
    "$PROJECT_DIR/src/main.cpp" `
    "$PROJECT_DIR/src/mainwindow.cpp" `
    "$PROJECT_DIR/src/mainwindow.h" `
    "$PROJECT_DIR/src/settingsdialog.cpp" `
    "$PROJECT_DIR/src/settingsdialog.h" `
    "$PROJECT_DIR/src/settingsdialog.ui" `
    -ts `
    "$TRANSLATIONS_DIR/ko_KR.ts" `
    "$TRANSLATIONS_DIR/en_US.ts" `
    "$TRANSLATIONS_DIR/ja_JP.ts"

if ($LASTEXITCODE -eq 0) {
    Write-Host "Translation files updated successfully!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Yellow
    Write-Host "1. Open Qt Linguist to translate the strings:"
    Write-Host "   $QT_BIN/linguist.exe $TRANSLATIONS_DIR/ko_KR.ts" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "2. After translation, rebuild the project to generate .qm files"
    Write-Host "   The .qm files will be automatically created during build"
} else {
    Write-Host "Error updating translation files!" -ForegroundColor Red
    exit 1
}
