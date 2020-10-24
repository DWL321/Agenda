#include "Log.hpp"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
using namespace std;
void Log::writelog(std::string username,std::string operation){
    ofstream file;
    string filename=rotate();
    file.open(filename,ios::app);
    if(file.is_open()){
        time_t curtime;
        time(&curtime);
        string s(ctime(&curtime));
        s.pop_back();
        file<<'"'<<s<<"\",\""<<username<<"\",\""<<operation<<'"'<<endl;
    }
}
string Log::rotate(){
    string filename;
    char x='1';
    while(1){
        filename="tmp/log";
        filename+=x;
        filename+=".csv";
        x++;
        fstream file;
        file.open(filename,ios::ate);
        if(!file)break;
        else{
            file.close();
            struct stat statbuf;
            char file_name[]={'t','m','p','/','l','o','g',x,'.','c','s','v'};
            stat(file_name,&statbuf);
            int size=statbuf.st_size;
            if(size<=100000)break;
        }
    }
    return filename;
}