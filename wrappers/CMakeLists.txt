cmake_minimum_required(VERSION 3.20)

set(this gui_bind)

project(${this} LANGUAGES CXX)

# won't work before project()!    
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(fix123 = 64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(fix123 = 32)
endif()



set(sources 
    bindings.cpp
    controller.cpp
    controllerwrapper.cpp
    ../simulator/cribbage.cpp
    ../simulator/player.cpp
)

set(headers
    ../simulator/cribbage.h
    ../simulator/player.h
    controller.h
    controllerwrapper.h
)

set(pybind11_DIR ${CMAKE_SOURCE_DIR}/venv/Lib/site-packages/pybind11/share/cmake/pybind11)
find_package(Python 3.7.9 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(${this} ${sources} ${headers})

target_link_libraries(${this} PUBLIC
    CribbageAISimulator
)
install(TARGETS ${this}
  COMPONENT python
  LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/venv/Lib/site-packages
  )