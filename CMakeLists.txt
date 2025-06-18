cmake_minimum_required(VERSION 3.10)
project(JOH)

set(CMAKE_CXX_STANDARD 17)

find_package(cxxopts CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(SOURCES
    joh_main.cpp
    arg_parser.cpp
    fs_system.cpp
)

add_executable(joh ${SOURCES})

target_link_libraries(joh cxxopts::cxxopts)
target_link_libraries(joh nlohmann_json::nlohmann_json)

if(WIN32)
    target_link_libraries(joh shell32 ole32)

    target_compile_definitions(joh PRIVATE _WIN32_WINNT=0x0601)
endif()