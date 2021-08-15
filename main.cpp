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
#include "ccjson.h"
#include "common.h"
enum jmode_t{
    undefined,
    convert,
    download,
    down_convert
};
using namespace std;
void print_usage(string program){
    cout << "Usage: " << program << " MODE [-o outputfile] inputfile/URL" << endl;
    cout << "MODE: -d download json subtitle" << endl;
    cout << "      -c convert json subtitle to srt format" << endl;
    cout << "      -h show this usage message" << endl;
    cout << "      -s specify start PID (Using this when you want to download multi-videos)" << endl;
    cout << "      -e specify end PID (Using this when you want to download multi-videos)" << endl;
    cout << "      -D specify the output directory, By the default, Program will create a directory called downloads/<BVID>" << endl;
    cout << "P.S.  -o option can not use in download mode!" << endl;
}
int main(int argc,char **argv)
{
    cout << "Bilibili JSON format CC subtitle downloader Ver 1.1.3 by Nathanli97" << endl;
    cout << "BUILD at " << __DATE__ << " " << __TIME__ << endl;
    prepare_env();
    enum jmode_t mode=undefined;
    int i,p_start=0,p_end=0;
    string buf;
    string input_file;
    string output_file;
    string output_dir;

    for(i=1;i<argc;i++)
    {
        buf=argv[i];
        if(buf=="-d")
        {
            if(mode!=undefined)
                mode=down_convert;
            else
                mode=download;
        }
        else if(buf=="-c")
        {
            if(mode!=undefined)
                mode=down_convert;
            else
                mode=convert;
        }
        else if(buf=="-o")
        {
            if(i+1<argc && argv[i+1][0]!='-')
                output_file=argv[++i];
            else{
                cerr << "-o requires a output file path" << endl;
                exit(-1);
            }
        }
        else if(buf=="-h") {
            print_usage(argv[0]);
            exit(0);
        }
        else if(buf=="-s")
        {
            p_start=stoi(argv[++i]);
        }
        else if(buf=="-e")
        {
            p_end=stoi(argv[++i]);
        }else if(buf=="-D")
        {
            output_dir = argv[++i];
        }
        else if(i==argc-1) {
            input_file = argv[i];
        }
        else{
            cerr << "Unknown option:" << buf << endl;
            exit(-1);
        }
    }
    if(!output_dir.empty())
    {
        if(file_exist(output_dir) != 0)
        {
            cerr << "Specified output directory does not existed: " << output_dir << endl;
            print_usage(argv[0]);
            exit(-1);
        }
    }
    if(mode==undefined)
    {
        cerr << "-d,-c are required" << endl;
        print_usage(argv[0]);
        exit(-1);
    }
    if(input_file.empty())
    {
        cerr << "inputfile option are required" << endl;
        exit(-1);
    }
    if(mode==download)
    {
        if(!output_file.empty())
            cerr << "WARN:-o option can not use in download mode! ignoring" << endl;
        do_download_json(input_file,output_dir,p_start,p_end);
    }else if(mode==convert)
    {
        if(output_file.empty())
            output_file=input_file;
        do_convert(input_file,output_file);
    }else if(mode==down_convert)
    {
        if(!output_file.empty())
            cerr << "WARN:-o option can not use in download mode! ignoring" << endl;
        do_download_json(input_file,output_dir,p_start,p_end,true);
    }
    CURLHelper::do_curl_cleanup();
    return 0;
}