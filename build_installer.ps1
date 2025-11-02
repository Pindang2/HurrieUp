# HurrieUp Installer Build Script
# This script builds a Release version, deploys Qt dependencies, and creates an installer

param(
    [string]$BuildDir = "out\build\x64-release",
    [string]$DeployDir = "installer\deploy",
    [string]$QtPath = "",
    [switch]$SkipBuild = $false,
    [switch]$SkipDeploy = $false
)

$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "HurrieUp Installer Build Script" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

# Step 1: Build Release version
if (-not $SkipBuild) {
    Write-Host "[1/4] Building Release version..." -ForegroundColor Green
    
    Push-Location $ProjectRoot
    
    # Check if release build directory exists
    $fullBuildPath = Join-Path $ProjectRoot $BuildDir
    
    if (-not (Test-Path $fullBuildPath)) {
        Write-Host "Creating new Release build configuration..." -ForegroundColor Yellow
        
        # Try to configure with preset first
        cmake --preset=x64-release 2>&1 | Out-Null
        
        # If preset fails, try manual configuration
        if ($LASTEXITCODE -ne 0) {
            Write-Host "Preset configuration failed, trying manual configuration..." -ForegroundColor Yellow
            
            # Create build directory
            New-Item -ItemType Directory -Path $fullBuildPath -Force | Out-Null
            
            # Configure with Visual Studio generator (more reliable on Windows)
            Push-Location $fullBuildPath
            cmake ..\..\..\.. -DCMAKE_BUILD_TYPE=Release
            Pop-Location
            
            if ($LASTEXITCODE -ne 0) {
                Write-Error "CMake configuration failed! Please configure manually."
            }
        }
    }
    
    # Build Release
    Write-Host "Building Release configuration..." -ForegroundColor Yellow
    cmake --build $fullBuildPath --config Release
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Build failed!"
    }
    
    Pop-Location
    Write-Host "Build completed successfully!" -ForegroundColor Green
    Write-Host ""
} else {
    Write-Host "[1/4] Skipping build (using existing build)..." -ForegroundColor Yellow
    Write-Host ""
}

# Step 2: Find windeployqt
Write-Host "[2/4] Locating Qt deployment tool..." -ForegroundColor Green

$windeployqt = ""

# Try to find windeployqt in PATH
$windeployqt = Get-Command windeployqt -ErrorAction SilentlyContinue

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
    Write-Error "windeployqt not found! Please provide Qt path using -QtPath parameter or ensure Qt is in PATH"
}

Write-Host "Found windeployqt: $windeployqt" -ForegroundColor Green
Write-Host ""

# Step 3: Deploy Qt dependencies
if (-not $SkipDeploy) {
    Write-Host "[3/4] Deploying Qt dependencies..." -ForegroundColor Green
    
    $exePath = Join-Path $ProjectRoot "$BuildDir\HurrieUp\HurrieUp.exe"
    $deployPath = Join-Path $ProjectRoot $DeployDir
    
    # Clean deploy directory
    if (Test-Path $deployPath) {
        Write-Host "Cleaning deploy directory..." -ForegroundColor Yellow
        Remove-Item -Path $deployPath -Recurse -Force
    }
    New-Item -ItemType Directory -Path $deployPath -Force | Out-Null
    
    # Copy executable
    Copy-Item -Path $exePath -Destination $deployPath -Force
    Write-Host "Copied HurrieUp.exe to deploy directory" -ForegroundColor Green
    
    # Run windeployqt
    Push-Location $deployPath
    & $windeployqt "HurrieUp.exe" --release --no-translations
    if ($LASTEXITCODE -ne 0) {
        Write-Error "windeployqt failed!"
    }
    Pop-Location
    
    Write-Host "Qt dependencies deployed successfully!" -ForegroundColor Green
    Write-Host ""
} else {
    Write-Host "[3/4] Skipping deployment (using existing deployment)..." -ForegroundColor Yellow
    Write-Host ""
}

# Step 4: Create installer with Inno Setup
Write-Host "[4/4] Creating installer with Inno Setup..." -ForegroundColor Green

$innoSetup = "C:\Program Files (x86)\Inno Setup 6\ISCC.exe"
if (-not (Test-Path $innoSetup)) {
    Write-Warning "Inno Setup not found at: $innoSetup"
    Write-Host ""
    Write-Host "Please download and install Inno Setup from:" -ForegroundColor Yellow
    Write-Host "https://jrsoftware.org/isdl.php" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "After installation, run this script again." -ForegroundColor Yellow
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
