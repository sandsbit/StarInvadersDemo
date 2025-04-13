cmake_minimum_required(VERSION 3.31)
project(StarInvadersDemo C)

set(CMAKE_C_STANDARD 23)

include_directories(src/main)

add_library(StarInvadersLogic SHARED
        src/main/logic/gamelogic.c
        src/main/logic/gamelogic.h
        src/main/utils/structs.h)

add_executable(StarInvadersDemo src/main/main.c
        src/main/logic/gamelogic.c
        src/main/logic/gamelogic.h
        src/main/utils/structs.h)
target_link_libraries(StarInvadersDemo StarInvadersLogic)
