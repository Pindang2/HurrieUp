# HurrieUP
허리피세요 리마인드 프로그램
일정시간마다 허리를 펴라고 해줍니다

A reminder application that helps you maintain a good posture by playing sounds at regular intervals.

## ✨ 기능 (Features)

- **시스템 트레이 실행**: 백그라운드에서 실행되며 시스템 트레이 아이콘으로 접근할 수 있습니다.
- **사용자 맞춤 소리**: 다양한 소리 조합(알림음 + 음성 또는 완성된 효과음)을 설정할 수 있습니다.
- **알림 간격 조절**: 원하는 시간 간격으로 알림을 받을 수 있습니다.
- **세부 설정**: 소리 볼륨, 알림음과 음성 사이의 간격 등을 조절할 수 있습니다.
- **일시정지 기능**: 필요에 따라 미리 설정된 시간 동안 알림을 일시적으로 중지할 수 있습니다.
- **다국어 지원**: 한국어, 영어, 일본어를 지원합니다.

`- **크로스플랫폼**: Windows, macOS, Linux에서 모두 사용할 수 있습니다. (예정)`

## 🛠️ 빌드 방법 (How to Build)

이 프로젝트는 C++와 Qt6를 사용하여 개발되었습니다. 빌드하려면 C++ 컴파일러, CMake, 그리고 Qt6 라이브러리(Widgets, Multimedia)가 필요합니다.

Windows 환경에서는 `vcpkg`를 사용하여 Qt6를 쉽게 설치할 수 있습니다.

1.  **vcpkg 설치**
    ```shell
    # vcpkg를 설치할 경로로 이동합니다.
    cd D:\_DEV
    # vcpkg 리포지토리를 복제합니다.
    git clone https://github.com/microsoft/vcpkg.git
    # vcpkg를 부트스트랩합니다.
    .\vcpkg\bootstrap-vcpkg.bat
    ```

2.  **Qt6 라이브러리 설치**
    ```shell
    # Qt6 관련 라이브러리를 설치합니다. (시간이 오래 걸릴 수 있습니다)
    .\vcpkg\vcpkg.exe install qtbase:x64-windows qtmultimedia:x64-windows
    ```

3.  **CMake와 vcpkg 연동**
    ```shell
    # vcpkg를 시스템에 통합하여 CMake가 라이브러리를 찾을 수 있도록 합니다.
    .\vcpkg\vcpkg.exe integrate install
    ```

4.  **프로젝트 빌드**

### Qt 온라인 인스톨러 사용 (대안)

`vcpkg`를 통한 설치에 문제가 있다면, Qt 공식 온라인 인스톨러를 사용할 수도 있습니다.

1.  [Qt Online Installer](https://www.qt.io/download-qt-installer)를 다운로드하고 실행합니다.
2.  Qt 계정으로 로그인한 후, "Custom installation"을 선택합니다.
3.  설치할 Qt 버전(예: Qt 6.x.x)을 선택하고, 아래 컴포넌트가 반드시 포함되도록 합니다.
    *   사용하는 컴파일러에 맞는 버전 (예: `MSVC 2019 64-bit`)
    *   `Additional Libraries` > `Qt Multimedia`
4.  설치가 완료되면, CMake가 Qt 라이브러리를 찾을 수 있도록 `CMAKE_PREFIX_PATH` 환경 변수를 설정해야 합니다. 예를 들어, Qt가 `C:\Qt\6.7.0\msvc2019_64`에 설치되었다면 이 경로를 환경 변수에 추가합니다.

    CMake를 실행할 때 다음과 같이 직접 경로를 지정할 수도 있습니다.
    ```shell
    cmake -B build -DCMAKE_PREFIX_PATH=C:\Qt\6.x.x\msvc2019_64
    ```

---

CMake와 C++ 컴파일러가 설치된 환경에서 아래 명령어를 실행하여 프로젝트를 빌드하세요.
    ```shell
    cmake -B build
    cmake --build build
    ```

## 🚀 사용법 (Usage)

빌드된 실행 파일을 실행하면 시스템 트레이에 아이콘이 나타납니다. 트레이 아이콘을 마우스 오른쪽 버튼으로 클릭하여 다음 메뉴를 사용할 수 있습니다.

- **일시정지**: 특정 시간 동안 알림을 끕니다.
- **설정**: 알림 간격, 소리 종류, 볼륨 등을 변경할 수 있는 설정 창을 엽니다.
- **종료**: 애플리케이션을 종료합니다.

## 📜 라이선스 (License)

이 프로젝트는 [GNU Lesser General Public License v3.0](LICENSE)에 따라 배포됩니다.

