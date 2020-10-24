#include <list>
#include <string>
#include "AgendaService.hpp"
#include "Error.hpp"
#include "Log.hpp"

Log log_1;

bool funhhh(const Meeting &a){
    return true;
}
bool ffhhh(const User &a){ 
    return true;
}

/**
 * constructor
*/
AgendaService::AgendaService(){
    startAgenda();
}

  /**
   * destructor
   */
AgendaService::~AgendaService(){
    quitAgenda();
}

  /**
   * check if the username match password
   * @param userName the username want to login
   * @param password the password user enter
   * @return if success, true will be returned
   */
bool AgendaService::userLogIn(const std::string &userName, const std::string &password){
  std::list<User> userList=this->m_storage->queryUser(ffhhh);
  for(auto i=userList.begin();i!=userList.end();++i){
    if(i->getName()==userName){
      if(i->getPassword()==password)return true;
      else return false;
    }
  }
  return false;
}

  /**
   * regist a user
   * @param userName new user's username
   * @param password new user's password
   * @param email new user's email
   * @param phone new user's phone
   * @return if success, true will be returned
   */
bool AgendaService::userRegister(const std::string &userName, const std::string &password,
    const std::string &email, const std::string &phone){
    std::list<User> userList=this->m_storage->queryUser(ffhhh);
    for(auto i=userList.begin();i!=userList.end();++i){
      if(i->getName()==userName){
        return false;
      }
    }
    User u= User (userName,password,email, phone);
    this->m_storage->createUser(u);
    return true;
}

  /**
   * delete a user
   * @param userName user's username
   * @param password user's password
   * @return if success, true will be returned
   */
User standard_user;
bool fff(const User &a){
    if(a.getName()==standard_user.getName())return true;
    else return false;
}
bool AgendaService::deleteUser(const std::string &userName, const std::string &password){
  std::list<User> userList=this->m_storage->queryUser(ffhhh);
  for(auto i=userList.begin();i!=userList.end();++i){
    if(i->getName()==userName){
      if(i->getPassword()==password){
        deleteAllMeetings(userName);
        std::list<Meeting> meetings=listAllParticipateMeetings(userName);
        for(auto j=meetings.begin();j!=meetings.end();++j){
          quitMeeting(userName,j->getTitle());
        }
        standard_user.setName(userName);
        this->m_storage->deleteUser(fff);
        return true;
      }
    }
  }
  return false; 
}

  /**
   * list all users from storage
   * @return a user list result
   */
std::list<User> AgendaService::listAllUsers(void) const{
  return this->m_storage->queryUser(ffhhh);
}

std::list<Meeting> meetingsearch(AgendaService*a, const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate){
  std::list<Meeting> meetings;
  meetings=a->listAllMeetings(userName);
  Date start=Date::stringToDate(startDate);
  Date end=Date::stringToDate(endDate);
  for(auto i=meetings.begin();i!=meetings.end();){
    if(i->getEndDate()<=start||i->getStartDate()>=end){
      i=meetings.erase(i);
    }
    else ++i;
  }
  return meetings;
}

Meeting standard_meet;
bool funny(const Meeting &a){
    if(a.getTitle()==standard_meet.getTitle())return true;
    else return false;
}
  /**
   * create a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @param startData the meeting's start date
   * @param endData the meeting's end date
   * @return if success, true will be returned
   */
bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator){
    Error e;
    if(Date::stringToDate(endDate)<=Date::stringToDate(startDate)){
      e.setvalue(1);
      throw e;
      return false;
    }
    if(!Date::isValid(Date::stringToDate(startDate))){
      e.setvalue(2);
      throw e;
      return false;
    }
    if(!Date::isValid(Date::stringToDate(endDate))){
      e.setvalue(3);
      throw e;
      return false;
    }
    if(participator.size()==0){
      e.setvalue(4);
      throw e;
      return false;
    }
    for(int i=0;i<participator.size();i++){
      for(int j=i+1;j<participator.size();j++){
        if(participator[i]==participator[j]){
          e.setvalue(5);
          e.setname(participator[i]);
          throw e;
          return false;
        }
      }
      standard_user.setName(participator[i]);
      if(userName==participator[i]){
        e.setvalue(6);
        throw e;
        return false;
      }
      std::list<User> users=this->m_storage->queryUser(fff);
      int o=meetingsearch(this,participator[i],startDate,endDate).size();
      if(!users.size()){
        e.setvalue(7);
        e.setname(participator[i]);
        throw e;
        return false;
      }
      if(o){
        e.setvalue(8);
        e.setname(participator[i]);
        throw e;
        return false;
      }
    }
    standard_user.setName(userName);
    std::list<User> users=this->m_storage->queryUser(fff);
    if(!users.size()){
      e.setvalue(9);
      e.setname(userName);
      throw e;
      return false;
    }
    if(meetingsearch(this,userName,startDate,endDate).size()){
      e.setvalue(10);
      e.setname(userName);
      throw e;
      return false;
    }
    standard_meet.setTitle(title);
    std::list<Meeting> meeting_=this->m_storage->queryMeeting(funny);
    if(meeting_.size()){
      e.setvalue(11); 
      throw e;
      return false;
    }
    Meeting meet(userName,participator,Date::stringToDate(startDate),Date::stringToDate(endDate),title);
    this->m_storage->createMeeting(meet);
    return true;
}
std::string participator_to;
void swi(Meeting &a){
  a.addParticipator(participator_to);
}
  /**
   * add a participator to a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
bool AgendaService::addMeetingParticipator(const std::string &userName,const std::string &title,const std::string &participator){
  Error e;
  std::list<Meeting> meetings=this->m_storage->queryMeeting(funhhh);
  std::list<User> usersss=this->m_storage->queryUser(ffhhh);
  auto j=usersss.begin();
  for(;j!=usersss.end();){
    if(j->getName()!=participator){
      ++j;
    }
    else break;
  }
  if(j==usersss.end()){
    e.setvalue(12);
    e.setname(participator);
    throw e;
    return false;
  }
  if(userName==participator){
    e.setvalue(13);
    throw e;
    return false;
  }
  for(auto i=meetings.begin();i!=meetings.end();++i){
    if(i->getTitle()==title){
      if(i->getSponsor()==userName){
        if(meetingsearch(this,participator,Date::dateToString(i->getStartDate()),Date::dateToString(i->getEndDate())).size()){
          e.setvalue(10);
          e.setname(participator);
          throw e;
        }
        standard_meet.setTitle(title);
        participator_to=participator;
        this->m_storage->updateMeeting(funny,swi);
        std::string ss="add "+participator+" to "+title+",succeed";
        log_1.writelog(userName,ss);
        return true;
      }
    }
  }
  e.setvalue(14);
  throw e;
  return false;
}
int if_meeting_empty=0;
int if_par_exist=0;
void swi_delete(Meeting &a){
  for(int i=0;i<a.getParticipator().size();++i){
    if(a.getParticipator()[i]==participator_to){
      if_par_exist=0;
      a.removeParticipator(participator_to);
    }
  }
  if(a.getParticipator().size()==0)if_meeting_empty=1;
}
  /**
   * remove a participator from a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
bool AgendaService::removeMeetingParticipator(const std::string &userName,const std::string &title,const std::string &participator){
  Error e;
  if(userName==participator){
    e.setvalue(15);
    throw e;
    return false;
  }
  std::list<Meeting> meetings=this->m_storage->queryMeeting(funhhh);
  for(auto i=meetings.begin();i!=meetings.end();++i){
    if(i->getTitle()==title){
      if(i->getSponsor()==userName){
        standard_meet.setTitle(title);
        participator_to=participator;
        if_meeting_empty=0;
        if_par_exist=1;
        this->m_storage->updateMeeting(funny,swi_delete);
        if(if_par_exist){
          e.setvalue(16);
          e.setname(participator);
          throw e;
          return false;
        }
        if(if_meeting_empty)this->m_storage->deleteMeeting(funny);
        std::string s("remove ");
        s+=participator;
        s+=" from ";
        s+=title;
        s+=",succeed";
        log_1.writelog(userName,s);
        return true;
      }
    }
  }
  e.setvalue(14);
  throw e;
  return false;
}

  /**
   * quit from a meeting
   * @param userName the current userName. need to be the participator (a sponsor can not quit his/her meeting)
   * @param title the meeting's title
   * @return if success, true will be returned
   */
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title){
  Error ee;
  std::list<Meeting> meetings=this->m_storage->queryMeeting(funhhh);
  for(auto i=meetings.begin();i!=meetings.end();++i){
    if(i->getTitle()==title){
      if(i->getSponsor()==userName){
        ee.setvalue(17);
        throw ee;
        return false;
      }
      try{
        removeMeetingParticipator(i->getSponsor(),title,userName);
      }
      catch(Error e){
        ee.setvalue(18);
        throw ee;
        return false;
      }
      std::string s("request to quit meeting:  ");
      s+=title;
      s+=",succeed";
      log_1.writelog(userName,s);
      return true;
    }
  }
  ee.setvalue(18);
  throw ee;
  return false;
}

  /**
   * search a meeting by username and title
   * @param userName as a sponsor OR a participator
   * @param title the meeting's title
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &title) const{
  std::list<Meeting> meetings=listAllMeetings(userName);
  for(auto i=meetings.begin();i!=meetings.end();){
    if(i->getTitle()!=title)i=meetings.erase(i);
    else ++i;
  }
  return meetings;
}
  /**
   * search a meeting by username, time interval
   * @param userName as a sponsor OR a participator
   * @param startDate time interval's start date
   * @param endDate time interval's end date
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const{
  std::list<Meeting> meetings;
  if(Date::stringToDate(startDate)>Date::stringToDate(endDate))return meetings;
  Date start=Date::stringToDate(startDate);
  Date end=Date::stringToDate(endDate);
  if(!Date::isValid(end)||!Date::isValid(start))return meetings;
  meetings=listAllMeetings(userName);
  for(auto i=meetings.begin();i!=meetings.end();){
    if(i->getEndDate()<start||i->getStartDate()>end){
      i=meetings.erase(i);
    }
    else ++i;
  }
  return meetings;
}

  /**
   * list all meetings the user take part in
   * @param userName user's username
   * @return a meeting list result 
   */
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const{
  std::list<Meeting> meetings=this->m_storage->queryMeeting(funhhh);
  for(auto i=meetings.begin();i!=meetings.end();){
    if(i->getSponsor()!=userName){
      int exist=0;
      if(i->isParticipator(userName))exist=1;
      if(!exist)i=meetings.erase(i);
      else ++i;
    }
    else ++i;
  }
  return meetings;
}

  /**
   * list all meetings the user sponsor
   * @param userName user's username
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const{
  std::list<Meeting> meetings=listAllMeetings(userName);
  for(auto i=meetings.begin();i!=meetings.end();){
    if(i->getSponsor()!=userName)i=meetings.erase(i);
    else ++i;
  }
  return meetings;
}

  /**
   * list all meetings the user take part in and sponsor by other
   * @param userName user's username
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::listAllParticipateMeetings(
      const std::string &userName) const{
  std::list<Meeting> meetings=listAllMeetings(userName);
  for(auto i=meetings.begin();i!=meetings.end();){
    if(i->getSponsor()==userName)i=meetings.erase(i);
    else ++i;
  }
  return meetings;
}

bool funfun(const Meeting& a){
  if(a.getSponsor()==standard_meet.getSponsor()&&a.getTitle()==standard_meet.getTitle())return true;
  else return false;
}

  /**
   * delete a meeting by title and its sponsor
   * @param userName sponsor's username
   * @param title meeting's title
   * @return if success, true will be returned
   */
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title){
  Error e;
  standard_meet.setTitle(title);
  standard_meet.setSponsor(userName);
  if(this->m_storage->deleteMeeting(funfun)==0){
    e.setvalue(14);
    throw e;
    return false;
  }
  return true;
}
bool funfuning(const Meeting& a){
  if(a.getSponsor()==standard_meet.getSponsor())return true;
  else return false;
}
  /**
   * delete all meetings by sponsor
   * @param userName sponsor's username
   * @return if success, true will be returned
   */
bool AgendaService::deleteAllMeetings(const std::string &userName){
  standard_meet.setSponsor(userName);
  return this->m_storage->deleteMeeting(funfuning);
}

  /**
   * start Agenda service and connect to storage
   */
void AgendaService::startAgenda(void){
  this->m_storage=Storage::getInstance();
}

  /**
   * quit Agenda service
   */
void AgendaService::quitAgenda(void){
  this->m_storage=nullptr;
}

