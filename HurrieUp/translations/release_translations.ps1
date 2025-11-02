# HurrieUp Translation Release Script
# This script converts .ts files to .qm files for distribution

$QT_BIN = "D:/_DEV/_QT_SDK/6.9.3/msvc2022_64/bin"
$PROJECT_DIR = Split-Path -Parent $PSScriptRoot
$TRANSLATIONS_DIR = "$PROJECT_DIR/translations"

Write-Host "Generating .qm files from .ts files..." -ForegroundColor Green

$TS_FILES = @(
    "$TRANSLATIONS_DIR/ko_KR.ts",
    "$TRANSLATIONS_DIR/en_US.ts",
    "$TRANSLATIONS_DIR/ja_JP.ts"
)

$success = $true
foreach ($ts_file in $TS_FILES) {
    $qm_file = $ts_file -replace '\.ts$', '.qm'
    Write-Host "Processing: $ts_file -> $qm_file" -ForegroundColor Cyan
    
    & "$QT_BIN/lrelease.exe" $ts_file -qm $qm_file
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Error processing $ts_file" -ForegroundColor Red
        $success = $false
    }
}

if ($success) {
    Write-Host ""
    Write-Host "All .qm files generated successfully!" -ForegroundColor Green
    Write-Host "The application will now use these translation files." -ForegroundColor Green
} else {
    Write-Host ""
    Write-Host "Some errors occurred during .qm generation!" -ForegroundColor Red
    exit 1
}
