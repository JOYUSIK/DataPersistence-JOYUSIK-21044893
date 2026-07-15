cmake_minimum_required(VERSION 3.20)
project(DataPersistence CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
    DOWNLOAD_NO_EXTRACT TRUE
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/third_party
)
FetchContent_MakeAvailable(nlohmann_json)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(DataPersistence ${SOURCES})
target_include_directories(DataPersistence PRIVATE include)
