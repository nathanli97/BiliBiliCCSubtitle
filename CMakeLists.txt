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

find_package(jsoncpp CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(boost CONFIG REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-fexec-charset=gbk -w)
endif(CMAKE_COMPILER_IS_GNUCXX)

add_executable(ccdown ccjson_downloader.cpp ccjson_convert.cpp main.cpp ccjson.h curl_helper.cpp curl_helper.h ccjson_downloader.h common.h common.cpp ccjson_convert.h)
target_link_libraries(ccdown PRIVATE jsoncpp_lib jsoncpp_object)
target_link_libraries(ccdown PRIVATE CURL::libcurl)