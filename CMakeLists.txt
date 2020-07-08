cmake_minimum_required(VERSION 3.16)
project(BiliBiliCCSubtitle)

set(CMAKE_CXX_STANDARD 14)
option(USE_CUSTOM_PRECOMPILED_CURL "switch on to define custom precompiled library libcurl" OFF)
option(USE_CUSTOM_PRECOMPILED_JSONCPP "switch on to define custom precompiled library jsoncpp" OFF)
option(BUILD_X86_64_VERSION "switch off to build 32bit version if you like." ON)
set(PATH_PRECOMPILED_CURL CACHE PATH "")
set(PATH_PRECOMPILED_JSONCPP CACHE PATH "")


if(NOT USE_CUSTOM_PRECOMPILED_CURL)
    message("Using precompiled libcurl")
    if(NOT FILE_CUSTOM_LIBCURL_FOUND)
        message("precompiled libcurl not found")
    endif(NOT FILE_CUSTOM_LIBCURL_FOUND)
endif(NOT USE_CUSTOM_PRECOMPILED_CURL)
#find_package(jsoncpp REQUIRED)
#find_package(curl REQUIRED)

#link_libraries(jsoncpp)
#link_libraries(curl)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-fexec-charset=gbk)
endif(CMAKE_COMPILER_IS_GNUCXX)

add_compile_options(-std=c++11)

add_executable(cctosrt convert_main.cpp)
add_executable(ccdown ccjson_downloader.cpp)