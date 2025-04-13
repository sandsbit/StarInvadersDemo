cmake_minimum_required(VERSION 3.31)
project(StarInvadersDemo C)

set(CMAKE_C_STANDARD 23)

include_directories(src/main)

add_executable(StarInvadersDemo src/main/main.c)
