cmake_minimum_required(VERSION 3.0)

project(lvl-ip C)

file(GLOB SOURCE_FILES_CC src/*.c)
add_executable(o ${SOURCE_FILES_CC})
target_include_directories(o PRIVATE include)
