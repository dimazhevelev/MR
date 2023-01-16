cmake_minimum_required(VERSION 2.8.12)
project(example)

find_package(PythonInterp 3.6 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

add_subdirectory(pybind11)
pybind11_add_module(example
    MRBrain.cpp
    MRTimedThread.cpp)

