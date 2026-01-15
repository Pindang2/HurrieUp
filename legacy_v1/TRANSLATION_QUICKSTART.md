# 🌐 다국어 지원 빠른 시작 가이드

## ✅ 완료된 작업

HurrieUp 애플리케이션에 **한국어**, **영어**, **일본어** 지원이 완료되었습니다!

### 지원 언어
- 🇰🇷 **한국어** (ko_KR)
- 🇺🇸 **영어** (en_US) 
- 🇯🇵 **일본어** (ja_JP)

## 🚀 즉시 사용 가능

애플리케이션을 빌드하고 실행하면:
- Windows 시스템 언어를 자동으로 감지
- 해당하는 언어로 UI 표시
- 지원하지 않는 언어는 영어로 표시 (기본값)

## 📝 번역된 UI 요소

### 트레이 아이콘 메뉴
- Pause → 일시정지 / 一時停止
- 30 minutes → 30분 / 30分
- Resume → 재개 / 再開
- Settings → 설정 / 設定
- Quit → 종료 / 終了

### 설정 대화상자
- Settings → 설정 / 設定
- Interval (minutes) → 간격 (분) / 間隔（分）
- Volume → 볼륨 / 音量
- Mode → 모드 / モード
- Complete → 완성 모드 / 完全モード
- Combination → 조합 모드 / 組み合わせモード
- Sound → 사운드 / サウンド
- Ring Sound → 벨 소리 / 着信音
- Voice Sound → 음성 소리 / 音声
- Sound Gap (ms) → 사운드 간격 (ms) / サウンド間隔（ms）

## 🔧 개발자를 위한 정보

### 새 문자열 추가 시
```cpp
// ✅ 올바른 방법
QAction *action = new QAction(tr("New Menu Item"), this);
QString message = tr("Status: %1").arg(status);

// ❌ 잘못된 방법 (번역 안 됨)
QAction *action = new QAction("New Menu Item", this);
```

### 번역 업데이트 워크플로우
```powershell
# 1. 새 문자열 추가 후
cd HurrieUp\translations
.\update_translations.ps1

# 2. Qt Linguist로 번역
# D:/_DEV/_QT_SDK/6.9.3/msvc2022_64/bin/linguist.exe ko_KR.ts

# 3. .qm 파일 생성
.\release_translations.ps1

# 4. 프로젝트 리빌드
cd ..\..
cmake --build build
```

## 📂 주요 파일

```
HurrieUp/
├── translations/
│   ├── ko_KR.ts, ko_KR.qm       # 한국어
│   ├── en_US.ts, en_US.qm       # 영어
│   ├── ja_JP.ts, ja_JP.qm       # 일본어
│   ├── update_translations.ps1   # 번역 추출 스크립트
│   └── release_translations.ps1  # QM 생성 스크립트
└── src/
    └── main.cpp                  # 언어 자동 감지 로직
```

## 🎯 테스트 방법

### 한국어 테스트
```powershell
# Windows 지역 설정을 한국어로 변경 후 실행
# 또는 임시로 테스트:
$env:LANG="ko_KR"
.\HurrieUp.exe
```

### 일본어 테스트
```powershell
$env:LANG="ja_JP"
.\HurrieUp.exe
```

### 영어 테스트
```powershell
$env:LANG="en_US"
.\HurrieUp.exe
```

## 📊 통계

- **총 번역 문자열**: 17개
- **지원 언어**: 3개
- **번역 완성도**: 100%
- **자동화 스크립트**: 2개

## 🆘 문제 해결

### 번역이 표시되지 않는 경우
1. `.qm` 파일이 생성되었는지 확인:
   ```powershell
   ls translations\*.qm
   ```

2. 리소스 파일에 포함되었는지 확인:
   ```powershell
   # CMake 재설정 및 리빌드
   cmake --build build --clean-first
   ```

3. 디버그 로그 확인:
   - 콘솔 출력에서 "Loaded translation: ..." 메시지 확인

### Qt Linguist를 찾을 수 없는 경우
```powershell
# Qt SDK 경로 확인
$QT_PATH = "D:/_DEV/_QT_SDK/6.9.3/msvc2022_64/bin"
& "$QT_PATH/linguist.exe"
```

## 📚 추가 리소스

- [Qt Linguist 매뉴얼](https://doc.qt.io/qt-6/linguist-manual.html)
- [Qt 국제화 가이드](https://doc.qt.io/qt-6/internationalization.html)
- 프로젝트 상세 문서: `TRANSLATION_IMPLEMENTATION.md`
- 번역 가이드: `translations/README.md`

---

**✨ 이제 HurrieUp은 다국어를 지원합니다!**

빌드하고 실행하면 자동으로 시스템 언어에 맞는 UI가 표시됩니다.
