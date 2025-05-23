cmake_minimum_required(VERSION 3.15)
project(myApp VERSION 1.0)

add_library(Collector src1.cpp)
add_library(Algo src2.cpp)
add_library(Engine src3.cpp)
add_library(Ui src4.cpp)
add_executable(MyApp main.cpp)

target_link_libraries(Collector
    PUBLIC Ui
    PRIVATE Algo Engine
)

target_link_libraries(MyApp PRIVATE Collector)

# set($ENV{PATH} $ENV{PATH}:/usr/bin/fileo)

# In this example, the Ui library is linked to the Collector library as PUBLIC,
# so even though MyApp only directly links to Collector, 
# MyApp will also be linked to Ui because of that PUBLIC relationship. 
# The Algo and Engine libraries, on the other hand, 
# are linked to Collector as PRIVATE, so MyApp will not be directly linked to them. 