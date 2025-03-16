cmake_minimum_required(VERSION 3.5)

project(asm1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(asm1 main.cpp assembly.cpp)

install(TARGETS asm1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
