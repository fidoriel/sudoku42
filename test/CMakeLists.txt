cmake_minimum_required(VERSION 3.0.0)
project(sudoku42cmdl VERSION 0.1.0)

include(CTest)
enable_testing()

set(SRC_F
    main.cpp
    sudoku.cpp
    sudoku.h
    )

add_executable(sudoku42cmdl ${SRC_F})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
