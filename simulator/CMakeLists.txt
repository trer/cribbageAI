cmake_minimum_required(VERSION 3.28.0)

set(this CribbageAISimulator)

set(HEARDER_FILES
    abstraction.h
    action.h
    card.h
    cheatingutcplayer.h
    cribbage.h
    deck.h
    globparameters.h
    greedyplayer.h
    hand.h
    mccfrplayer.h
    player.h
    scorer.h
    statistics.h
    utils.h
)

set(source
    abstraction.cpp
    action.cpp
    card.cpp
    cheatingutcplayer.cpp
    cribbage.cpp
    deck.cpp
    greedyplayer.cpp
    hand.cpp
    mccfrplayer.cpp
    player.cpp
    scorer.cpp
    statistics.cpp
    utils.cpp
)


add_library(${this} STATIC ${source} ${HEADER_FILES})



