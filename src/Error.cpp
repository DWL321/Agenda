#include "Error.hpp"
#include <string>
#include <iostream>
using namespace std;
Error::Error(){
    this->value=0;
}
void Error::setvalue(int v){
    this->value=v;
}
void Error::setname(std::string n){
    this->name=n;
}
int Error::getvalue(){
    return this->value;
}
std::string Error::getname(){
    return this->name;
}
void Error::search(){
    switch (this->value)
    {
    case 1:cout<<"The end time is not latter than the start time!\n";
        break;
    case 2:cout<<"The start time is invalid!\n";
        break;
    case 3:cout<<"The end time is invalid!\n";
        break;
    case 4:cout<<"There is no participator!\n";
        break;
    case 5:cout<<"There are repeated participators: "<<this->name<<" !\n";
        break;
    case 6:cout<<"The sponser can not be participator!\n";
        break;
    case 7:cout<<"No user called "<<this->name<<" !\n";
        break;
    case 8:cout<<this->name<<" is busy at that time!\n";
        break;
    case 9:cout<<"No user called "<<this->name<<" !\n";
        break;
    case 10:cout<<this->name<<" is busy at that time!\n";
        break;
    case 11:cout<<"The title has existed!\n";
        break;
    case 12:cout<<"No user called "<<this->name<<" !\n";
        break;
    case 13:cout<<"The sponser can not be participator!\n";
        break;
    case 14:cout<<"You are not the sponser of the meeting!\n";
        break;
    case 15:cout<<"The sponser can not remove himself!\n";
        break;
    case 16:cout<<this->name<<" does not take apart in this meeting!\n";
        break;
    case 17:cout<<"The sponser can not quit meeting!\n";
        break;
    case 18:cout<<"You do not take apart in this meeting!\n";
        break;
    default:
        break;
    }
}