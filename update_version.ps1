<#
.SYNOPSIS
    Update the project version in several repository files in one step.

.DESCRIPTION
    Replace the app version in installer/setup.iss (#define MyAppVersion "x.y.z"),
    update any occurrences in installer/README.md (HurrieUp-Setup-x.y.z.exe and define mention),
    and update the displayed version in HurrieUp/src/aboutdialog.cpp.

.PARAMETER Version
    The new version string (semantic style, e.g. 1.2.3 or 1.2.3-beta).

.PARAMETER WriteVersionFile
    If specified, write a top-level `VERSION` file containing the version.

.EXAMPLE
    .\update_version.ps1 -Version 1.0.2 -WriteVersionFile

NOTES
    The script makes .bak backups for edited files (same directory).
#>

param(
    [Parameter(Mandatory=$true)][string]$Version,
    [switch]$WriteVersionFile
)

function FailIfInvalidVersion {
    param($v)
    if ($v -notmatch '^[0-9]+\.[0-9]+\.[0-9]+([\-A-Za-z0-9\.]+)?$') {
        Write-Error "Version '$v' is not in expected format (e.g. 1.2.3 or 1.2.3-beta)."
        exit 1
    }
}

FailIfInvalidVersion -v $Version

$Root = Split-Path -Parent $MyInvocation.MyCommand.Definition
Set-Location $Root

$changed = @()

function Backup-File([string]$path) {
    if (Test-Path $path) {
        $bak = "$path.bak"
        Copy-Item -Path $path -Destination $bak -Force
    }
}

Write-Host "Updating repository files to version $Version"

# 1) Replace #define MyAppVersion "..." across files that contain it
$definePattern = '#define\s+MyAppVersion\s+"[^"]+"'
$filesWithDefine = Get-ChildItem -Path $Root -Recurse -File | Where-Object { Select-String -Path $_.FullName -Pattern '#define\s+MyAppVersion' -Quiet }
foreach ($f in $filesWithDefine) {
    $full = $f.FullName
    Backup-File $full
    $content = Get-Content -Raw -LiteralPath $full
    $new = [Regex]::Replace($content, '(#define\s+MyAppVersion\s+")([^"]+)(")', "`$1$Version`$3")
    if ($new -ne $content) {
        Set-Content -LiteralPath $full -Value $new -Encoding UTF8
        $changed += $full
        Write-Host "Updated MyAppVersion in: $full"
    }
}

# 2) Update README installer example filenames and mentions
$readme = Join-Path $Root 'installer\README.md'
if (Test-Path $readme) {
    Backup-File $readme
    $content = Get-Content -Raw -LiteralPath $readme
    $new = $content -replace 'HurrieUp-Setup-\d+\.\d+\.\d+\.exe', "HurrieUp-Setup-$Version.exe"
    $new = [Regex]::Replace($new, '(#define\s+MyAppVersion\s+")([^"]+)(")', "`$1$Version`$3")
    if ($new -ne $content) {
        Set-Content -LiteralPath $readme -Value $new -Encoding UTF8
        $changed += $readme
        Write-Host "Updated README installer examples: $readme"
    }
}

# 3) Update About dialog displayed version (only in the line that creates versionLabel)
$about = Join-Path $Root 'HurrieUp\src\aboutdialog.cpp'
if (Test-Path $about) {
    Backup-File $about
    $content = Get-Content -Raw -LiteralPath $about

    # Look for the versionLabel initialization line and replace any semantic version inside it
    $new = [Regex]::Replace($content, '(versionLabel\s*=\s*new QLabel\("[^"]*?)\d+\.\d+\.\d+([^-"\\]*)("\,\s*this\))', "`$1$Version`$2`$3")

    # Fallback: if the specific pattern didn't match, try replacing a version inside the string literal on the line with 'versionLabel ='
    if ($new -eq $content) {
        $new = [Regex]::Replace($content, '(versionLabel\s*=\s*new QLabel\("[^"]*?)\d+\.\d+\.\d+([^"]*"\,\s*this\))', "`$1$Version`$2")
    }

    if ($new -ne $content) {
        Set-Content -LiteralPath $about -Value $new -Encoding UTF8
        $changed += $about
        Write-Host "Updated About dialog version in: $about"
    }
}

# 4) As a safety, update any literal 'HurrieUp-Setup-x.y.z.exe' usage across repository
$setupPattern = 'HurrieUp-Setup-\d+\.\d+\.\d+\.exe'
$textFiles = Get-ChildItem -Path $Root -Recurse -File | Where-Object { $_.Extension -match '\.md|\.txt|\.iss|\.ps1|\.cpp|\.h' }
foreach ($f in $textFiles) {
    $full = $f.FullName
    $content = Get-Content -Raw -LiteralPath $full
    if ($content -match $setupPattern) {
        Backup-File $full
        $new = $content -replace $setupPattern, "HurrieUp-Setup-$Version.exe"
        Set-Content -LiteralPath $full -Value $new -Encoding UTF8
        $changed += $full
        Write-Host "Replaced setup filename in: $full"
    }
}

# 5) Optionally write VERSION file
if ($WriteVersionFile) {
    $vfile = Join-Path $Root 'VERSION'
    Set-Content -LiteralPath $vfile -Value $Version -Encoding UTF8
    $changed += $vfile
    Write-Host "Wrote VERSION file: $vfile"
}

Write-Host "\nUpdate complete. Files changed:"
if ($changed.Count -eq 0) { Write-Host "  (none)" } else { $changed | Sort-Object -Unique | ForEach-Object { Write-Host "  $_" } }

Write-Host "Backups (*.bak) were created next to modified files. Review changes and commit when ready."

exit 0
