#include "Program.h"
#include <vector>

using namespace std;
vector<Program> records();
bool proginchannel(string channel, string day, int hour, int minutes);
string retchannelstring(Program p);
Program::Program(string name, unsigned int duration, string day, unsigned int hour, unsigned int minutes) :exhibitionDate(day, hour, minutes){
	this->name = name;
	this->duration = duration;
	this->exhibitionDate = Date(day, hour, minutes);
	this->type = "";
	this->recorded = false;
}
vector<string> recprogchannels();
bool equalprogram(Program one, Program two){
	if ((one.retname() == two.retname())){
		return true;
	}
	else{
		return false;
	}
}
bool Program::ifprogramisrec(){
	Program prog = Program(name, duration, exhibitionDate.getday(), exhibitionDate.gethour(), exhibitionDate.getminutes());
	vector<Program> p = records();
	for (int i = 0; i < p.size(); i++){
		if (equalprogram(prog, p[i])){
			return true;
		}
	}
	return false;
	}

void Program::printproginfo(){
	vector<string> teste;
	string text;
	string channel;
	ofstream myfile;
	int a = 1;
	clrscr();
	cout << "\n\n\t[ESC] to go back\n\n\t" << retname() << "\n\n\tType: " << type << "\n\n\tDuration: " << duration << "\n\n\tExhibiton Date: " << exhibitionDate.tostring() << endl << endl;
	string end = endofprogram().tostring();
	cout << "\tEnd Date: " << end << "\n\n";
	cout << "\tRecords: ";
	Program pp = retprogram();
	Program p;
	if (pp.ifprogramisrec()){
		if (checkifonair() == "past"){
			cout << "Recorded\n";
			cout << "\tPress any key to go back...\n";
			getch();
		}
		else if (checkifonair() == "live"){
			cout << "Recording\n";
			cout << "\tPress any key to go back...\n";
			getch();
		}
		else if ((checkifonair() == "future")){
			cout << "Will be recorded\n";
			cout << "\tPress any key to go back...\n";
			getch();
		}
	}
	else{
		if (!(checkifonair() == "past" || checkifonair() == "live")){
			cout << "Do you want to record this program? Y: for yes/N: for no\n\n";
			while (a != 'y' && a != 'n' && a != 27){ //ESC=27 Y=121 N=110
				a = _getch();
				if (a == 121){
					cout << "\n\nWriting to Records.txt";
					ifstream file;
					file.open("records.txt");
					while (getline(file, text)){
						teste.push_back(text);
					}
					file.close();
					p = Program(name, duration, exhibitionDate.getday(), exhibitionDate.gethour(), exhibitionDate.getminutes());
					channel = retchannelstring(p);
					teste.push_back(channel);
					text = exhibitionDate.getday();
					teste.push_back(text);
					text = std::to_string(exhibitionDate.gethour());
					teste.push_back(text);
					text = std::to_string(exhibitionDate.getminutes());
					teste.push_back(text);
					myfile.open("records.txt");
					for (int i = 0; i < teste.size(); i++){
						myfile << teste[i] << endl;
					}
					myfile.close();
				}
			}
		}
		else{
			cout << "Not recorded " << endl;
			cout << "\tPress any key to go back...\n";

			getch();
		}
	}

}


