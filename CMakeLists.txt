#Change to directory where wxwidgets is located. 

#create build directory Mac: mkdir macbuild; cd macbuild
#create build directory Linux: mkdir gtkbuild; cd gtkbuild
#build instrucions for wxWidgets Platform Linux und macOS: ../configure --disable-shared --disable-sys-libs
#change to project directory and: mkdir build; cd build
#cmake corfigure with: cmake ..
#compile with: make -j4


#change build directory Win32: cd build\msw
#build instrucions for wxWidgets Platform Windows MinGW: mingw32-make SHELL=CMD.exe -j4 -f makefile.gcc BUILD=release UNICODE=1 SHARED=0 MONOLITHIC=0 DwxUSE_LIBPNG=builtin DwxUSE_ZLIB=builtin DCMAKE_POSITION_INDEPENDENT_CODE=ON
#the build command will fail the first and second time. the thrid will be succesful
#change to project directory and: mkdir build; cd build
#cmake CMD.exe: cmake .. -G "MinGW Makefiles"
#mingw32 Compile: mingw32-make -j4

cmake_minimum_required(VERSION 3.0)
set(CMAKE_BIULD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)

project(sudoku42)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}/")

message("CXX Compiler: ${CMAKE_CXX_COMPILER}")
message("CXX Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message("CXX Compiler is GNU: ${CMAKE_COMPILER_IS_GNUCXX}")

if(WIN32)
    message("Target: Windows32")
    #MONOLITHIC compiled value
    set(MONO 0)
    #directory where wxwidget is located root folder
    set(wxWidgets_BASE_DIR_WIN "C:/Users/lucak/wxWidgets-3.1.4-MONO0")
    include_directories(
        includes
        "${wxWidgets_BASE_DIR_WIN}/include/"
        "${wxWidgets_BASE_DIR_WIN}/lib/gcc_lib/mswu/"
    )

    link_directories(
        "${wxWidgets_BASE_DIR_WIN}/lib/gcc_lib/"
    )

    if(NOT MONO)
        set(WX_LIBS
            #MONOLITHIK=0
            libwxmsw31u_core.a #core needs to be the first one
            libwxbase31u.a
            libwxbase31u_net.a
            libwxbase31u_xml.a
            libwxexpat.a
            libwxjpeg.a
            libwxmsw31u_adv.a
            libwxmsw31u_aui.a
            libwxmsw31u_gl.a
            libwxmsw31u_html.a
            libwxmsw31u_media.a
            libwxmsw31u_propgrid.a
            libwxmsw31u_ribbon.a
            libwxmsw31u_richtext.a
            libwxmsw31u_stc.a
            libwxmsw31u_webview.a
            libwxmsw31u_xrc.a
            libwxpng.a
            libwxregexu.a
            libwxscintilla.a
            libwxtiff.a
            libwxzlib.a
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

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Target: Linux")
    #directory where wxwidget is located root folder
    set(wxWidgets_ROOT_DIR "/home/luca/wxWidgets-3.1.4")
    set(wxWidgets_CONFIG_EXECUTABLE "${wxWidgets_ROOT_DIR}/gtkbuild/wx-config")
elseif(APPLE)
    message("Target: Apple")
    #directory where wxwidget is located root folder
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
    set(CMAKE_CXX_FLAGS "-mwindows -luxtheme -loleacc -lversion -lshlwapi -lcomctl32 -static-libstdc++ -static-libgcc")
    add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})
    target_link_libraries(${PROJECT_NAME} ${WX_LIBS} ${CMAKE_CXX_FLAGS})
elseif(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRC_FILES})
    target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS "-no-pie")
    add_executable(${PROJECT_NAME} ${SRC_FILES})
    target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${CMAKE_CXX_FLAGS})
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