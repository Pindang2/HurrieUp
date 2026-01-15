# HurrieUp 다국어 지원 구현 완료

## 개요
HurrieUp 애플리케이션에 한국어, 영어, 일본어 다국어 지원이 성공적으로 추가되었습니다.

## 구현된 기능

### 1. 번역 파일
- ✅ `translations/ko_KR.ts` - 한국어 번역 (17개 문자열)
- ✅ `translations/en_US.ts` - 영어 번역 (17개 문자열)
- ✅ `translations/ja_JP.ts` - 일본어 번역 (17개 문자열)
- ✅ 각 언어별 `.qm` 파일 자동 생성

### 2. 소스 코드 변경

#### main.cpp
- QTranslator 추가
- 시스템 로케일 자동 감지
- 번역 파일 자동 로드 기능 구현

#### mainwindow.cpp
- 모든 UI 문자열을 `tr()` 함수로 래핑
- 번역 가능한 문자열:
  - "Pause" (일시정지/一時停止)
  - "Resume" (재개/再開)
  - "Settings" (설정/設定)
  - "Quit" (종료/終了)
  - "HurrieUp" / "HurrieUp (Paused)"
  - "%1 minutes" (동적 분 단위 표시)

#### settingsdialog.ui
- UI 파일의 모든 문자열이 이미 Qt 번역 시스템 호환
- 자동으로 번역 파일에 포함됨

### 3. 빌드 시스템

#### CMakeLists.txt
- Qt LinguistTools 패키지 추가
- 번역 파일 자동 처리 설정
- `.ts` 파일에서 `.qm` 파일 자동 생성

#### translations.qrc
- 번역된 `.qm` 파일을 리소스로 포함
- 애플리케이션에 임베드되어 배포

### 4. 유틸리티 스크립트

#### update_translations.ps1
```powershell
# 사용법: 새로운 문자열을 코드에 추가한 후 실행
.\update_translations.ps1
```
- 소스 코드에서 번역 가능한 문자열 추출
- `.ts` 파일 업데이트

#### release_translations.ps1
```powershell
# 사용법: 번역 작업 완료 후 실행
.\release_translations.ps1
```
- `.ts` 파일을 `.qm` 파일로 변환
- 배포용 바이너리 번역 파일 생성

## 번역된 문자열 목록

| 원문 (소스) | 한국어 | 영어 | 日本語 |
|------------|--------|------|--------|
| Pause | 일시정지 | Pause | 一時停止 |
| Resume | 재개 | Resume | 再開 |
| Settings | 설정 | Settings | 設定 |
| Quit | 종료 | Quit | 終了 |
| HurrieUp | 헐리업 | HurrieUp | ハリーアップ |
| HurrieUp (Paused) | 헐리업 (일시정지됨) | HurrieUp (Paused) | ハリーアップ (一時停止中) |
| %1 minutes | %1분 | %1 minutes | %1分 |
| Interval (minutes): | 간격 (분): | Interval (minutes): | 間隔（分）： |
| Volume: | 볼륨: | Volume: | 音量： |
| Mode: | 모드: | Mode: | モード： |
| Complete | 완성 모드 | Complete | 完全モード |
| Combination | 조합 모드 | Combination | 組み合わせモード |
| Sound: | 사운드: | Sound: | サウンド： |
| Ring Sound: | 벨 소리: | Ring Sound: | 着信音： |
| Voice Sound: | 음성 소리: | Voice Sound: | 音声： |
| Sound Gap (ms): | 사운드 간격 (ms): | Sound Gap (ms): | サウンド間隔（ms）： |

## 언어 자동 감지

애플리케이션은 Windows 시스템 언어 설정을 자동으로 감지하여 적절한 언어로 표시됩니다:

1. **한국어 시스템** (`ko_KR`, `ko_*`)
   - 자동으로 한국어 번역 로드
   
2. **일본어 시스템** (`ja_JP`, `ja_*`)
   - 자동으로 일본어 번역 로드
   
3. **기타 시스템**
   - 영어가 기본 언어로 표시

## 빌드 및 테스트

### 빌드 방법
```powershell
cd d:\_DEV\HurrieUp
cmake --build build
```

빌드 시 자동으로:
1. `.ts` 파일에서 `.qm` 파일 생성
2. `.qm` 파일이 리소스 파일에 포함
3. 실행 파일에 번역 파일 임베드

### 테스트 방법
1. 애플리케이션 실행
2. 시스템 언어에 따라 UI가 해당 언어로 표시됨
3. 트레이 아이콘 메뉴 확인
4. 설정 대화상자 열기 및 확인

## 추가 번역 작업이 필요한 경우

### 1. Qt Linguist로 번역 수정
```powershell
D:/_DEV/_QT_SDK/6.9.3/msvc2022_64/bin/linguist.exe .\translations\ko_KR.ts
```

### 2. 새 문자열 추가 워크플로우
1. 코드에서 `tr()` 함수 사용:
   ```cpp
   QString text = tr("New translatable string");
   ```
2. 번역 파일 업데이트:
   ```powershell
   .\translations\update_translations.ps1
   ```
3. Qt Linguist로 번역 작업
4. QM 파일 생성:
   ```powershell
   .\translations\release_translations.ps1
   ```
5. 프로젝트 리빌드

## 파일 구조

```
HurrieUp/
├── src/
│   ├── main.cpp              (QTranslator 초기화 추가)
│   ├── mainwindow.cpp        (tr() 함수로 문자열 래핑)
│   ├── mainwindow.h
│   ├── settingsdialog.cpp
│   ├── settingsdialog.h
│   └── settingsdialog.ui     (UI 문자열 자동 번역)
├── translations/
│   ├── ko_KR.ts              (한국어 번역 소스)
│   ├── ko_KR.qm              (한국어 번역 바이너리)
│   ├── en_US.ts              (영어 번역 소스)
│   ├── en_US.qm              (영어 번역 바이너리)
│   ├── ja_JP.ts              (일본어 번역 소스)
│   ├── ja_JP.qm              (일본어 번역 바이너리)
│   ├── translations.qrc      (번역 리소스 파일)
│   ├── update_translations.ps1
│   ├── release_translations.ps1
│   └── README.md
├── CMakeLists.txt            (번역 빌드 설정 추가)
└── resources.qrc
```

## 주의사항

1. **새 UI 문자열 추가 시 반드시 `tr()` 함수 사용**
   - ✅ `new QAction(tr("Text"), this)`
   - ❌ `new QAction("Text", this)`

2. **번역 파일 동기화**
   - 코드 변경 후 `update_translations.ps1` 실행
   - 번역 후 `release_translations.ps1` 실행

3. **빌드 시 자동 처리**
   - CMake가 자동으로 `.qm` 파일 생성
   - 리소스 파일에 자동 포함

## 성공 지표

- ✅ 17개의 UI 문자열이 3개 언어로 번역됨
- ✅ 시스템 로케일 자동 감지 작동
- ✅ 모든 번역 파일 (.ts, .qm) 정상 생성
- ✅ 빌드 시스템 통합 완료
- ✅ 번역 워크플로우 자동화 스크립트 제공

## 향후 확장 가능성

1. 추가 언어 지원:
   - 중국어 (zh_CN)
   - 프랑스어 (fr_FR)
   - 독일어 (de_DE)

2. 동적 언어 변경:
   - 설정 메뉴에 언어 선택 옵션 추가
   - 재시작 없이 언어 변경 기능

3. 컨텍스트별 번역:
   - 같은 단어의 다른 의미에 대한 컨텍스트 번역

---

**구현 완료 일자**: 2025년 10월 23일
**지원 언어**: 한국어, 영어, 일본어
**번역 문자열 수**: 17개
