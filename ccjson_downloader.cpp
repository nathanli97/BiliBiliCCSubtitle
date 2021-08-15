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

#include "ccjson_downloader.h"
#include "json/json.h"
#include "common.h"
#include "ccjson_convert.h"
using namespace std;

int do_download_json(string const & inputfile, string outputdir, int p_start,int p_end,bool auto_convert)
{
    smatch match;
    auto html=CURLHelper::do_simple_get(inputfile);
    auto part=*html;
    auto part_pid=inputfile;
    string part_bvid;
    string part_aid;
    string outputfile;
    bool has_pid=false;
    Json::Value playlist,subtitlelist;
    Json::Reader reader;

    int ipid;

    //Matching pid
    if(std::regex_search (part_pid,match,regex(R"(p=\d+)")))
    {
        part_pid=match.begin()->str();
        if(std::regex_search (part_pid,match,regex(R"(\d+)")))
        {
            part_pid=match.begin()->str();
            has_pid=true;
        }
    }
    if(part_pid==inputfile)
        part_pid="1";

    std::string part_playlist_url;
    bool is_ntl = false;
    std::string ntl_name;
    //Matching aid and bvid
    if(std::regex_search (part,match,regex(R"(__INITIAL_STATE__=\{"aid":\d+,"bvid":"[A-Za-z0-9]+")")))
    {
        part=match.begin()->str();
        if(std::regex_search (part,match,regex(R"(__INITIAL_STATE__=\{"aid":\d+,)")))
        {
            part_aid=match.begin()->str();
            if(std::regex_search (part_aid,match,regex(R"(\d+)")))
            {
                part_aid=match.begin()->str();
            }
        }

        if(std::regex_search (part,match,regex(R"("bvid":"[A-Za-z0-9]+)")))
        {
            part_bvid=match.begin()->str();
            part_bvid=regex_replace(part_bvid,regex(R"("bvid":")",regex_constants::extended),"");
        }

        part_playlist_url = "https://api.bilibili.com/x/player/pagelist?bvid="+part_bvid+"&jsonp=jsonp";
    }else
    {
        //Maybe this is the global-version BiliBili
        //play/1010919/10446796
        if(std::regex_search (part,match,regex(R"(play/\d+/\d+)")))
        {
            part=match.begin()->str();
            if(std::regex_search (part,match,regex(R"(\d+$)")))
            {
                part=match.begin()->str();
                part_playlist_url = "https://app.global.bilibili.com/intl/gateway/v2/app/subtitle?ep_id=" + part;
                ntl_name = "GLOBAL" + part;
                is_ntl = true;
            }
        }

    }
    if(part_playlist_url.empty())
    {
        return -1;
    }
    //Getting playlist
    auto part_playlist=CURLHelper::do_simple_get(part_playlist_url);

    if(!reader.parse(*part_playlist,playlist))
    {
        cerr << "Failed to parse json document when parsing playlist!" << endl;
        return -1;
    }
    if(stoi(part_pid)-1 >= playlist["data"].size() || stoi(part_pid) < 0)
    {
        cerr << "ERR:PID " << part_pid << " not found." << endl;
        return -1;
    }

    if(!is_ntl)
    {
        ipid=stoi(part_pid);
        if(p_start!=0 && p_end==0)
            p_end=playlist["data"].size();
        if(p_start<0 || p_start-1 >=playlist["data"].size())
            p_start=1;
        if(p_end<0 || p_end-1 >=playlist["data"].size())
            p_end=1;

        if(has_pid)
        {
            p_start=ipid;
            p_end=ipid;
        }
    }else
    {
        p_start = p_end = -1;
    }
    for(int pid=p_start;pid<=p_end;pid++)
    {
        Json::Value part_cid;
        std::string subtitle_info;
        if(!is_ntl)
        {
            part_cid=playlist["data"][pid-1]["cid"];
            if(!part_cid.isInt())
            {
                cerr << "Cannot get CID" << endl;
                return -1;
            }
            subtitle_info=*CURLHelper::do_simple_get("https://api.bilibili.com/x/player.so?id=cid:"+to_string(part_cid.asInt())+"&aid="+part_aid+"&bvid="+part_bvid);
        }
        else
        {
            subtitle_info=*part_playlist;
        }
        if(std::regex_search (subtitle_info,match,regex(R"(<subtitle>.*</subtitle>)")))
        {
            subtitle_info=match.begin()->str();
#ifdef __WIN32
            subtitle_info=Utf8ToGbk(subtitle_info.c_str());
#endif
            subtitle_info=regex_replace(subtitle_info,regex(R"(^<subtitle>)",regex_constants::extended),"");
            subtitle_info=regex_replace(subtitle_info,regex(R"(</subtitle>$)",regex_constants::extended),"");
        }
        if(!reader.parse(subtitle_info,subtitlelist))
        {
            cerr << "Failed to parse json document when parsing information of subtitle!" << endl;
            return -1;
        }
        if (is_ntl)
        {
            subtitlelist = subtitlelist["data"];
        }
        auto subtitles_root=subtitlelist["subtitles"];
        if(subtitles_root.size()==0)
        {
            cerr << "No CC-subtitles was found in P" << pid << ", ignoring" << endl;
            continue;
        }

        for(auto i:subtitles_root)
        {
            if(outputdir.empty())
            {
                if(part_bvid.empty() && is_ntl)
                {
                    part_bvid = ntl_name;
                }
                outputdir = "downloads/" + part_bvid + "/";
                if(file_exist(outputdir) != 0)
                    _mkdir(outputdir.c_str());
            }
            if(is_ntl)
            {
                auto url = i["url"].asString();
                auto lang = i["key"].asString();
                auto lang_local = "";
                outputfile = ntl_name + "-" + lang +".json";
                outputfile = outputdir + outputfile ;
#ifdef __WIN32
                lang_local=Utf8ToGbk(lang_local);
#endif
                CURLHelper::download_file(i["url"].asString(),outputfile);
                cout << "Found: " << lang << " " << lang_local << " " << " ==> " << outputfile << endl;
                if(auto_convert)
                    do_convert(outputfile,outputfile);
            }
            else
            {
                outputfile = "AV" + part_aid + "(" + part_bvid + ")-P" + to_string(pid)+ "-" + i["lan"].asString() + ".json";
                outputfile = outputdir + outputfile;
                CURLHelper::download_file(string("http:")+i["subtitle_url"].asString(),outputfile);
                cout << "Found: " << i["lan"].asString()  << " " << " ==> " << outputfile << endl;
                if(auto_convert)
                    do_convert(outputfile,outputfile);
            }
        }
    }
    return 0;
}