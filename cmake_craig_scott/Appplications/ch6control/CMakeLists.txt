cmake_minimum_required(VERSION 3.15)
project(control VERSION 1.0.0)
add_executable(controller main.cxx)

set(is_messy YES)

# BOOLEAN
if("ON" AND is_messy)
    message("whattt")
else()
    message("shit")
endif()

# VERSION
if(1.2.8 VERSION_LESS 1.4.0)
    message("hello")
endif()

# FILE
if(EXISTS ../main.cxx)
    message("good")
endif()

# DIRECTORY
if(IS_DIRECTORY ..)
    message("booo")
endif()

# more basic loop - to avoid
foreach(listVar hello big naoar)
    message(${listVar})
endforeach()

# more general - seperate items from list
set(list1 mico choco podo)
set(list2 "mica" "chaca" "pada")
foreach(listVar IN LISTS list1 list2 ITEMS great "mother")
    message("value: ${listVar}")
endforeach()

# In Zip list - like python list
set(list1 A B C D)
set(list2 1 2 3 4)
foreach(var1 var2 IN ZIP_LISTS list1 list2)
    message("var1/var2: ${var1}/${var2}")    
endforeach()

foreach(var IN ZIP_LISTS list1 list2)
        message("var_0/var_1: ${var_0}/${var_1}")        
endforeach()


