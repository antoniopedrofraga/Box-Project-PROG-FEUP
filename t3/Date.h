#ifndef _DATE
 #define _DATE

#include <iostream>
#include <string>

using namespace std;


class Date{
  string weekDay;
  unsigned int hour;
  unsigned int minutes;
 public:
	 Date(){

	 };
  Date(string day, unsigned int hour, unsigned int minutes);
  string getday() const{
	  return weekDay;
  };
 
  int gethour();
  int getminutes();
  string tostring();
  
};


#endif

