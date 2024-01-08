cmake_minimum_required(VERSION 3.20)

project(gui_bind LANGUAGES CXX)

set(pybind11_DIR ${CMAKE_SOURCE_DIR}/venv/Lib/site-packages/pybind11/share/cmake/pybind11)
find_package(Python 3.7.9 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(gui_bind simulator.cpp)

install(TARGETS gui_bind
  COMPONENT python
  LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/venv/Lib/site-packages
  )