cmake_minimum_required(VERSION 3.10)
project(Solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Solver
    main.cpp
    Solution.cpp
    Solution.hpp
)
