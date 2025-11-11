# CMakeList.txt: HurrieUp에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#

# Find Qt LinguistTools for translations
find_package(Qt6 REQUIRED COMPONENTS LinguistTools)

# Qt translation files
set(TS_FILES
    translations/ko_KR.ts
    translations/en_US.ts
    translations/ja_JP.ts
)

# 이 프로젝트의 실행 파일에 소스를 추가합니다.
# Windows에서는 WIN32 플래그를 사용하여 콘솔 창 없이 실행
if(WIN32)
    add_executable (HurrieUp WIN32
        "src/main.cpp"
        "src/mainwindow.h"
        "src/mainwindow.cpp"
        "src/settingsdialog.h"
        "src/settingsdialog.cpp"
        "src/soundinfodialog.h"
        "src/soundinfodialog.cpp"
        "src/aboutdialog.h"
        "src/aboutdialog.cpp"
        "resources.qrc"
        "translations/translations.qrc"
        ${TS_FILES}
    )
else()
    # Linux/Unix에서는 일반 실행 파일로 빌드
    add_executable (HurrieUp
        "src/main.cpp"
        "src/mainwindow.h"
        "src/mainwindow.cpp"
        "src/settingsdialog.h"
        "src/settingsdialog.cpp"
        "src/soundinfodialog.h"
        "src/soundinfodialog.cpp"
        "src/aboutdialog.h"
        "src/aboutdialog.cpp"
        "resources.qrc"
        "translations/translations.qrc"
        ${TS_FILES}
    )
endif()

# Windows 아이콘 리소스 추가
if(WIN32)
    target_sources(HurrieUp PRIVATE "HurrieUp.rc")
endif()

# Link to the Qt libraries
target_link_libraries(HurrieUp PRIVATE Qt6::Widgets Qt6::Multimedia)

# Configure translations after target is created
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/translations")
qt_add_lupdate(SOURCE_TARGETS HurrieUp TS_FILES ${TS_FILES})
qt_add_lrelease(TS_FILES ${TS_FILES} QM_FILES_OUTPUT_VARIABLE QM_FILES)

# Copy .qm files to source directory so they can be included in resources
add_custom_command(TARGET HurrieUp PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/translations/ko_KR.qm
        ${CMAKE_CURRENT_SOURCE_DIR}/translations/ko_KR.qm
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/translations/en_US.qm
        ${CMAKE_CURRENT_SOURCE_DIR}/translations/en_US.qm
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/translations/ja_JP.qm
        ${CMAKE_CURRENT_SOURCE_DIR}/translations/ja_JP.qm
    COMMENT "Copying .qm files to source directory..."
)

# Platform-specific post-build steps
if(WIN32)
    # Windows: Run windeployqt to copy Qt dependencies
    add_custom_command(TARGET HurrieUp POST_BUILD
        COMMAND "D:/_DEV/_QT_SDK/6.9.3/msvc2022_64/bin/windeployqt.exe"
            --pdb
            --$<IF:$<CONFIG:Debug>,debug,release>
            $<TARGET_FILE:HurrieUp>
        COMMENT "Running windeployqt..."
    )
elseif(UNIX AND NOT APPLE)
    # Linux: Install target (optional, for system-wide installation)
    install(TARGETS HurrieUp DESTINATION bin)
    
    # Linux: Create AppImage-compatible structure (optional)
    # You can use linuxdeploy or similar tools for deployment
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET HurrieUp PROPERTY CXX_STANDARD 20)
endif()

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.
