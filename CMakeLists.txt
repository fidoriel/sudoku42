cmake_minimum_required(VERSION 3.0)

set(CMAKE_BIULD_TYPE Debug)


project(sudoku42)

if(WIN32)
	# Tell find_package() where to look, via wxWidgets_ROOT_DIR…
	set(wxWidgets_ROOT_DIR "C:/cThrough/wxWidgets")
elseif(APPLE)
	set(wxWidgets_ROOT_DIR "/Users/lucakleinschmidt/wxWidgets")
	# Note, find_package() on OSX does not use wxWidgets_ROOT_DIR, use intermediate wx-config exe instead…
	set(wxWidgets_CONFIG_EXECUTABLE "${wxWidgets_ROOT_DIR}/buildOSX/wx-config")
endif()

find_package(wxWidgets COMPONENTS core base gl adv html xml xrc aui REQUIRED) # xrc needs adv

#if libs are static use --libs --static instead for standard libs.
set(WX_CONFIG_ARGS_LIBS "--libs")
set(WXWINDOWS_LIBRARIES "`${CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE} ${WX_CONFIG_ARGS_LIBS}`")

set(CMAKE_WXWINDOWS_CXX_FLAGS "`${CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE} --cxxflags`")

if(wxWidgets_FOUND)
	include("${wxWidgets_USE_FILE}")
	message("wxWidgets found!")
else(wxWidgets_FOUND)
	message("wxWidgets not found!")
endif(wxWidgets_FOUND)

# Define a variable containing a list of source files for the project
set(SRC_FILES
    main.cpp
    sudoku.cpp
    sudoku.h
    )

if(WIN32)
    # Include a RC file for windows
    list(APPEND SRC_FILES sample.rc)
elseif(APPLE)
    # Add an icon for the apple .app file
    list(APPEND SRC_FILES AppIcon.icns)
endif()

# Define the build target for the executable
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SRC_FILES})
# Link required libraries to the executable
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RESOURCE "AppIcon.icns"
        MACOSX_BUNDLE_ICON_FILE sAppIcon.icns
        MACOSX_BUNDLE_COPYRIGHT "Copyright fidoriel 2020"
        MACOSX_BUNDLE_GUI_IDENTIFIER "sudoku42"
        )
    add_custom_command(
        TARGET sudoku42
        POST_BUILD
        COMMAND plutil -replace NSHighResolutionCapable -bool true sudoku42.app/Contents/Info.plist
        )

endif()
