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
    }

    //Getting playlist
    auto part_playlist=CURLHelper::do_simple_get("https://api.bilibili.com/x/player/pagelist?bvid="+part_bvid+"&jsonp=jsonp");

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
    for(int pid=p_start;pid<=p_end;pid++)
    {
        auto part_cid=playlist["data"][pid-1]["cid"];
        if(!part_cid.isInt())
        {
            cerr << "Cannot get CID" << endl;
            return -1;
        }
        auto subtitle_info=*CURLHelper::do_simple_get("https://api.bilibili.com/x/player.so?id=cid:"+to_string(part_cid.asInt())+"&aid="+part_aid+"&bvid="+part_bvid);

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
                outputdir = "downloads/" + part_bvid + "/";
                if(file_exist(outputdir) != 0)
                    _mkdir(outputdir.c_str());
            }
            outputfile = "AV" + part_aid + "(" + part_bvid + ")-P" + to_string(pid)+ "-" + i["lan"].asString() + ".json";
            outputfile = outputdir + outputfile;
            CURLHelper::download_file(string("http:")+i["subtitle_url"].asString(),outputfile);
            cout << "Found: " << i["lan"].asString() << " " << i["lan_doc"].asString() << " " << " ==> " << outputfile << endl;
            if(auto_convert)
                do_convert(outputfile,outputfile);
        }
    }
    return 0;
}