cmake_minimum_required(VERSION 3.28.0)

set(this main)


project(${this} VERSION 0.1.0 LANGUAGES C CXX)


include(CTest)
enable_testing()

set(source
    main.cpp
)

add_executable(${this} ${source})

add_subdirectory(googletest)
add_subdirectory(simulator)
add_subdirectory(tests)

target_link_libraries(${this} PUBLIC
    CribbageAISimulator
)





