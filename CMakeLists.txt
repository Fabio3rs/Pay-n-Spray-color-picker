cmake_minimum_required(VERSION 3.16)
project("Pay'n'Spray Color Picker" CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_DEFAULT 17)
set(CXX17 ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_EXTENSIONS ON)

#include(CTest)
#enable_testing()
#add_subdirectory(googletest)
#include(GoogleTest)

file(GLOB SOURCES
    "src/*.cpp"
)

include_directories(SYSTEM include)

add_library(paynspray_internals STATIC ${PLUGIN_SDK_DEPENDENCIES} ${SOURCES})
target_compile_definitions(paynspray_internals PUBLIC -DGTASA=1 -DCRT_SECURE_NO_WARNINGS)
target_compile_options(paynspray_internals PUBLIC -no-pie -fno-pic)

add_library(paynspray SHARED ${ASI_SOURCES} ${SOURCES})
target_compile_definitions(paynspray PUBLIC -DGTASA=1)
target_compile_options(paynspray PUBLIC -no-pie -fno-pic -static)
target_link_libraries(paynspray PUBLIC paynspray_internals -static)

set_target_properties(
    paynspray 
    PROPERTIES
        OUTPUT_NAME "paynspray"
        PREFIX ""
        SUFFIX ".asi"
)
