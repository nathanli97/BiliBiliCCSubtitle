//
// Created by taboo on 2020/7/9.
//

#ifndef BILIBILICCSUBTITLE_COMMON_H
#define BILIBILICCSUBTITLE_COMMON_H

extern bool verbose;

#ifdef _WIN32
#include <windows.h>
#include <iostream>
std::string Utf8ToGbk(const char *src_str);
#endif

#endif //BILIBILICCSUBTITLE_COMMON_H
