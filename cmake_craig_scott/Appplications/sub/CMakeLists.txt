cmake_minimum_required(VERSION 3.0)
project(sub VERSION 1.2 LANGUAGES CXX)

add_executable(subexec main.cxx)


message("top:CMAKE_SOURCE_DIR         = ${CMAKE_SOURCE_DIR}")
message("top:CMAKE_BINARY_DIR         = ${CMAKE_BINARY_DIR}")
message("top:CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("top:CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")

set(myVar foo)
message("Parent (before): myVar    = ${myVar}")
message("Parent (before): childVar = ${childVar}")


add_subdirectory(mysub)

message("top:   CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("top:   CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")

message("Parent (after):  myVar    = ${myVar}")
message("Parent (after):  childVar = ${childVar}")



function(func arg)
    if(DEFINED arg)
        message("Function arg is a defined variable")
    else()
        message("Function arg is NOT a defined variable")
    endif()
endfunction()
macro(macr arg)
    if(DEFINED arg)
        message("Macro arg is a defined variable")
    else()
        message("Macro arg is NOT a defined variable")
    endif()
endmacro()

func(foobar)
macr(foobar)