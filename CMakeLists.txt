cmake_minimum_required(VERSION 3.11)

project(
    celeste_dll_1
    VERSION 0.1
    DESCRIPTION "My first hacking dll for Celeste game"
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(DLL_SOURCE_FILES src/dllmain.cpp
src/InfiniteDashesHack.cpp
src/MemoryHelper.cpp
src/Logger.cpp
src/WindowsManager.cpp)

add_library(celeste_hack_dll_1 SHARED ${DLL_SOURCE_FILES})