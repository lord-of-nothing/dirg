cmake_minimum_required(VERSION 3.10)

project(cut VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/clang_format.cmake)

add_format_target()

if(NOT WIN32)
    set(WIN32 0)
endif()

# compiler options build everywhere cpp sample but other functions clang only because of using llvm
if(MSVC)
    add_compile_options("/W3" "$<$<CONFIG:RELEASE>:/O2>")
else()
    add_compile_options("-Wall" "-Wextra" "$<$<CONFIG:RELEASE>:-O2>")
endif()

add_subdirectory(backend)
add_subdirectory(frontend)
