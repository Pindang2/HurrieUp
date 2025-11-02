# Quick Installer Build Script (Using existing Release build)
# This script uses your existing release build to quickly create an installer

param(
    [string]$QtPath = ""
)

$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "HurrieUp Quick Installer (Release)" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "This will create an installer using your existing release build." -ForegroundColor Yellow
Write-Host ""

# Use existing release build
$exePath = "out\build\x64-release\HurrieUp\HurrieUp.exe"
$fullExePath = Join-Path $ProjectRoot $exePath

if (-not (Test-Path $fullExePath)) {
    Write-Error "Release build not found at: $fullExePath`nPlease build the project in Release mode first!"
}

Write-Host "[1/3] Found existing build: $exePath" -ForegroundColor Green
Write-Host ""

# Step 2: Find windeployqt
Write-Host "[2/3] Deploying Qt dependencies..." -ForegroundColor Green

$windeployqt = ""

# Try to find windeployqt in PATH
$windeployqtCmd = Get-Command windeployqt -ErrorAction SilentlyContinue
if ($windeployqtCmd) {
    $windeployqt = $windeployqtCmd.Source
}

if (-not $windeployqt) {
    # Try Qt SDK installation
    $qtSdkPath = "D:\_DEV\_QT_SDK\6.9.3\msvc2022_64\bin\windeployqt.exe"
    if (Test-Path $qtSdkPath) {
        $windeployqt = $qtSdkPath
    }
}

if (-not $windeployqt) {
    # Try user-provided path
    if ($QtPath -and (Test-Path "$QtPath\bin\windeployqt.exe")) {
        $windeployqt = "$QtPath\bin\windeployqt.exe"
    }
}

if (-not $windeployqt) {
    Write-Error "windeployqt not found! Please provide Qt path using -QtPath parameter`nExample: .\build_installer_release_quick.ps1 -QtPath 'D:\_DEV\_QT_SDK\6.9.3\msvc2022_64'"
}

Write-Host "Found windeployqt: $windeployqt" -ForegroundColor Green

$deployPath = Join-Path $ProjectRoot "installer\deploy"

# Clean deploy directory
if (Test-Path $deployPath) {
    Write-Host "Cleaning deploy directory..." -ForegroundColor Yellow
    Remove-Item -Path $deployPath -Recurse -Force
}
New-Item -ItemType Directory -Path $deployPath -Force | Out-Null

# Copy executable
Copy-Item -Path $fullExePath -Destination $deployPath -Force
Write-Host "Copied HurrieUp.exe to deploy directory" -ForegroundColor Green

# Run windeployqt (use --release for release builds)
Push-Location $deployPath
& $windeployqt "HurrieUp.exe" --release --no-translations
if ($LASTEXITCODE -ne 0) {
    Pop-Location
    Write-Error "windeployqt failed!"
}
Pop-Location

Write-Host "Qt dependencies deployed successfully!" -ForegroundColor Green
Write-Host ""

# Step 3: Create installer with Inno Setup
Write-Host "[3/3] Creating installer with Inno Setup..." -ForegroundColor Green

$innoSetup = "C:\Program Files (x86)\Inno Setup 6\ISCC.exe"
if (-not (Test-Path $innoSetup)) {
    Write-Warning "Inno Setup not found at: $innoSetup"
    Write-Host ""
    Write-Host "Please download and install Inno Setup from:" -ForegroundColor Yellow
    Write-Host "https://jrsoftware.org/isdl.php" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "After installation, run this script again." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Deploy folder is ready at: $deployPath" -ForegroundColor Green
    Write-Host "You can create the installer manually using installer\setup.iss" -ForegroundColor Green
    exit 1
}

$issPath = Join-Path $ProjectRoot "installer\setup.iss"
& $innoSetup $issPath

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "=====================================" -ForegroundColor Cyan
    Write-Host "Installer created successfully!" -ForegroundColor Green
    Write-Host "=====================================" -ForegroundColor Cyan
    Write-Host ""
    $outputDir = Join-Path $ProjectRoot "installer\output"
    Write-Host "Output location: $outputDir" -ForegroundColor Green
    
    # List created installers
    Get-ChildItem -Path $outputDir -Filter "*.exe" | ForEach-Object {
        Write-Host "  - $($_.Name) ($([math]::Round($_.Length / 1MB, 2)) MB)" -ForegroundColor Cyan
    }
} else {
    Write-Error "Inno Setup compilation failed!"
}

Write-Host ""
Write-Host "Done!" -ForegroundColor Green
