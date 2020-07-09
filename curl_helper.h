//
// Created by taboo on 2020/7/9.
//

#ifndef BILIBILICCSUBTITLE_CURL_HELPER_H
#define BILIBILICCSUBTITLE_CURL_HELPER_H
#include <iostream>
#include <memory>
#include <fstream>
#include "curl/curl.h"
std::shared_ptr<std::string> do_simple_get(std::string const &url);
bool download_file(std::string const & url,std::string const &  filename);
void do_curl_cleanup();
#endif //BILIBILICCSUBTITLE_CURL_HELPER_H
