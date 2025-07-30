cmake_minimum_required(VERSION 3.10)
project(chip8)

# Use c++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SOURCES
set(
    SOURCES
    main.cpp
    platform.cpp
    chip8.cpp
)

# exec target
add_executable(${PROJECT_NAME} ${SOURCES})

# find amd link sfml
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-audio)