//
// Created by taboo on 2020/7/9.
//

#ifndef BILIBILICCSUBTITLE_CCJSON_DOWNLOADER_H
#define BILIBILICCSUBTITLE_CCJSON_DOWNLOADER_H
#include <iostream>
#include <regex>
#include "curl_helper.h"
int do_download_json(std::string const & inputfile,int p_start,int p_end,bool auto_convert=false);
#endif //BILIBILICCSUBTITLE_CCJSON_DOWNLOADER_H
