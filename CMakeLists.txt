cmake_minimum_required(VERSION 3.10)
project(Wrapper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(SOURCES
    source/main.cpp
    source/TestSubjects/Subject1.cpp
    source/Tests/Test.cpp
)

include_directories(source/TestSubjects)
include_directories(source)

add_executable(${PROJECT_NAME} ${SOURCES})