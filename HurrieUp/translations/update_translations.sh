#!/bin/bash

# HurrieUp Translation Update Script
# This script updates the translation files with new strings from the source code

QT_BIN="/home/pindang2/Qt/6.9.3/gcc_64/bin"  # Adjust this path to your Qt installation
PROJECT_DIR="$(dirname "$(dirname "$0")")"
TRANSLATIONS_DIR="$PROJECT_DIR/translations"

echo "Updating translation files..."

# Run lupdate to extract translatable strings
"$QT_BIN/lupdate" \
    "$PROJECT_DIR/src/main.cpp" \
    "$PROJECT_DIR/src/mainwindow.cpp" \
    "$PROJECT_DIR/src/mainwindow.h" \
    "$PROJECT_DIR/src/settingsdialog.cpp" \
    "$PROJECT_DIR/src/settingsdialog.h" \
    "$PROJECT_DIR/src/settingsdialog.ui" \
    -ts \
    "$TRANSLATIONS_DIR/ko_KR.ts" \
    "$TRANSLATIONS_DIR/en_US.ts" \
    "$TRANSLATIONS_DIR/ja_JP.ts"

if [ $? -eq 0 ]; then
    echo "Translation files updated successfully!"
    echo ""
    echo "Next steps:"
    echo "1. Open Qt Linguist to translate the strings:"
    echo "   $QT_BIN/linguist $TRANSLATIONS_DIR/ko_KR.ts"
    echo ""
    echo "2. After translation, rebuild the project to generate .qm files"
    echo "   The .qm files will be automatically created during build"
else
    echo "Error updating translation files!"
    exit 1
fi