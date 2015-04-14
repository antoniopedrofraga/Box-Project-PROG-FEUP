#ifndef _CHANNEL
 #define _CHANNEL

#include<iostream>
#include<vector>
#include<string>
#include<istream>
#include "Program.h"

using namespace std;
vector<Program> bublesortprog(vector<Program> vec);
class Channel{
	string name;
	vector<Program> programs;
public:
	vector<Program> retPrograms();
	void setChannel(Program p);
	void setProgram(vector<Program> p);
	Channel(){

	}
	Channel(string name);
	void pushvectorprog(Program a){
		programs.push_back(a);
	}
	void setvectorprog(std::istream& file);
	//print name of channel with endline
	string retname(){
		return name;
	}
	
	string retnameconst() const{
		return name;
	}
	vector<Program> retvecprog(string day) const{
		vector<Program> vec;
		int size = programs.size();
		for (int i = 0; i < size; i++){
			if (programs[i].retday() == day){
				vec.push_back(programs[i]);
			}
		}
		return vec;
	}
	vector<Program> Channel::retProgramsconst() const{
		return this->programs;
	}
	vector<Program> retvecprogchan(string day, const vector<Program> p) const{
		vector<Program> vec;
		int size = p.size();
		for (int i = 0; i < size; i++){
			if (p[i].retday() == day){
				vec.push_back(programs[i]);
			}
		}
		int s = vec.size();
		for (int pos = s - 1; pos > 0; pos--){ //Buble sort para ordenar de maneira aquele que comece mais cedo fique em cima
			for (int i = pos - 1; i >= 0; i--){
				if (vec[pos].rettempo() < vec[i].rettempo()){
					Program p = vec[pos];
					vec[pos] = vec[i];
					vec[i] = p;
				}
			}
		}
		return vec;
	}
	void printprog(){
		int size = programs.size();
		for (int i = 0; i < size; i++){
			cout << "\t" << i + 1 << "." << programs[i].retname() << endl;
		}
	}
	int vecprogsize(){
		return programs.size();
	}
	bool checkday(string day){
		int size = programs.size();
		for (int i = 0; i < size; i++){
			if (programs[i].retday() == day){
				return true;
			}
		}
		return false;
	}
	void pushbackprogbtype(vector<Program> &p, string type){
		int size = programs.size();
		for (int i = 0; i < size; i++){
			if (programs[i].rettype() == type){
				p.push_back(programs[i]);
			}
		}
}
	void pushbacktypetoday(vector<string> &typetoday, string day){
		int size = programs.size();
		for (int i = 0; i < size; i++){
			if (programs[i].retday() == day){
				typetoday.push_back(programs[i].rettype());
			}
		}
	}
	void pushbackprogsoftypetoday(vector<Program> &p,string type, string day){
		int size = programs.size();
		for (int i = 0; i < size; i++){
			if ((programs[i].rettype() == type) && (programs[i].retday() == day)){
				p.push_back(programs[i]);
			}
		}
	
};

#endif

