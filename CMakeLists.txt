#build instrucions for wxWidgets Platform Linux und macOS: ../configure --disable-shared --disable-sys-libs

#build instrucions for wxWidgets Platform Windows MinGW: mingw32-make SHELL=CMD.exe -j4 -f makefile.gcc BUILD=release UNICODE=1 SHARED=0 MONOLITHIC=1 DwxUSE_LIBPNG=builtin DwxUSE_ZLIB=builtin DCMAKE_POSITION_INDEPENDENT_CODE=ON
#cmake CMD.exe: cmake . -G "MinGW Makefiles"
#mingw32 Compile: mingw32-make -f Makefile

cmake_minimum_required(VERSION 3.0)
set(CMAKE_BIULD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)

project(sudoku42)

message("C Compiler: ${CMAKE_C_COMPILER}")
message("C Compiler ID: ${CMAKE_C_COMPILER_ID}")
message("C Compiler is GNU: ${CMAKE_COMPILER_IS_GNUC}")
message("C Compiler: ${CMAKE_CXX_COMPILER}")
message("C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message("C++ Compiler is GNU: ${CMAKE_COMPILER_IS_GNUCXX}")

if(WIN32)
    message("Target: Windows32")
    set(MONO 1)
    set(wxWindowsBaseDir "C:/Users/lucak/wxWidgets-3.1.4")
    include_directories(
        includes
        ${wxWindowsBaseDir}/include/
        ${wxWindowsBaseDir}/lib/gcc_lib/mswu/
    )

    link_directories(
        ${wxWindowsBaseDir}/lib/gcc_lib/
    )

    if(NOT MONO)
        set(WX_LIBS
            #MONOLITHIK=0
            libwx_baseu_net-3.1.a
            libwx_baseu_xml-3.1.a
            libwx_baseu-3.1.a
            libwx_mswu_adv-3.1.a
            libwx_mswu_core-3.1.a
            libwx_mswu_adv-3.1.a
            libwx_mswu_gl-3.1.a
            libwx_mswu_html-3.1.a
            libwx_mswu_media-3.1.a
            libwx_mswu_propgrid-3.1.a
            libwx_mswu_qa-3.1.a
            libwx_mswu_ribbon-3.1.a
            libwx_mswu_richtext-3.1.a
            libwx_mswu_stc-3.1.a
            libwx_mswu_webview-3.1.a
            libwx_mswu_xrc-3.1.a
            libwxexpat-3.1.a
            libwxjpeg-3.1.a
            libwxpng-3.1.a
            libwxregexu-3.1.a
            libwxscintilla-3.1.a
            libwxtiff-3.1.a
            libwxzlib-3.1.a
        )
    elseif(MONO)
        set(WX_LIBS
            #MONOLITHIK=1
            libwxexpat.a
            libwxjpeg.a
            libwxmsw31u.a
            libwxmsw31u_gl.a
            libwxpng.a
            libwxregexu.a
            libwxscintilla.a
            libwxzlib.a
            libwxtiff.a
        )
    endif()
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Target: Linux")
    set(wxWidgets_ROOT_DIR "/home/luca/wxWidgets-3.1.4")
    set(wxWidgets_CONFIG_EXECUTABLE "${wxWidgets_ROOT_DIR}/gtkbuild/wx-config")
elseif(APPLE)
    message("Target: Apple")
	set(wxWidgets_ROOT_DIR "/Users/lucakleinschmidt/wxWidgets-3.1.4")
	set(wxWidgets_CONFIG_EXECUTABLE "${wxWidgets_ROOT_DIR}/macbuild/wx-config")
endif()

if(NOT WIN32)
    #find_package(wxWidgets COMPONENTS core base gl adv html xml xrc aui REQUIRED) Modified Linux has not all??
    find_package(wxWidgets COMPONENTS core base adv html xml xrc aui REQUIRED)

    #if libs are static use --libs --static instead for standard libs.
    set(WX_CONFIG_ARGS_LIBS "--libs --static")
    set(WXWINDOWS_LIBRARIES "`${CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE} ${WX_CONFIG_ARGS_LIBS}`")

    set(CMAKE_WXWINDOWS_CXX_FLAGS "`${CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE} --cxxflags`")

    if(wxWidgets_FOUND)
	    include("${wxWidgets_USE_FILE}")
	    message("wxWidgets was found")
    else(wxWidgets_FOUND)
	    message("wxWidgets was not found")
    endif(wxWidgets_FOUND)
endif()

set(SRC_FILES
    src/GUIsudoku.cpp
    src/sudoku.cpp
    src/sudoku.h
    )

if(WIN32)
    # Include a RC file for windows
    list(APPEND SRC_FILES icons/sample.rc)
elseif(APPLE)
    # Add an icon for the apple .app file
    list(APPEND SRC_FILES icons/AppIcon.icns)
endif()

if(WIN32)
    #-mwindows suppresses CMD
    set(CMAKE_CXX_FLAGS "-mwindows -luxtheme -loleacc -lversion -lshlwapi -lcomctl32 -s")
    add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})
    target_link_libraries(${PROJECT_NAME} ${WX_LIBS} ${CMAKE_CXX_FLAGS})
elseif(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRC_FILES})
    target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS "-no-pie")
    add_executable(${PROJECT_NAME} ${SRC_FILES})
    target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
endif()

#After compilation processing
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RESOURCE "icons/AppIcon.icns"
        MACOSX_BUNDLE_ICON_FILE AppIcon.icns
        MACOSX_BUNDLE_COPYRIGHT "Copyright fidoriel 2020"
        MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_NAME}"
        )
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND plutil -replace NSHighResolutionCapable -bool true ${PROJECT_NAME}.app/Contents/Info.plist
        )
endif()