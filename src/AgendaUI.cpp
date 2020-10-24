#include "AgendaUI.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include "Error.hpp"
#include "Log.hpp"
using namespace std;

Log log;

AgendaUI::AgendaUI(){
    startAgenda();
}
void AgendaUI::OperationLoop(void){
    string operation;
    do{
        cout<<"-------------------------------------Agenda-------------------------------------\n"
            <<"Action :\n"
            <<"l    - log in Agenda by user name and password\n"
            <<"r    - register an Agenda account\n"
            <<"q    - quit Agenda\n"
            <<"--------------------------------------------------------------------------------\n"
            <<endl<<"Agenda :~$ ";
        cin>>operation;
        if(operation=="l")userLogIn();
        else if(operation=="r")userRegister();
        else if(operation=="q")quitAgenda();
    }while(operation!="q");
}

    /**
     * constructor
     */
void AgendaUI::startAgenda(void){
    std::string s("start");
    s+=",succeed";
    log.writelog("Agenda",s);
    this->m_agendaService.startAgenda();
}

    /**
     * catch user's operation
     * @return the operation
     */
std::string AgendaUI::getOperation(){
    string operation;
    cout<<"-------------------------------------Agenda-------------------------------------\n"
        <<"Action :\n"
        <<"o    - log out Agenda\n"
        <<"dc   - delete Agenda account\n"
        <<"lu   - list all Agenda user\n"
        <<"cm   - create a meeting\n"
        <<"amp  - add meeting participator\n"
        <<"rmp  - remove meeting participator\n"
        <<"rqm  - request to quit meeting\n"
        <<"la   - list all meetings\n"
        <<"las  - list all sponsor meetings\n"
        <<"lap  - list all participator meetings\n"
        <<"qm   - query meeting by title\n"
        <<"qt   - query meeting by time interval\n"
        <<"dm   - delete meeting by title\n"
        <<"da   - delete all meetings\n"
        <<"--------------------------------------------------------------------------------\n"
        <<endl<<"Agenda@"<<this->m_userName<<" :~# ";
    cin>>operation;
    return operation;
}

    /**
     * execute the operation
     * @return if the operationloop continue
     */
bool AgendaUI::executeOperation(std::string t_operation){
    if(t_operation=="o"){
        string operation="log out, succeed";
        log.writelog(this->m_userName,operation);
        userLogOut();
        return false;
    }
    else if(t_operation=="dc"){
        log.writelog(this->m_userName,"delete Agenda account,succeed");
        deleteUser();
        return false;
    }
    else if(t_operation=="lu"){
        log.writelog(this->m_userName,"list all Agenda user,succeed");
        listAllUsers();
    }
    else if(t_operation=="cm")createMeeting();
    else if(t_operation=="amp"){
        cout<<endl<<"[add participator] [meeting title] [participator username]\n"
            <<"[add participator] ";
        string title,participator;
        cin>>title>>participator;
        try{
            this->m_agendaService.addMeetingParticipator(this->m_userName,title,participator);
        }
        catch(Error e){
            std::string ss="add "+participator+" to "+title+",fail";
            log.writelog(this->m_userName,ss);
            e.search();
            cout<<"[add participator] error!\n";
            return true;
        }
        cout<<"[add participator] succeed!\n";
    }
    else if(t_operation=="rmp"){
        cout<<endl<<"[remove participator] [meeting title] [participator username]\n"
            <<"[remove participator] ";
        string title,participator;
        cin>>title>>participator;
        try{
            this->m_agendaService.removeMeetingParticipator(this->m_userName,title,participator);
        }
        catch(Error e){
            std::string s("remove ");
            s+=participator;
            s+=" from ";
            s+=title;
            s+=",fail";
            log.writelog(this->m_userName,s);
            e.search();
            cout<<"[remove participator] error!\n";
            return true;
        }
        cout<<"[remove participator] succeed!\n";
    }
    else if(t_operation=="rqm"){
        cout<<endl<<"[quit meeting] [meeting title]\n"
            <<"[quit meeting] ";
        string title;
        cin>>title;
        try{
            this->m_agendaService.quitMeeting(this->m_userName,title);
        }
        catch(Error e){
            std::string s("request to quit meeting:  ");
            s+=title;
            s+=",fail";
            log.writelog(this->m_userName,s);
            e.search();
            cout<<"[quit meeting] error!\n";
            return true;
        }
        cout<<"[quit meeting] succeed!\n";
    }
    else if(t_operation=="la"){
        std::string s("list all meetings,succeed");
        log.writelog(this->m_userName,s);
        listAllMeetings();
    }
    else if(t_operation=="las")listAllSponsorMeetings();
    else if(t_operation=="lap")listAllParticipateMeetings();
    else if(t_operation=="qm")queryMeetingByTitle();
    else if(t_operation=="qt")queryMeetingByTimeInterval();
    else if(t_operation=="dm")deleteMeetingByTitle();
    else if(t_operation=="da")deleteAllMeetings();
    return true;
}

    /**
     * user Login
     */
void AgendaUI::userLogIn(void){
    string username,password;
    cout<<"\n[log in] [username] [password]\n"
        <<"[log in] ";
    cin>>username>>password;
    if(this->m_agendaService.userLogIn(username,password)){
        this->m_userName=username;
        this->m_userPassword=password;
        cout<<"[log in] succeed!\n";
        log.writelog(this->m_userName,"log in,succeed");
        while(executeOperation(getOperation()));
    }
    else{
        cout<<"[log in] Password error or user doesn't exist\n";
    }
}

    /**
     * user regist
     */
void AgendaUI::userRegister(void){
    string username,password,email,phone;
    cout<<"\n[register] [user name] [password] [email] [phone]\n"
        <<"[register] ";
    cin>>username>>password>>email>>phone;
    if(this->m_agendaService.userRegister(username,password,email,phone)){
        cout<<"[register] succeed!\n";
        log.writelog(this->m_userName,"register,succeed");
    }
    else cout<<"[register] This username has been registered\n";
}

    /**
     * user logout
     */
void AgendaUI::userLogOut(void){
    this->m_userName.clear();
    this->m_userPassword.clear();
    cout<<endl<<endl;
}

    /**
     * quit the Agenda
     */
void AgendaUI::quitAgenda(void){
    std::string s("quit,succeed");
    log.writelog("Agenda",s);
    this->m_agendaService.quitAgenda();
}

    /**
     * delete a user from storage
     */
void AgendaUI::deleteUser(void){
    this->m_agendaService.deleteUser(this->m_userName,this->m_userPassword);
    cout<<endl<<"[delete agenda account] succeed!\n";
    userLogOut();
}

    /**
     * list all users from storage
     */
void AgendaUI::listAllUsers(void){
    list<User> users=this->m_agendaService.listAllUsers();
    cout<<endl<<"[list all users]"<<endl<<endl
        <<setiosflags(ios::left)
        <<setw(27)<<"name"<<setw(27)<<"email"<<setw(27)<<"phone"<<endl;
    for(auto i=users.begin();i!=users.end();++i){
        cout<<setiosflags(ios::left)
            <<setw(27)<<i->getName()<<setw(27)<<i->getEmail()<<setw(27)<<i->getPhone()<<endl;
    }
}

    /**
     * user create a meeting with someone else
     */
void AgendaUI::createMeeting(void){
    cout<<endl<<"[create meeting] [the number of participators]"
        <<endl<<"[create meeting] ";
    int num;
    cin>>num;
    vector<string> participators;
    for(int i=1;i<=num;i++){
        string participator;
        cout<<"[create meeting] [please enter the participator "<<i<<" ]"
            <<endl<<"[create meeting] ";
        cin>>participator;
        participators.push_back(participator);
    }
    cout<<"[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"
        <<endl<<"[create meeting] ";
    string title,start,end;
    cin>>title>>start>>end;
    try{
        this->m_agendaService.createMeeting(this->m_userName,title,start,end,participators);
    }
    catch(Error e){
        std::string s("create a meeting:");
        s+=title;
        s+=",fail";
        log.writelog(this->m_userName,s);
        e.search();
        cout<<"[create meeting] error!\n";
        return ;
    }
    string s("create a meeting:");
    s+=title;
    cout<<"[create meeting] succeed!\n";
}

    /**
     * list all meetings from storage
     */
void AgendaUI::listAllMeetings(void){
    cout<<endl<<"[list all meetings]"<<endl<<endl;
    printMeetings(this->m_agendaService.listAllMeetings(this->m_userName));
}

    /**
     * list all meetings that this user sponsored
     */
void AgendaUI::listAllSponsorMeetings(void){
    std::string s("list all sponser meetings,succeed");
    log.writelog(this->m_userName,s);
    cout<<endl<<"[list all sponsor meetings]"<<endl<<endl;
    printMeetings(this->m_agendaService.listAllSponsorMeetings(this->m_userName));
}

    /**
     * list all meetings that this user take part in
     */
void AgendaUI::listAllParticipateMeetings(void){
    std::string s("list all participator meetings,succeed");
    log.writelog(this->m_userName,s);
    cout<<endl<<"[list all participator meetings]"<<endl<<endl;
    printMeetings(this->m_agendaService.listAllParticipateMeetings(this->m_userName));
}

    /**
     * search meetings by title from storage
     */
void AgendaUI::queryMeetingByTitle(void){
    cout<<endl<<"[query meeting] [title]"<<endl<<"[query meeting] ";
    string title;
    cin>>title;
    cout<<endl;
    std::string s("query meeting by title: ");
    s+=title;
    s+=",succeed";
    log.writelog(this->m_userName,s);
    printMeetings(this->m_agendaService.meetingQuery(this->m_userName,title));
}

    /**
     * search meetings by timeinterval from storage
     */
void AgendaUI::queryMeetingByTimeInterval(void){
    cout<<endl<<"[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"<<endl<<"[query meeting] ";
    string start,end;
    cin>>start>>end;
    cout<<endl;
    std::string s("query meeting by time interval: ");
    s+=start;
    s+='--';
    s+=end;
    s+=",succeed";
    log.writelog(this->m_userName,s);
    printMeetings(this->m_agendaService.meetingQuery(this->m_userName,start,end));
}

    /**
     * delete meetings by title from storage
     */
void AgendaUI::deleteMeetingByTitle(void){
    cout<<endl<<"[delete meeting] [title]\n"<<"[delete meeting] ";
    string title;
    cin>>title;
    try{
        this->m_agendaService.deleteMeeting(this->m_userName,title);
    }
    catch(Error e){
        std::string s("delete meeting by title: ");
        s+=title;
        s+=",fail";
        e.search();
        cout<<"[error] delete meeting fail\n";
        return ;
    }
    std::string s("delete meeting by title: ");
    s+=title;
    s+=",succeed";
    log.writelog(this->m_userName,s);
    cout<<"[delete meeting] succeed!\n";
}

    /**
     * delete all meetings that this user sponsored
     */
void AgendaUI::deleteAllMeetings(void){
    this->m_agendaService.deleteAllMeetings(this->m_userName);
    std::string s("delete all meetings,succeed");
    log.writelog(this->m_userName,s);
    cout<<endl<<"[delete all meeting] succeed!\n";
}

    /**
     * show the meetings in the screen
     */
void AgendaUI::printMeetings(std::list<Meeting> t_meetings){
    cout<<setiosflags(ios::left)
        <<setw(15)<<"title"<<setw(15)<<"sponsor"<<setw(20)<<"start time"<<setw(20)<<"end time"<<"participators"<<endl;
    for(auto i=t_meetings.begin();i!=t_meetings.end();++i){
        cout<<setiosflags(ios::left)
        <<setw(15)<<i->getTitle()<<setw(15)<<i->getSponsor()<<setw(20)<<Date::dateToString(i->getStartDate())<<setw(20)<<Date::dateToString(i->getEndDate());
        for(int j=0;j<i->getParticipator().size();j++){
            if(j)cout<<",";
            cout<<i->getParticipator()[j];
        }
        cout<<endl;
    }
}

AgendaUI ui;

void AgendaUI::fc(int sig){
    if(sig == SIGINT){
        ui.quitAgenda();
        exit(0);
    }
}
int main(){
    signal(SIGINT,AgendaUI::fc);
    ui.OperationLoop();
    return 0;
}
