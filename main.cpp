//
// Created by taboo on 2020/7/8.
//
#include "ccjson.h"
bool verbose=false;
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
    cout << "      -s specify start PID" << endl;
    cout << "      -e spdcify end PID" << endl;
    cout << "P.S.  -o option can not use in download mode!" << endl;
}
int main(int argc,char **argv)
{
    cout << "Bilibili JSON format CC subtitle downloader Ver 1.1.0 by Nathanli97" << endl;
    enum jmode_t mode=undefined;
    int i,p_start=0,p_end=0;
    string buf;
    string input_file;
    string output_file;

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
        }else if(i==argc-1) {
            input_file = argv[i];
        }else{
            cerr << "Unknown option:" << buf << endl;
            exit(-1);
        }
    }

    if(mode==undefined)
    {
        cerr << "-d,-c are required" << endl;
        print_usage(argv[0]);
        exit(-1);
    }
    if(input_file=="")
    {
        cerr << "inputfile option are required" << endl;
        exit(-1);
    }
    if(mode==download)
    {
        if(output_file!="")
            cerr << "WARN:-o option can not use in download mode! ignoring" << endl;
        do_download_json(input_file,p_start,p_end);
    }else if(mode==convert)
    {
        if(output_file=="")
            output_file=input_file;
        do_convert(input_file,output_file);
    }else if(mode==down_convert)
    {
        if(output_file!="")
            cerr << "WARN:-o option can not use in download mode! ignoring" << endl;
        do_download_json(input_file,p_start,p_end,true);
    }
    do_curl_cleanup();
    return 0;
}