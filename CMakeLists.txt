cmake_minimum_required(VERSION 3.28.0)

set(this main1)

project(${this} LANGUAGES CXX)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(fix123 "64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(fix123 "32")# 32 bits
endif()

include(CTest)
enable_testing()

set(source
    main.cpp
)



add_executable(${this} ${source})

add_subdirectory(wrappers)
add_subdirectory(simulator)
#add_subdirectory(GUI)
# add_subdirectory(tests)
# add_subdirectory(googletest)


target_link_libraries(${this} PUBLIC
    CribbageAISimulator
)
