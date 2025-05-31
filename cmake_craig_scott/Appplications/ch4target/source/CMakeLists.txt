cmake_minimum_required(VERSION 3.15)
project(myApp VERSION 1.0)

add_library(Collector src1.cpp)
add_library(Algo src2.cpp)
add_library(Engine SHARED src3.cpp )
add_library(Ui src4.cpp)
add_executable(MyApp main.cpp)

# Collector use ALGO, Engine for its internal implementation, They are not needed for any binary depending on Collector >> PRIVATE
# Collector use UI both for its internal implementation and interface to be used by another binary, any binary depending on Collector built has Ui as dependency.
target_link_libraries(Collector
    PUBLIC Ui 
    PRIVATE Algo Engine
)

# MyApp depend on Collector for its internam implmentation.
target_link_libraries(MyApp PRIVATE Collector)

# set($ENV{PATH} $ENV{PATH}:/usr/bin/fileo)

# In this example, the Ui library is linked to the Collector library as PUBLIC,
# so even though MyApp only directly links to Collector, 
# MyApp will also be linked to Ui because of that PUBLIC relationship. 
# The Algo and Engine libraries, on the other hand, 
# are linked to Collector as PRIVATE, so MyApp will not be directly linked to them. 