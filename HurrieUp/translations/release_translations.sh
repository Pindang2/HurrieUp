#!/bin/bash

# HurrieUp Translation Release Script
# This script converts .ts files to .qm files for distribution

QT_BIN="/home/pindang2/Qt/6.9.3/gcc_64/bin"  # Adjust this path to your Qt installation
PROJECT_DIR="$(dirname "$(dirname "$0")")"
TRANSLATIONS_DIR="$PROJECT_DIR/translations"

echo "Generating .qm files from .ts files..."

TS_FILES=(
    "$TRANSLATIONS_DIR/ko_KR.ts"
    "$TRANSLATIONS_DIR/en_US.ts"
    "$TRANSLATIONS_DIR/ja_JP.ts"
)

success=true
for ts_file in "${TS_FILES[@]}"; do
    qm_file="${ts_file%.ts}.qm"
    echo "Processing: $ts_file -> $qm_file"
    
    "$QT_BIN/lrelease" "$ts_file" -qm "$qm_file"
    
    if [ $? -ne 0 ]; then
        echo "Error processing $ts_file"
        success=false
    fi
done

if $success; then
    echo ""
    echo "All .qm files generated successfully!"
    echo "The application will now use these translation files."
else
    echo ""
    echo "Some errors occurred during .qm generation!"
    exit 1
fi