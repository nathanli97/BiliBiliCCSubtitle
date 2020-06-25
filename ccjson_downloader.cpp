//
// Created by taboo on 2020/6/25.
//
#include <iostream>
#include <regex>
#include <memory>
#include <fstream>
#include <curl/curl.h>
using namespace std;
struct Memory{
    shared_ptr<string> memory;
};
static size_t write_Callback(void *contents,size_t size,size_t nmemb,void *userp)
{
    size_t realsize = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;
    mem->memory->insert(mem->memory->size(),(char*)contents,realsize);
    return realsize;
}
void usage(string program){
    cout << "Usage: " << program << " [-o outputfile] input_url" << endl;
    cout << "input_url: Bilibili Player page" << endl;
}
shared_ptr<string> simple_get(string url)
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
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:77.0) Gecko/20100101 Firefox/77.0" );
    headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    headers = curl_slist_append(headers, "Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
    headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1" );
    headers = curl_slist_append(headers, "Cache-Control: max-age=0");
    headers = curl_slist_append(headers, "TE: Trailers");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    return mem.memory;
}
bool download_file(string url,string filename)
{
    ofstream file(filename,ios::out|ios::trunc);
    if(!file.is_open())
    {
        cerr << "Failed to open file:" << filename << endl;
        return false;
    }
    auto raw=simple_get(url);
    file << *raw;
    file.close();
    return true;
}
int main(int argc,char **argv)
{
    cout << "Bilibili JSON format CC subtitle downloader by 晴酱(alias Nathanli)" << endl;
    if(argc < 2 || argc > 4)
    {
        cerr << "Invaild argument" << endl;
        usage(argv[0]);
        return -1;
    }
    string inputfile=argv[1];
    string outputfile="default.json";
    if(inputfile=="-o")
    {
        if(argc !=4)
        {
            cerr << "Invaild argument" << endl;
            usage(argv[0]);
            return -1;
        }
        inputfile=argv[3];
        outputfile=argv[2];
    } else
        cerr << "Warning : You are not specified the output file name.Using the default name" << endl;
    auto html=simple_get(inputfile);
    regex e_subtitle(R"(subtitle\\[A-Za-z0-9]+\.json)",regex_constants::extended);
    regex e_lang(R"("lan":"[A-Za-z-]+")",regex_constants::extended);
    smatch match_subtitle,match_lang;
    string str_subtitle=*html,str_lang=*html;
    
    str_subtitle=regex_replace(str_subtitle,regex("\\\\u002F",regex_constants::extended),"\\");
    
    vector<string> urls;
    vector<string> langs;
    
    while (std::regex_search (str_subtitle,match_subtitle,e_subtitle)) {
        for (auto x=match_subtitle.begin();x!=match_subtitle.end();x++) {
            string url=x->str();
            url=regex_replace(url,regex("subtitle\\\\"),"");
            urls.push_back(url);
        }
        str_subtitle = match_subtitle.suffix().str();
    }
    while (std::regex_search (str_lang,match_lang,e_lang)) {
        for (auto x=match_lang.begin();x!=match_lang.end();x++) {
            string lang=x->str();
            lang=regex_replace(lang,regex(R"(lan|"|:)"),"");
            langs.push_back(lang);
        }
        str_lang = match_lang.suffix().str();
    }
    int size=min(langs.size(),urls.size());
    for(int i=0;i<size;i++)
    {
        string save_file=outputfile;
        if(save_file.substr(save_file.size()-4,4)!="json")
            save_file+="."+langs[i]+".json";
        else
            save_file=regex_replace(save_file,regex(R"(json$)"),langs[i]+".json");
        download_file("http://i0.hdslb.com/bfs/subtitle/"+urls[i],save_file);
        cout << "Saved " << langs[i] << " as " << save_file << endl;
    }

    curl_global_cleanup();
    return 0;
}