cmake_minimum_required(VERSION 3.30)
project(vocab2note C)

set(CMAKE_C_STANDARD 23)

add_executable(vocab2note src/main.c
        src/alphabet.h)

find_package(raylib REQUIRED)
target_link_libraries(vocab2note m pulse pulse-simple raylib)