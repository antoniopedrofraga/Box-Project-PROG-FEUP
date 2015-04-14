#include "Date.h"

using namespace std;
string inttostring(int n){
	int a = n;
	string numerostring;
	while (n != 0)
	{
		int i = n % 10;
		n = n / 10;
		i = i + '0';
		numerostring = static_cast<char>(i)+numerostring;
	}
	if (a < 10){
		numerostring = "0" + numerostring;
	}
	return numerostring;
}

Date::Date(string day, unsigned int hour, unsigned int minutes){
  // fill in with correct code ....
	this->weekDay = day;
	this->hour = hour;
	this->minutes = minutes;
}


int Date::gethour(){
	return hour;
}
int Date::getminutes(){
	return minutes;
}


string Date::tostring(){
	string date;
	date = weekDay + " " + inttostring(hour) + ":" + inttostring(minutes);
	return date;
}
