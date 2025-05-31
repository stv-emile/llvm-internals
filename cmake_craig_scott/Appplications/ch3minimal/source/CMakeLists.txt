cmake_minimum_required(VERSION 3.5)
project(minimal 
    VERSION 4.7.2 
    LANGUAGES CXX)
# use --target targetName to build a specific target.
# eg. cmake --build build_dir --target main

set(TESTED "YES" CACHE BOOL "Verify if tested")
set(TESTED "NO")
option(NOT_TESTED "verify if not tested" "NO")
add_executable(main main.cpp)
add_executable(main_deb main.cpp)
add_executable(main_disc EXCLUDE_FROM_ALL main.cpp)
