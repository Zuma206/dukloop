cmake_minimum_required(VERSION 3.30)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
project(dukloop)

add_executable(${PROJECT_NAME} src/main.c)

find_package(duktape REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE duktape::duktape)