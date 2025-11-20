이 애플리케이션은 시스템 트레이를 기본적으로 사용하고, 약간의 GUI를 사용해 유저에게 세팅을 지원하는 애플리케이션입니다.
목적은 일정 시간마다 유저가 설정한 오디오 콤비네이션 (ring+voice 또는 complete)을 재생하도록 하는 것입니다.
물론 기본적으로 번들된 오디오를 써도 되지만, 유저가 직접 추가할 수도 있도록 한다.

이 애플리케이션은 LGPL 라이선스 하에 배포되는 Qt 라이브러리 GUI 애플리케이션입니다.
Windows, Mac, Linux를 지원합니다.

/HurrieUp/src 폴더 안에 기본적으로 번들할 음성 파일들과 리스트업 json 파일이 있습니다. 이 파일들을 활용하도록 합시다.

유저는 기본적으로 `10분`마다 `.../ring/new_message.wav` 파일과 `.../voice/clovahurry_enahanced` 파일이 `1초`의 간격을 두고 `85%`의 볼륨으로 async 재생되는 설정을 가지게 됩니다. 코드 처리한 부분은 전부 유저가 커스텀할 수 있도록 합시다.

애플리케이션이 실행되면 트레이 아이콘이 생성되며, GUI가 닫혀도 실행을 유지합니다.
트레이 아이콘을 우클릭하면 아래와 같은 트리를 가진 메뉴가 표시됩니다:
```
[일시정지]
* 일시정지
    - 30분 동안
    - 1시간 동안
    - 2시간 동안
    - 3시간 동안
    - 6시간 동안
    - 9시간 동안
    - 12시간 동안
* 일시정지 해제
[설정]
* 설정
[종료]
* 종료
```

---

## 프로젝트 구조

```
HurrieUp/
├── CMakeLists.txt                          # 루트 CMake 설정
├── CMakePresets.json                       # CMake 프리셋
├── LICENSE
├── Readme.md
├── project_description_for_ai.md           # 이 파일
├── TRANSLATION_IMPLEMENTATION.md
├── TRANSLATION_QUICKSTART.md
└── HurrieUp/                               # 메인 애플리케이션 디렉토리
    ├── CMakeLists.txt                      # 애플리케이션 CMake 설정
    ├── resources.qrc                       # Qt 리소스 파일
    ├── src/
    │   ├── main.cpp                        # 애플리케이션 진입점
    │   ├── mainwindow.h/cpp                # 메인 윈도우 (트레이 아이콘 관리)
    │   ├── settingsdialog.h/cpp/ui         # 설정 다이얼로그
    │   └── soundinfodialog.h/cpp/ui        # 사운드 정보 다이얼로그
    ├── resources/
    │   ├── complete/
    │   │   └── bundled_complete_sounds.json
    │   ├── ring/
    │   │   └── bundled_rings.json
    │   └── voice/
    │       ├── bundled_voices.json
    │       └── SUPERTONE/
    │           └── list.txt
    └── translations/
        ├── en_US.ts/qm
        ├── ja_JP.ts/qm
        ├── ko_KR.ts/qm
        ├── translations.qrc
        ├── update_translations.ps1
        └── release_translations.ps1
```

---

## 핵심 클래스 및 기능

### MainWindow (mainwindow.h/cpp)
- **역할**: 시스템 트레이 아이콘 관리 및 오디오 재생 로직
- **주요 멤버**:
  - `QSystemTrayIcon *trayIcon` - 시스템 트레이 아이콘
  - `QTimer *timer` - 정기적인 알림 타이머
  - `QTimer *pauseTimer` - 일시정지 자동 해제 타이머 (2025-11-13 수정됨)
  - `QMediaPlayer *ringPlayer, *voicePlayer, *completePlayer` - 3개의 독립적인 오디오 플레이어
  - `QAudioOutput *ringAudioOutput, *voiceAudioOutput, *completeAudioOutput` - 각 플레이어의 오디오 출력
  
- **주요 메서드**:
  - `playSounds()` - 설정에 따라 ring+voice 또는 complete 사운드 재생
    - **2025-11-13 수정**: ring과 voice 중 하나만 선택되어도 재생 가능하도록 개선
    - ring만: ring만 재생
    - voice만: voice만 즉시 재생
    - 둘 다: ring → (soundGap ms 대기) → voice 순차 재생
  - `pause(int minutes)` - 지정된 시간동안 알림 일시정지
  - `resume()` - 일시정지 해제
  - `loadSettings()` / `saveSettings()` - QSettings를 통한 설정 저장/로드
  - `loadSoundFiles()` - 번들 사운드 파일 로드
  - `loadBundledSounds()` - JSON에서 사운드 정보 파싱

- **설정 값**:
  - `intervalMinutes` - 알림 간격 (기본값: 10분)
  - `volume` - 볼륨 (기본값: 85%)
  - `soundGap` - ring과 voice 사이 간격 (기본값: 1000ms)
  - `isCompleteMode` - complete 모드 사용 여부 (기본값: false)
  - `completePaths`, `ringPaths`, `voicePaths` - 선택된 사운드 파일 경로들 (QStringList)

### SettingsDialog (settingsdialog.h/cpp/ui)
- **역할**: 사용자 설정 GUI
- **주요 기능**:
  - 알림 간격 설정 (분 단위)
  - 볼륨 조절 슬라이더
  - 사운드 간격 조절 (ms 단위)
  - Complete 모드 vs Combination 모드 선택 (라디오 버튼)
  - 사운드 선택 (체크박스 리스트)
    - 각 사운드마다: 체크박스, 미리듣기 버튼(▶), 정보 버튼(ℹ), 삭제 버튼(🗑, 사용자 추가 사운드만)
  - 전체 미리듣기 버튼 - 현재 선택된 설정으로 테스트 재생
    - **2025-11-13 수정**: ring/voice 중 하나만 선택되어도 미리듣기 가능
    - 선택된 소리가 없으면 안내 메시지 표시
  - 사용자 사운드 추가 기능
  
- **주요 멤버**:
  - `previewPlayer1, previewPlayer2` - 미리듣기용 플레이어 2개
  - `ringCheckboxes, voiceCheckboxes, completeCheckboxes` - 각 카테고리의 체크박스 리스트
  - `bundledRings, bundledVoices, bundledCompletes` - 번들 사운드 목록
  - `userRings, userVoices, userCompletes` - 사용자 추가 사운드 목록

### SoundInfo 구조체
```cpp
struct SoundInfo {
    QString name;           // 사운드 이름
    QString description;    // 설명
    QString path;          // 파일 경로 (qrc:// 또는 절대 경로)
    bool isBundled;        // 번들 사운드 여부
};
```

---

## 최근 수정 사항 (2025-11-13)

### 1. pauseTimer 초기화 문제 수정
**문제**: 트레이 메뉴에서 일시정지 시간 선택 시 Access Violation (0xC0000005) 크래시 발생
**원인**: `pauseTimer`가 생성자에서 초기화되지 않아 잘못된 포인터 접근
**해결**:
```cpp
// MainWindow 생성자에 추가
pauseTimer = new QTimer(this);
pauseTimer->setSingleShot(true);
connect(pauseTimer, &QTimer::timeout, this, &MainWindow::resume);

// pause() 함수 간소화 - 불필요한 null 체크 및 동적 생성 제거
void MainWindow::pause(int minutes) {
    timer->stop();
    resumeAction->setEnabled(true);
    trayIcon->setToolTip(tr("HurrieUp (Paused)"));
    if (minutes > 0) {
        pauseTimer->start(minutes * 60 * 1000);
    }
}
```

### 2. Ring/Voice 단독 재생 지원
**문제**: ring과 voice가 모두 선택되어야만 소리 재생됨
**해결**: 
- `MainWindow::playSounds()` 수정
  - ring만 선택: ring만 재생
  - voice만 선택: voice만 즉시 재생 (delay 없음)
  - 둘 다 선택: 기존처럼 ring → voice 순차 재생
  
```cpp
bool hasRing = !ringPaths.isEmpty();
bool hasVoice = !voicePaths.isEmpty();

if (!hasRing && !hasVoice) {
    qDebug() << "No ring or voice sounds selected!";
    return;
}

if (hasRing) {
    // ring 재생 로직
}

if (hasVoice) {
    int delay = hasRing ? soundGap : 0;
    QTimer::singleShot(delay, this, [...]{ 
        // voice 재생 로직
    });
}
```

### 3. 미리듣기 버튼 개선
**문제**: ring 또는 voice가 하나도 선택되지 않은 경우 미리듣기 시 아무 반응 없음
**해결**:
- `SettingsDialog::onPreviewClicked()` 수정
- 선택된 소리가 없으면 사용자에게 안내 메시지 표시:
  - Complete 모드: "Please select at least one complete sound to preview."
  - Combination 모드: "Please select at least one ring sound or voice sound to preview."
- ring/voice 중 하나만 선택되어도 미리듣기 가능

---

## 개발 환경 설정

### vcpkg를 통한 Qt6 설치
```powershell
# 1. vcpkg 설치 경로로 이동
cd D:\_DEV

# 2. vcpkg 클론
git clone https://github.com/microsoft/vcpkg.git

# 3. 부트스트랩
.\vcpkg\bootstrap-vcpkg.bat

# 4. Qt6 설치 (시간 소요)
.\vcpkg\vcpkg.exe install qtbase:x64-windows qtmultimedia:x64-windows

# 5. VS Code CMake 통합
.\vcpkg\vcpkg.exe integrate install
```

### CMake 설정
- `CMakePresets.json`에서 vcpkg toolchain 경로 설정
- Qt6의 Core, Widgets, Multimedia 모듈 사용
- C++17 표준

---

## 빌드 및 실행

### VS Code에서
1. CMake Tools 익스텐션 사용
2. 프리셋 선택 (예: x64-debug)
3. Build 실행
4. Launch 또는 F5로 실행

### 출력 위치
`out/build/x64-debug/HurrieUp/HurrieUp.exe`

---

## 번역 (i18n)

- Qt Linguist 사용
- 지원 언어: 한국어(ko_KR), 영어(en_US), 일본어(ja_JP)
- `.ts` 파일 업데이트: `update_translations.ps1`
- `.qm` 파일 생성: `release_translations.ps1`
- 번역 파일은 `translations/translations.qrc`로 리소스에 포함

---

## 알려진 이슈 및 TODO

- [ ] macOS, Linux 테스트 및 최적화
- [ ] 트레이 아이콘 커스텀 이미지 추가
- [ ] 사용자 추가 사운드 파일 검증 강화
- [ ] 설정 다이얼로그 UI 개선
- [ ] 알림 로그 기능 추가

---

### AI memory generated by agent

