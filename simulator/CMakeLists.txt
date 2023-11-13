cmake_minimum_required(VERSION 3.28.0)

set(this CribbageAISimulator)

set(headers
    action.h
    card.h
    cribbage.h
    deck.h
    hand.h
    player.h
    scorer.h
)

set(sources
    action.cpp
    card.cpp
    cribbage.cpp
    deck.cpp
    hand.cpp
    player.cpp
    scorer.cpp
)

add_library(${this} STATIC ${headers} ${sources})