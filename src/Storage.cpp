#include <functional> 
#include <list>
#include <memory>
#include <string>
#include "Storage.hpp" 
#include "Path.hpp"
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "Log.hpp"

Log log_;

Storage::Storage(){
    this->m_dirty=false;
    this->m_meetingList.clear();
    this->m_userList.clear();
    readFromFile();
}
/**
*   read file content into memory
*   @return if success, true will be returned
*/
bool Storage::readFromFile(void){
    std::string str;
    std::ifstream fpath;
    std::ifstream fpath_u(Path::userPath);
    fpath.open(Path::meetingPath);
    if(!fpath.is_open()||!fpath_u.is_open())return false;
    while(getline(fpath,str)){
        if(str.size()==0)continue;
        int count=1;
        int len=str.size();
        std::vector<int>pos;
        for(count =0;count<len;count++){
            if(str[count]=='"')pos.push_back(count);
        }
        Meeting m;
        std::string spo;
        count=pos[0]+1;
        while(count<pos[1]){
            spo+=str[count++];
        }
        m.setSponsor(spo);
        count=pos[2]+1;
        while(count<pos[3]){
            std::string name;
            while(count<pos[3]&&str[count]!='&'){
                name+=str[count++];
            }
            m.addParticipator(name);
            count++;
        }
        count=pos[4]+1;
        std::string ds;
        while(count<pos[5]){
            ds+=str[count++];
        }
        m.setStartDate(m.getStartDate().stringToDate(ds));
        count=pos[6]+1;
        std::string de;
        while(count<pos[7]){
            de+=str[count++];
        }
        m.setEndDate(m.getEndDate().stringToDate(de));
        count=pos[8]+1;
        std::string title;
        while(count<pos[9]){
            title+=str[count++];
        }
        m.setTitle(title);
        this->m_meetingList.push_back(m);
    }
    std::string s("read from meetings.csv");
    s+=",succeed";
    log_.writelog("Agenda",s);
    fpath.close();
    while(getline(fpath_u,str)){
        User u;
        int count=1;
        std::string name;
        if(str.size()==0)continue;
        int len=str.size();
        std::vector<int>pos;
        for(int i=0;i<len;i++){
            if(str[i]=='"')pos.push_back(i);
        }
        count=pos[0]+1;
        while(count<pos[1]){
            name+=str[count++];
        }
        u.setName(name);
        count=pos[2]+1;
        std::string password;
        while(count<pos[3]){
            password+=str[count++];
        }
        u.setPassword(password);
        count=pos[4]+1;
        std::string email;
        while(count<pos[5]){
            email+=str[count++];
        }
        u.setEmail(email);
        count=pos[6]+1;
        std::string phone;
        while(count<pos[7]){
            phone+=str[count++];
        }
        u.setPhone(phone);
        this->m_userList.push_back(u);
    }
    s="read from users.csv";
    s+=",succeed";
    log_.writelog("Agenda",s);
    fpath_u.close();
    return true;
}

/**
*   write file content from memory
*   @return if success, true will be returned
*/
bool Storage::writeToFile(void){
    std::ofstream fpath;
    fpath.open(Path::meetingPath);
    std::ofstream fpath_u(Path::userPath);
    if(!fpath.is_open()||!fpath_u.is_open())return false;
    std::list<Meeting>::iterator j;
    for(j =this->m_meetingList.begin();j!=this->m_meetingList.end();++j){
        std::vector<std::string> ss=j->getParticipator();
        fpath<<'"';
        fpath<<j->getSponsor();
        fpath<<"\",\"";
        if(ss.size())fpath<<ss[0];
        for(int k=1;k<j->getParticipator().size();k++){
            fpath<<'&';
            fpath<<ss[k];
        }
        fpath<<"\",\"";
        fpath<<j->getStartDate().dateToString(j->getStartDate());
        fpath<<"\",\"";
        fpath<<j->getEndDate().dateToString(j->getEndDate());
        fpath<<"\",\"";
        fpath<<j->getTitle();
        fpath<<"\"\n";
    }
    std::string s_("write to meetings.csv");
    s_+=",succeed";
    log_.writelog("Agenda",s_);
    fpath.close();
    std::list<User>::iterator i;
    for(i =this->m_userList.begin();i!=this->m_userList.end();++i){
        fpath_u<<'"';
        fpath_u<<i->getName();
        fpath_u<<"\",\"";
        fpath_u<<i->getPassword();
        fpath_u<<"\",\"";
        fpath_u<<i->getEmail();
        fpath_u<<"\",\"";
        fpath_u<<i->getPhone();
        fpath_u<<"\"\n";
    }
    s_="write to users.csv";
    s_+=",succeed";
    log_.writelog("Agenda",s_);
    fpath_u.close();
    return true;
}

/**
* get Instance of storage
* @return the pointer of the instance
*/
std::shared_ptr<Storage> Storage::getInstance(void){
    if(m_instance==nullptr) m_instance=std::shared_ptr<Storage>(new Storage());
    return m_instance;
}

  /**
  *   destructor
  */
Storage::~Storage(){
    sync();
}

// CRUD for User & Meeting
// using C++11 Function Template and Lambda Expressions

/**
* create a user
* @param a user object
*/
void Storage::createUser(const User &t_user){
    this->m_userList.push_back(t_user);
    this->m_dirty=true;
}

/**
* query users
* @param a lambda function as the filter
* @return a list of fitted users
*/
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const{
    std::list<User> l;
    std::list<User>::const_iterator i;
    for(i =this->m_userList.begin();i!=this->m_userList.end();++i){
        bool ret=filter(*i);
        if(ret)l.push_back(*i);
    }
    return l;
}

/**
* update users
* @param a lambda function as the filter
* @param a lambda function as the method to update the user
* @return the number of updated users
*/
int Storage::updateUser(std::function<bool(const User &)> filter,
std::function<void(User &)> switcher){
    int count=0;
    std::list<User>::iterator i;
    for(i =this->m_userList.begin();i!=this->m_userList.end();++i){
        bool ret=filter(*i);
        if(ret){
            count++;
            switcher(*i);
        }
    }
    if(count>0)this->m_dirty=true;
    return count;
}

/**
* delete users
* @param a lambda function as the filter
* @return the number of deleted users
*/
int Storage::deleteUser(std::function<bool(const User &)> filter){
    int count=0;
    for(auto i =this->m_userList.begin();i!=this->m_userList.end();){
        bool ret=filter(*i);
        if(ret){
            i=this->m_userList.erase(i);
            count++;
        }
        else ++i;
    }
    if(count>0)this->m_dirty=true;
    return count;
}

/**
* create a meeting
* @param a meeting object
*/
void Storage::createMeeting(const Meeting &t_meeting){
    this->m_meetingList.push_back(t_meeting);
    std::string s("create a meeting:");
    s+=t_meeting.getTitle();
    s+=",succeed";
    log_.writelog(t_meeting.getSponsor(),s);
    this->m_dirty=true;
}

/**
* query meetings
* @param a lambda function as the filter
* @return a list of fitted meetings
*/
std::list<Meeting> Storage::queryMeeting(
    std::function<bool(const Meeting &)> filter) const{
    std::list<Meeting> l;
    std::list<Meeting>::const_iterator i;
    for(i=this->m_meetingList.begin();i!=this->m_meetingList.end();++i){
        bool ret=filter(*i);
        if(ret)l.push_back(*i);
    }
    return l;
}

/**
* update meetings
* @param a lambda function as the filter
* @param a lambda function as the method to update the meeting
* @return the number of updated meetings
*/
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
    std::function<void(Meeting &)> switcher){
    int count=0;
    std::list<Meeting>::iterator i;
    for(i=this->m_meetingList.begin();i!=this->m_meetingList.end();++i){
        bool ret=filter(*i);
        if(ret){
            switcher(*i);
            count++;
        }
    }   
    if(count>0)this->m_dirty=true;
    return count;
}

/**
* delete meetings
* @param a lambda function as the filter
* @return the number of deleted meetings
*/
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter){
    int count=0;
    for(auto i=this->m_meetingList.begin();i!=this->m_meetingList.end();){
        bool ret=filter(*i);
        if(ret){
          i=this->m_meetingList.erase(i);
          count++;
        }
        else ++i;
    } 
    if(count>0)this->m_dirty=true;
    return count;
}

/**
* sync with the file
*/
bool Storage::sync(void){
    this->m_dirty=false;
    return writeToFile();
}
std::shared_ptr<Storage> Storage::m_instance=nullptr;

