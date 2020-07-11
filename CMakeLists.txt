#
# Copyright 2020 NathanLi
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

cmake_minimum_required(VERSION 3.16)
project(BiliBiliCCSubtitle)

set(CMAKE_CXX_STANDARD 14)
option(USE_CUSTOM_PRECOMPILED_CURL "switch on to define custom precompiled library libcurl" OFF)
option(USE_CUSTOM_PRECOMPILED_JSONCPP "switch on to define custom precompiled library jsoncpp" OFF)
option(BUILD_X86_64_VERSION "switch off to build 32bit version if you like." ON)
set(PATH_PRECOMPILED_CURL CACHE PATH "")
set(PATH_PRECOMPILED_JSONCPP CACHE PATH "")
if(BUILD_X86_64_VERSION)
    message("You are going to build X86_64 version of BiliBiliCCSubtitle")
else(BUILD_X86_64_VERSION)
    message("You are going to build X86 version of BiliBiliCCSubtitle")
endif(BUILD_X86_64_VERSION)

if(NOT USE_CUSTOM_PRECOMPILED_CURL)
    if(BUILD_X86_64_VERSION)
        message("Using precompiled libcurl x86_64")
        file(ARCHIVE_EXTRACT INPUT ${PROJECT_SOURCE_DIR}/3rdparty/curl-7.71.1-win64-mingw.zip DESTINATION ${PROJECT_BINARY_DIR}/3rdparty)
        set(PATH_PRECOMPILED_CURL ${PROJECT_BINARY_DIR}/3rdparty/curl-7.71.1-win64-mingw)
    else(BUILD_X86_64_VERSION)
        message("Using precompiled libcurl x86")
        file(ARCHIVE_EXTRACT INPUT ${PROJECT_SOURCE_DIR}/3rdparty/curl-7.71.1-win32-mingw.zip DESTINATION ${PROJECT_BINARY_DIR}/3rdparty)
        set(PATH_PRECOMPILED_CURL ${PROJECT_BINARY_DIR}/3rdparty/curl-7.71.1-win32-mingw)
    endif(BUILD_X86_64_VERSION)
endif(NOT USE_CUSTOM_PRECOMPILED_CURL)
include_directories(${PATH_PRECOMPILED_CURL}/include)
link_directories(${PATH_PRECOMPILED_CURL}/lib)

if(NOT USE_CUSTOM_PRECOMPILED_JSONCPP)
    if(BUILD_X86_64_VERSION)
        message("Using precompiled JSONCPP x86_64")
        file(ARCHIVE_EXTRACT INPUT ${PROJECT_SOURCE_DIR}/3rdparty/jsoncpp-1.9.3-win64-mingw.zip DESTINATION ${PROJECT_BINARY_DIR}/3rdparty)
        set(PATH_PRECOMPILED_JSONCPP ${PROJECT_BINARY_DIR}/3rdparty/jsoncpp-1.9.3-win64-mingw)
    else(BUILD_X86_64_VERSION)
        message("Using precompiled JSONCPP x86")
        file(ARCHIVE_EXTRACT INPUT ${PROJECT_SOURCE_DIR}/3rdparty/jsoncpp-1.9.3-win32-mingw.zip DESTINATION ${PROJECT_BINARY_DIR}/3rdparty)
        set(PATH_PRECOMPILED_JSONCPP ${PROJECT_BINARY_DIR}/3rdparty/jsoncpp-1.9.3-win32-mingw)
    endif(BUILD_X86_64_VERSION)
endif(NOT USE_CUSTOM_PRECOMPILED_JSONCPP)
include_directories(${PATH_PRECOMPILED_JSONCPP}/include)
link_directories(${PATH_PRECOMPILED_JSONCPP}/lib)

link_libraries(jsoncpp)
link_libraries(curl)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-fexec-charset=gbk -w)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(NOT BUILD_X86_64_VERSION)
    add_compile_options(-m32)
endif(NOT BUILD_X86_64_VERSION)

add_compile_options(-std=c++11)

add_executable(ccdown ccjson_downloader.cpp ccjson_convert.cpp main.cpp ccjson.h curl_helper.cpp curl_helper.h ccjson_downloader.h common.h common.cpp ccjson_convert.h)
