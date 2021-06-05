/*
 * Copyright 2020 NathanLi
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include "curl_helper.h"
using namespace std;
std::size_t write_Callback(void *contents,size_t size,size_t nmemb,void *userp)
{
    size_t realsize = size * nmemb;
    auto *mem = (struct Memory *)userp;
    mem->memory->insert(mem->memory->size(),(char*)contents,realsize);
    return realsize;
}
shared_ptr<string> CURLHelper::do_simple_get(string const &url)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(!curl)
    {
        cerr << "CURL initial failed!" << endl;
        exit(-1);
    }
    struct Memory mem;
    mem.memory=make_shared<string>();
    curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_Callback);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void*)&mem);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:78.0) Gecko/20100101 Firefox/78.0" );
    headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    headers = curl_slist_append(headers, "Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
    headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1" );
    headers = curl_slist_append(headers, "Cache-Control: max-age=0");
    headers = curl_slist_append(headers, "TE: Trailers");
    headers = curl_slist_append(headers, "Referer: https://www.bilibili.com/");
    headers = curl_slist_append(headers, "Origin: https://www.bilibili.com/");
    //headers = curl_slist_append(headers, "Host: https://www.bilibili.com/");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    return mem.memory;
}
bool CURLHelper::download_file(string const & url,string const &  filename)
{
    ofstream file(filename,ios::out|ios::trunc);
    if(!file.is_open())
    {
        cerr << "Failed to open file:" << filename << endl;
        return false;
    }
    auto raw=do_simple_get(url);
    file << *raw;
    file.close();
    return true;
}
void CURLHelper::do_curl_cleanup()
{
    curl_global_cleanup();
}