# HurrieUp Installer 빌드 가이드

이 문서는 HurrieUp 애플리케이션의 Windows 인스톨러를 생성하는 방법을 설명합니다.

## 사전 준비 사항

1. **Inno Setup 설치**
   - [Inno Setup 다운로드](https://jrsoftware.org/isdl.php)
   - 기본 설치 경로: `C:\Program Files (x86)\Inno Setup 6\`
   - 한국어, 영어, 일본어 언어 파일이 포함되어 있습니다

2. **Qt 빌드 환경**
   - CMake 설정이 완료된 상태
   - Release 빌드가 가능한 상태
   - vcpkg 또는 Qt 설치를 통해 `windeployqt` 도구 사용 가능

## 인스톨러 생성 방법

### 방법 1: 자동 빌드 스크립트 사용 (권장)

PowerShell에서 다음 명령을 실행하세요:

```powershell
.\build_installer.ps1
```

이 스크립트는 다음 단계를 자동으로 수행합니다:
1. Release 빌드 생성
2. Qt 의존성 파일 수집 (windeployqt)
3. Inno Setup을 사용한 인스톨러 생성

#### 스크립트 옵션

```powershell
# 빌드 건너뛰고 기존 빌드 사용
.\build_installer.ps1 -SkipBuild

# 특정 Qt 경로 지정
.\build_installer.ps1 -QtPath "C:\Qt\6.x.x\msvc2019_64"

# 사용자 지정 빌드 디렉토리
.\build_installer.ps1 -BuildDir "out\build\custom-release"
```

### 방법 2: 수동 빌드

1. **Release 빌드 생성**
   ```powershell
   cmake --preset=x64-release
   cmake --build out\build\x64-release --config Release
   ```

2. **배포 폴더 준비**
   ```powershell
   # 배포 폴더 생성
   New-Item -ItemType Directory -Path "installer\deploy" -Force
   
   # 실행 파일 복사
   Copy-Item "out\build\x64-release\HurrieUp\HurrieUp.exe" "installer\deploy\"
   
   # Qt 의존성 파일 수집
   cd installer\deploy
   windeployqt HurrieUp.exe --release --no-translations
   cd ..\..
   ```

3. **Inno Setup으로 인스톨러 생성**
   ```powershell
   "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" installer\setup.iss
   ```

## 출력 결과

인스톨러는 다음 경로에 생성됩니다:
```
installer\output\HurrieUp-Setup-1.0.1.exe
```

## 인스톨러 설정 수정

`installer\setup.iss` 파일을 편집하여 다음 항목을 수정할 수 있습니다:

- **앱 버전**: `#define MyAppVersion "1.0.1"`
- **게시자 정보**: `#define MyAppPublisher "HurrieUp Team"`
- **URL**: `#define MyAppURL "https://github.com/yourusername/HurrieUp"`
- **앱 아이콘**: `SetupIconFile=..\HurrieUp\resources\app_icon.ico`

## 인스톨러 기능

생성된 인스톨러는 다음 기능을 제공합니다:

- ✅ 64비트 Windows 지원
- ✅ 다국어 설치 (한국어, 영어, 일본어)
- ✅ 바탕화면 바로가기 생성 옵션
- ✅ 시작 프로그램 등록 옵션
- ✅ 프로그램 추가/제거에서 제거 가능
- ✅ 제거 시 사용자 설정 삭제 옵션

## 문제 해결

### windeployqt를 찾을 수 없음

vcpkg로 Qt를 설치한 경우:
```powershell
.\build_installer.ps1 -QtPath "D:\_DEV\vcpkg\installed\x64-windows\tools\Qt6"
```

Qt 공식 인스톨러로 설치한 경우:
```powershell
.\build_installer.ps1 -QtPath "C:\Qt\6.7.0\msvc2019_64"
```

### Inno Setup을 찾을 수 없음

Inno Setup이 기본 경로가 아닌 곳에 설치된 경우, `build_installer.ps1` 파일의 `$innoSetup` 변수를 수정하세요.

### 빌드 오류

Release 빌드가 제대로 되었는지 확인:
```powershell
cmake --build out\build\x64-release --config Release
```

## 배포

생성된 `.exe` 파일을 사용자에게 배포할 수 있습니다. 
별도의 의존성 설치 없이 바로 실행 가능합니다.

## 라이선스

이 프로젝트는 LGPL 라이선스를 따릅니다. 인스톨러에는 LICENSE 파일이 포함됩니다.
