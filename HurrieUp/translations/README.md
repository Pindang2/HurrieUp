# HurrieUp 다국어 지원 (Internationalization)

이 프로젝트는 한국어, 영어, 일본어를 지원합니다.

## 번역 파일

- `translations/ko_KR.ts` - 한국어 번역
- `translations/en_US.ts` - 영어 번역
- `translations/ja_JP.ts` - 일본어 번역

## 번역 파일 업데이트 방법

### 1. 새로운 문자열 추가 시

코드에서 UI 문자열을 추가할 때는 반드시 `tr()` 함수를 사용하세요:

```cpp
QAction *action = new QAction(tr("New Text"), this);
trayIcon->setToolTip(tr("Status Message"));
```

### 2. 번역 파일 업데이트

새로운 문자열을 추가한 후, Qt Linguist를 사용하여 .ts 파일을 업데이트합니다:

```bash
# Windows (Qt SDK 경로에 맞게 조정)
D:/_DEV/_QT_SDK/6.9.3/msvc2022_64/bin/lupdate.exe HurrieUp/src -ts HurrieUp/translations/*.ts

# 또는 CMake를 통해
cmake --build build --target HurrieUp_lupdate
```

### 3. 번역 작업

Qt Linguist를 사용하여 .ts 파일을 열고 번역 작업을 수행합니다:

```bash
D:/_DEV/_QT_SDK/6.9.3/msvc2022_64/bin/linguist.exe HurrieUp/translations/ko_KR.ts
```

### 4. .qm 파일 생성

번역이 완료되면 .qm 파일을 생성합니다 (CMake가 자동으로 처리):

```bash
cmake --build build
```

## 언어 감지

애플리케이션은 시스템 언어 설정을 자동으로 감지하여 적절한 언어로 표시됩니다:

- 한국어 시스템: 한국어 표시
- 일본어 시스템: 일본어 표시
- 기타: 영어 표시 (기본값)

## 번역 파일 구조

```xml
<message>
    <source>원본 텍스트</source>
    <translation>번역된 텍스트</translation>
</message>
```

## 주요 번역 문자열

### 메인 윈도우
- Pause - 일시정지 / 一時停止
- Resume - 재개 / 再開
- Settings - 설정 / 設定
- Quit - 종료 / 終了

### 설정 대화상자
- Interval (minutes) - 간격 (분) / 間隔（分）
- Volume - 볼륨 / 音量
- Mode - 모드 / モード
- Complete - 완성 모드 / 完全モード
- Combination - 조합 모드 / 組み合わせモード

---

# HurrieUp Internationalization

This project supports Korean, English, and Japanese.

## Translation Files

- `translations/ko_KR.ts` - Korean translation
- `translations/en_US.ts` - English translation
- `translations/ja_JP.ts` - Japanese translation

## How to Update Translations

### 1. Adding New Strings

When adding UI strings in code, always use the `tr()` function:

```cpp
QAction *action = new QAction(tr("New Text"), this);
trayIcon->setToolTip(tr("Status Message"));
```

### 2. Update Translation Files

After adding new strings, update the .ts files using Qt Linguist:

```bash
# Windows (adjust Qt SDK path)
D:/_DEV/_QT_SDK/6.9.3/msvc2022_64/bin/lupdate.exe HurrieUp/src -ts HurrieUp/translations/*.ts

# Or through CMake
cmake --build build --target HurrieUp_lupdate
```

### 3. Translation Work

Open .ts files with Qt Linguist to perform translations:

```bash
D:/_DEV/_QT_SDK/6.9.3/msvc2022_64/bin/linguist.exe HurrieUp/translations/ko_KR.ts
```

### 4. Generate .qm Files

After translation is complete, generate .qm files (CMake handles this automatically):

```bash
cmake --build build
```

## Language Detection

The application automatically detects the system language and displays the appropriate language:

- Korean system: Korean display
- Japanese system: Japanese display
- Others: English display (default)

---

# HurrieUp 国際化

このプロジェクトは韓国語、英語、日本語をサポートしています。

## 翻訳ファイル

- `translations/ko_KR.ts` - 韓国語翻訳
- `translations/en_US.ts` - 英語翻訳
- `translations/ja_JP.ts` - 日本語翻訳

## 翻訳の更新方法

### 1. 新しい文字列を追加する場合

コードでUI文字列を追加する際は、必ず`tr()`関数を使用してください：

```cpp
QAction *action = new QAction(tr("New Text"), this);
trayIcon->setToolTip(tr("Status Message"));
```

### 2. 翻訳ファイルの更新

新しい文字列を追加した後、Qt Linguistを使用して.tsファイルを更新します：

```bash
# Windows (Qt SDKパスを調整)
D:/_DEV/_QT_SDK/6.9.3/msvc2022_64/bin/lupdate.exe HurrieUp/src -ts HurrieUp/translations/*.ts

# またはCMakeを通じて
cmake --build build --target HurrieUp_lupdate
```

### 3. 翻訳作業

Qt Linguistを使用して.tsファイルを開き、翻訳作業を行います：

```bash
D:/_DEV/_QT_SDK/6.9.3/msvc2022_64/bin/linguist.exe HurrieUp/translations/ko_KR.ts
```

### 4. .qmファイルの生成

翻訳が完了したら、.qmファイルを生成します（CMakeが自動的に処理）：

```bash
cmake --build build
```

## 言語検出

アプリケーションはシステムの言語設定を自動的に検出し、適切な言語で表示されます：

- 韓国語システム：韓国語表示
- 日本語システム：日本語表示
- その他：英語表示（デフォルト）
