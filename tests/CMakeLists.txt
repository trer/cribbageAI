cmake_minimum_required(VERSION 3.28.0)

set(this maintest)
set(TARGET_FILE tests/maintest.cpp)

include(CTest)
enable_testing()

set(sources
    utilstest.cpp
    cardtest.cpp
    decktest.cpp
    handtest.cpp
    maintest.cpp
    scorertest.cpp
    cribbagetest.cpp
)


add_executable(${this} ${sources})

target_link_libraries(${this} PUBLIC STATIC
    gtest_main
    CribbageAISimulator
)


add_test(
    NAME ${this}
    COMMAND ${this}
)
