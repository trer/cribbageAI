cmake_minimum_required(VERSION 3.28.0)

set(this CribbageAISimulator)

set(HEARDER_FILES
    action.h
    card.h
    cribbage.h
    deck.h
    hand.h
    player.h
    scorer.h
    utils.h
)

set(source
    action.cpp
    card.cpp
    cribbage.cpp
    deck.cpp
    hand.cpp
    player.cpp
    scorer.cpp
    utils.cpp
)


add_library(${this} STATIC ${source} ${HEADER_FILES})



