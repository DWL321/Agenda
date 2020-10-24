#include <iostream>
#include <time.h>
#include <fstream>
#include <string>

class Log{
    public:
        void writelog(std::string username,std::string operation);
        std::string rotate();
};