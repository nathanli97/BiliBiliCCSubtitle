cmake_minimum_required(VERSION 3.16)
project(BiliBiliCCSubtitle)

set(CMAKE_CXX_STANDARD 14)

find_package(jsoncpp REQUIRED)
find_package(curl REQUIRED)

link_libraries(jsoncpp)
link_libraries(curl)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-fexec-charset=gbk)
endif(CMAKE_COMPILER_IS_GNUCXX)

add_compile_options(-std=c++11)

add_executable(cctosrt convert_main.cpp)
add_executable(ccdown ccjson_downloader.cpp)