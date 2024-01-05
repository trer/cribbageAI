cmake_minimum_required(VERSION 3.28.0)

set(this main)


project(${this} VERSION 0.1.0 LANGUAGES C CXX)


include(CTest)
enable_testing()

set(source
    main.cpp
)

file (GLOB PYTHON_FILES "GUI/*.cpp" "GUI/*.hpp")


add_executable(${this} ${source})

add_subdirectory(simulator)

set(python_LIBRARIES "${CMAKE_SOURCE_DIR}/venv/Lib/site-packages")
set(Python_EXECUTABLE "${CMAKE_SOURCE_DIR}/venv/Scripts/python")
set(pybind11_DIR ${CMAKE_SOURCE_DIR}/venv/Lib/site-packages/pybind11/share/cmake/pybind11)
find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)



pybind11_add_module(py${this}
	${source}
	${HEADER_FILES}
	${PYTHON_FILES}
)

install(TARGETS py${this}
  COMPONENT python
  LIBRARY DESTINATION "${python_LIBRARIES}"
  )

add_subdirectory(tests)
add_subdirectory(googletest)





target_link_libraries(${this} PUBLIC
    CribbageAISimulator
)




