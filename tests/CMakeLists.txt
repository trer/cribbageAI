cmake_minimum_required(VERSION 3.28.0)

set(this maintest)
set(TARGET_FILE tests/maintest.cpp)

include(CTest)
enable_testing()

set(sources
    utilstest.cpp
    cardtest.cpp
    cheatingutcplayertest.cpp
    decktest.cpp
    greedyplayertest.cpp
    handtest.cpp
    maintest.cpp
    mccfrplayertest.cpp
    playertest.cpp
    scorertest.cpp
    cribbagetest.cpp
)


add_executable(${this} ${sources})

target_link_libraries(${this} PUBLIC
    gtest_main
    CribbageAISimulator
)


add_test(
    NAME ${this}
    COMMAND ${this}
)
