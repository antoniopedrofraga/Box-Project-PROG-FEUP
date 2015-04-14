#ifndef _PROGRAM
 #define _PROGRAM

#include<iostream>
#include<vector>
#include <fstream>
#include "Date.h"
#include <istream>
#include <conio.h>
#include <string>
Date getdata();
void clrscr();
using namespace std;
class Program{
  string name;
  string type;    // introduced in the final version of the work text
  bool recorded;  // introduced in the final version of the work text
  int duration;   // in minutes
  Date exhibitionDate; 
  
 public:
	 Program(){

	 };
  Program(string name, unsigned int duration, string day, unsigned int hour, unsigned int minutes);

  string retname(){
	  string iflive;
	  if (checkifonair() == "live"){
		  iflive = name + " [LIVE] ";
		  return iflive;
	  }
	  else{
		  return name;
	  }
  }
  string rettype(){
	  return type;
  }
  string retday() const{
	 return exhibitionDate.getday();
  }
  int rethour(){
	  return exhibitionDate.gethour();
  }
  int retminutes(){
	  return exhibitionDate.getminutes();
  }
  int retduration(){
	  return duration;
  }
  void setrecord(){
	  if (checkifonair() == "past"){
		  recorded = true;
	  }
  }

  bool dateinprog(Date p){
	  if (((exhibitionDate.getday()== p.getday())) && ((exhibitionDate.gethour() == p.gethour())) && ((exhibitionDate.getminutes() == p.getminutes()))){
		  return true;
	  }
	  else{
		  return false;
	  }
  }

  Program retprogram(){
	  return Program(name, duration, retday(), rethour(), retminutes());
  }
  bool ifprogramisrec();
  void printproginfo();
  int rettempo(){
	  int tempo;
	  tempo = exhibitionDate.gethour() * 60 + exhibitionDate.getminutes();
	  return tempo;
  }
  //get info from file
  Program Program::getproginfo(std::istream& file,string name) {
	  string text;
	  getline(file, text);
	  string type = text;
	  getline(file, text);
	  unsigned int duracao = atoi(text.c_str());
	  getline(file, text);
	  string dia = text;
	  getline(file, text);
	  unsigned int hora = atoi(text.c_str());
	  getline(file, text);
	  unsigned int minutos = atoi(text.c_str());
	  Program p = Program(name, duracao, dia, hora, minutos);
	  p.type = type;
	  return p;
  }
  int convertdaytonumber(string day){
	  if (day == "Sunday"){
		  return 1;
	  }
	  else if (day == "Monday"){
		  return 2;
	  }
	  else if (day == "Tuesday"){
		  return 3;
	  }
	  else if (day == "Wendnesday"){
		  return 4;
	  }
	  else if (day == "Thursday"){
		  return 5;
	  }
	  else if (day == "Friday"){
		  return 6;
	  }
	  else if (day == "Saturday"){
		  return 7;
	  }
  }
  string convertnumbertoday(int day){
	  if (day == 1 || day==8){
		  return "Sunday";
	  }
	  else if (day == 2){
		  return "Monday";
	  }
	  else if (day == 3){
		  return "Tuesday";

	  }
	  else if (day == 4){
		  return "Wendnesday";
	  }
	  else if (day == 5){
		  return "Thursday";
	  }
	  else if (day == 6){
		  return "Friday";
	  }
	  else if (day == 7){
		  return "Saturday";
	  }
  }

  Date endofprogram(){
	  Date programdate = exhibitionDate;
	  string programdays = programdate.getday();
	  int programday = convertdaytonumber(programdays);
	  int programhour = programdate.gethour();
	  int programminutes = programdate.getminutes();
	  vector<int> programtime;// composto por [dia (de 1 a 7), minutos no dia (hora*60+minutos)]
	  int minutespassed = 60 * programhour + programminutes;
	  int limit = 60 * 24;
	  int minutespassedinend = minutespassed + duration;
	  if (minutespassedinend >= limit){
		  programday++;
		  minutespassedinend = minutespassedinend - limit;
	  }
	  
	  string newday = convertnumbertoday(programday);
	  int newhour = minutespassedinend / 60;
	  float test = minutespassedinend % 60;
	  int newminutes = minutespassedinend % 60;
	  return Date(newday, newhour, newminutes);
  }
  string checkifonair(){
	  //do meu programa
	  Date programdate = exhibitionDate;
	  string programdays = programdate.getday();
	  int programday = convertdaytonumber(programdays);
	  int programhour = programdate.gethour();
	  int programminutes = programdate.getminutes();
	  //data do fim do meu programa
	  int minutespassed = 60 * programhour + programminutes;
	  int limit = 60 * 24;
	  int minutespassedinend = minutespassed + duration;
	  if (minutespassedinend >= limit){
		  programday++;
		  minutespassedinend = minutespassedinend - limit;
	  }
	  //data atual
	  string days = getdata().getday();
	  int day = convertdaytonumber(days);
	  int hour = getdata().gethour();
	  int minutes = getdata().getminutes();
	  int minutesnow = 60 * hour + minutes;
	  if (programday > day){
		  return "future";
	  }
	  else if (programday < day){
		  return "past";
	  }
	  else if (programday == day){
		  if (minutesnow>minutespassedinend){
			  return "past";
		  }
		  else if (minutesnow < minutespassed){
			  return "future";
		  }
		  else{
			  return "live";
		  }
	  }

  }

 
};


#endif

