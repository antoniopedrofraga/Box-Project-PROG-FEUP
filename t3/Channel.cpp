#include "Channel.h"
#include "Program.h"
#include "Box.h"
#include <fstream>
void clrscr();

char firstchar(std::istream& file, string &info){
	getline(file, info);
	return info[0];
}
Channel::Channel(string name){ 
	this->name = name;

}

vector<Program> Channel::retPrograms(){
	return this->programs;
}

void Channel::setChannel(Program p){
	this->programs.push_back(p);
}
void Channel::setProgram(vector<Program> p){
	this->programs = p;
}

