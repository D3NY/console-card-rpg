cmake_minimum_required(VERSION 3.17)
project(zabojdan)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -pedantic -Wextra -g ${CMAKE_CXX_FLAGS}")

include_directories(src/)

add_executable(zabojdan
        src/main.cpp
        src/CApplication.h src/CApplication.cpp
        src/CGame.h src/CGame.cpp
        src/CCard.h src/CCard.cpp
        src/CEffect.h src/CEffect.cpp
        src/CEffectAttack.h src/CEffectAttack.cpp
        src/CEffectDefend.h src/CEffectDefend.cpp
        src/CEffectHeal.h src/CEffectHeal.cpp
        src/CEffectMana.h src/CEffectMana.cpp
        src/CPlayer.h src/CPlayer.cpp
        src/CPlayerHuman.h src/CPlayerHuman.cpp
        src/CPlayerRobot.h src/CPlayerRobot.cpp
        src/CInterface.h src/CInterface.cpp
        src/CIOHandler.h src/CIOHandler.cpp
        src/CStats.cpp src/CStats.h
        src/EEffectType.h
        src/EGameType.h)