#include <string>
#include "Date.hpp"

/**
* @brief default constructor
*/
Date::Date(){
    this->m_day=0;
    this->m_hour=0;
    this->m_minute=0;
    this->m_month=0;
    this->m_year=0;
}

/**
* @brief constructor with arguments
*/
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute){
    this->m_day=t_day;
    this->m_hour=t_hour;
    this->m_minute=t_minute;
    this->m_month=t_month;
    this->m_year=t_year;
}

/**
* @brief constructor with a string
*/
Date::Date(const std::string &dateString){
    *this=stringToDate(dateString);
}
/**
* @brief return the year of a Date
* @return   a integer indicate the year of a date
*/
int Date::getYear(void) const{
    return this->m_year;
}

/**
* @brief set the year of a date
* @param a integer indicate the new year of a date
*/
void Date::setYear(const int t_year){
    this->m_year=t_year;
}

/**
* @brief return the month of a Date
* @return   a integer indicate the month of a date
*/
int Date::getMonth(void) const{
    return this->m_month;
}

/**
* @brief set the month of a date
* @param a integer indicate the new month of a date
*/
void Date::setMonth(const int t_month){
    this->m_month=t_month;
}

/**
* @brief return the day of a Date
* @return   a integer indicate the day of a date
*/
int Date::getDay(void) const{
    return this->m_day;
}

/**
* @brief set the day of a date
* @param a integer indicate the new day of a date
*/
void Date::setDay(const int t_day){
    this->m_day=t_day;
}

/**
* @brief return the hour of a Date
* @return   a integer indicate the hour of a date
*/
int Date::getHour(void) const{
    return this->m_hour;
}

/**
* @brief set the hour of a date
* @param a integer indicate the new hour of a date
*/
void Date::setHour(const int t_hour){
    this->m_hour=t_hour;
}

/**
* @brief return the minute of a Date
* @return   a integer indicate the minute of a date
*/
int Date::getMinute(void) const{
    return this->m_minute;
}

/**
* @brief set the minute of a date
* @param a integer indicate the new minute of a date
*/
void Date::setMinute(const int t_minute){
    this->m_minute=t_minute;
}

/**
*   @brief check whether the date is valid or not
*   @return the bool indicate valid or not
*/
bool Date::isValid(const Date &t_date){
    if(t_date.m_year<1000||t_date.m_year>9999)return false;
    if(t_date.m_month<1||t_date.m_month>12||t_date.m_day<1)return false;
    if(t_date.m_month==2){
        if(t_date.m_year%400==0||(t_date.m_year%100&&t_date.m_year%4==0)){
            if(t_date.m_day>29)return false;
        }
        else{
            if(t_date.m_day>28)return false;
        }
    }
    else if((t_date.m_month<8&&t_date.m_month%2)||(t_date.m_month>7&&t_date.m_month%2==0)){
        if(t_date.m_day>31)return false;
    }
    else{
        if(t_date.m_day>30)return false;
    }
    if(t_date.m_hour<0||t_date.m_hour>23)return false;
    if(t_date.m_minute<0||t_date.m_minute>59)return false;
    return true;
}

/**
* @brief convert a string to date, if the format is not correct return
* 0000-00-00/00:00
* @return a date
*/
Date Date::stringToDate(const std::string &t_dateString){
    Date D;
    int k=0;
    if(t_dateString.size()==16){
        if(t_dateString[0]>='0'&&t_dateString[0]<='9')k++;
        if(t_dateString[1]>='0'&&t_dateString[1]<='9')k++;
        if(t_dateString[2]>='0'&&t_dateString[2]<='9')k++;
        if(t_dateString[3]>='0'&&t_dateString[3]<='9')k++;
        if(t_dateString[5]>='0'&&t_dateString[5]<='9')k++;
        if(t_dateString[6]>='0'&&t_dateString[6]<='9')k++;
        if(t_dateString[8]>='0'&&t_dateString[8]<='9')k++;
        if(t_dateString[9]>='0'&&t_dateString[9]<='9')k++;
        if(t_dateString[11]>='0'&&t_dateString[11]<='9')k++;
        if(t_dateString[12]>='0'&&t_dateString[12]<='9')k++;
        if(t_dateString[14]>='0'&&t_dateString[14]<='9')k++;
        if(t_dateString[15]>='0'&&t_dateString[15]<='9')k++;
        if(t_dateString[4]=='-')k++;
        if(t_dateString[7]=='-')k++;
        if(t_dateString[10]=='/')k++;
        if(t_dateString[13]==':')k++;
    }
    if(k<16){
        D.setDay(0);
        D.setHour(0);
        D.setMinute(0);
        D.setMonth(0);
        D.setYear(0);
        return D;
    }
    D.setYear((t_dateString[0]-'0')*1000+(t_dateString[1]-'0')*100+(t_dateString[2]-'0')*10+(t_dateString[3]-'0'));
    D.setMonth((t_dateString[5]-'0')*10+(t_dateString[6]-'0'));
    D.setDay((t_dateString[8]-'0')*10+(t_dateString[9]-'0'));
    D.setHour((t_dateString[11]-'0')*10+(t_dateString[12]-'0'));
    D.setMinute((t_dateString[14]-'0')*10+(t_dateString[15]-'0'));
    return D;
}

/**
* @brief convert a date to string, if the date is invalid return
* 0000-00-00/00:00
*/
std::string Date::dateToString(const Date &t_date){
    std::string date;
    if(!isValid(t_date)){
        date="0000-00-00/00:00";
        return date;
    }
    date+=t_date.m_year/1000+'0';
    date+=t_date.m_year%1000/100+'0';
    date+=t_date.m_year%100/10+'0';
    date+=t_date.m_year%10+'0';
    date+='-';
    if(t_date.m_month<10)date+='0';
    else date+=t_date.m_month/10+'0';
    date+=t_date.m_month%10+'0';
    date+='-';
    if(t_date.m_day<10)date+='0';
    else date+=t_date.m_day/10+'0';
    date+=t_date.m_day%10+'0';
    date+='/';
    if(t_date.m_hour<10)date+='0';
    else date+=t_date.m_hour/10+'0';
    date+=t_date.m_hour%10+'0';
    date+=':';
    if(t_date.m_minute<10)date+='0';
    else date+=t_date.m_minute/10+'0';
    date+=t_date.m_minute%10+'0';
    return date;
}

/**
*  @brief overload the assign operator
*/
Date &Date::operator=(const Date &t_date){
    this->m_day=t_date.m_day;
    this->m_minute=t_date.m_minute;
    this->m_month=t_date.m_month;
    this->m_year=t_date.m_year;
    this->m_hour=t_date.m_hour;
    return *this;

}

  /**
  * @brief check whether the CurrentDate is equal to the t_date
  */
bool Date::operator==(const Date &t_date) const{
    if(this->m_day==t_date.m_day&&this->m_year==t_date.m_year&&this->m_month==t_date.m_month){
        if(this->m_hour==t_date.m_hour&&this->m_minute==t_date.m_minute)return true;
    }
    return false;
}

  /**
  * @brief check whether the CurrentDate is  greater than the t_date
  */
bool Date::operator>(const Date &t_date) const{
    if(this->m_year>t_date.m_year)return true;
    else if(this->m_year==t_date.m_year){
        if(this->m_month>t_date.m_month)return true;
        else if(this->m_month==t_date.m_month){
            if(this->m_day>t_date.m_day)return true;
            else if(this->m_day==t_date.m_day){
                if(this->m_hour>t_date.m_hour)return true;
                else if(this->m_hour==t_date.m_hour){
                    if(this->m_minute>t_date.m_minute)return true;
                }
            }
        }
    }
    return false;
}

/**
* @brief check whether the CurrentDate is  less than the t_date
*/
bool Date::operator<(const Date &t_date) const{
    if(!(*this==t_date)&&!(*this>t_date))return true;
    else return false;
}

  /**
  * @brief check whether the CurrentDate is  greater or equal than the t_date
  */
bool Date::operator>=(const Date &t_date) const{
    if((*this==t_date)||(*this>t_date))return true;
    else return false;
}

/**
* @brief check whether the CurrentDate is  less than or equal to the t_date
*/
bool Date::operator<=(const Date &t_date) const{
    if(!(*this>t_date))return true;
    return false;
}
