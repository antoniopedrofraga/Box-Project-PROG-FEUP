#ifndef _BOX
 #define _BOX

#include<iostream>
#include<vector>
#include<string>
#include "Date.h"
#include "Channel.h"
#include "Program.h"
#include "Movie.h"

 using namespace std;

vector<Program> bublesortprog(vector<Program> vec);
bool typeisonvec(vector<string> p, string type);
bool repeated(string a, vector<string> p,int position);
void firstscreen();

class Box{
private:
	string password;
	Date currentDate;
	vector<Channel> channels;
	vector<Movie> movieClub;
	vector<Movie> seenMovies;
	vector<Channel> recordList;  // stores recorded programs and programs yet to be recorded
	//  vector<Program> recorded; // removed in the final version of the work text


public:
	Box(string passwd, Date currentDate);

	string channelname(int a){
		return channels[a].retname();
	}

	//devolve um vector de programas nesse dia
	vector<Program> listByDay(string day) const{
		vector<Program> v;
		vector<Program> test;
		int size = channels.size();
		for (int i = 0; i < size; i++){
			test = channels[i].retvecprog(day);
			int sizetest = test.size();
			for (int i = 0; i < sizetest; i++){
				v.push_back(test[i]);
			}
		}

		return bublesortprog(v);
	};


	vector<Program> listByChannel(string channel, string day) const{ //devolve um vector de programas de um canal que vao para o ar nesse dia
		vector<Program> todayinchannel;
		int size = todayinchannel.size();
		int sizechan = channels.size();
		for (int i = 0; i < sizechan; i++){
			if (channels[i].retnameconst() == channel){
				todayinchannel = channels[i].retvecprogchan(day, channels[i].retProgramsconst());
			}
		}
		return todayinchannel;
	};
	vector<Program> listByType(string  type, string day) const{

	};
	vector<string> existingTypes(){
		vector<string> p;
		vector<Program> prog;
		int size = channels.size();
		for (int i = 0; i < size; i++){
			prog = channels[i].retPrograms();
			int sizeprog = prog.size();
			for (int a = 0; a < sizeprog; a++){
				if (!typeisonvec(p, prog[a].rettype())){
					p.push_back(prog[a].rettype());
				}
			}
		}
		return p;
	}
	bool rentMovies(string title);
	float moneySpent() const;
	int timesWhatched(string title) const;
	bool changePassword(string password){
		this->password = password;
		return true;
	};          // ask, verify and change the password
	// Channel CRUD
	bool createdChanel();
	bool removeChanel();
	bool updateChanel();
	// Program CRUD
	bool createdProgram(string chanel);
	bool removeProgram();
	bool updateProgram();
	// Movie CRUD
	bool createdMovie();
	bool removeMovie();
	bool updateMovie();
	//return date
	Date todate();
	//getchannel
	void vecchan();
	bool checkifchannelinprog(string name){
		int size = recordList.size();
		for (int i = 0; i < size; i++){
			if (recordList[i].retname() == name){
				return true;
			}
		}
		return false;
	}
	int positioninprog(string name){
		int size = recordList.size();
		for (int i = 0; i < size; i++){
			if (recordList[i].retname() == name){
				return i;
			}
		}
	}

	Program getprogram(string channel, string day, int hour, int minutes){
		int size = channels.size();
		vector<Program> p;
		for (int i = 0; i < size; i++){
			if (channels[i].retname() == channel){
				p = channels[i].retPrograms();
				int sizeprog = p.size();
				for (int a = 0; a < sizeprog; a++){
					if ((p[a].retday() == day) && (p[a].rethour() == hour) && (p[a].retminutes() == minutes)){
						return p[a];
					}
				}
			}
		}
	}
	void setrecordlist(){
		ifstream file;
		file.open("records.txt");
		string text;
		string channel;
		string day;
		int hour;
		int minutes;
		int position;
		Program p;
		Channel c;
		int size;
		while (getline(file, text)){
			channel = text;
			getline(file, text);
			day = text;
			getline(file, text);
			hour = atoi(text.c_str());
			getline(file, text);
			minutes = atoi(text.c_str());
			p = getprogram(channel, day, hour, minutes);
			if (checkifchannelinprog(channel)){
				position = positioninprog(channel);
				recordList[position].pushvectorprog(p);
			}
			else{
				size = recordList.size();
				c = Channel(channel);
				recordList.push_back(c);
				recordList[size].pushvectorprog(p);
			}

		}
	}
	int vecchansize(){
		return channels.size();
	}
	//print canais
	void printallchan(){
		int size = channels.size();
		for (int i = 0; i < size; i++){
			cout << "\t" << i + 1 << ".";
			cout << channels[i].retname();
			cout << endl;
		}
	}
	void printchan(int p){
		cout << channels[p].retname();
	}
	void printprog(int channel){
		channels[channel].printprog();
	}

	//return Channel of program
	string retchannel(Program p){
		int chansize = channels.size();
		for (int i = 0; i < chansize; i++){
			vector<Program> vecp = channels[i].retPrograms();
			int size = vecp.size();
			for (int a = 0; a < size; a++){
				if ((vecp[a].retname() == p.retname())){
					return channels[i].retname();
				}
			}
		}
	}
	//return position of vector channel
	void DispProgInfoWithChanAndProg(int chan, int prog){
		vector<Program> p = channels[chan].retPrograms();
		p[prog].printproginfo();
	};
	int vecprogsize(int i){
		return channels[i].vecprogsize();
	}
	vector<Channel> retchanonday(string day){
		vector<Channel> p;
		int size = channels.size();
		for (int i = 0; i < size; i++){
			if (channels[i].checkday(day)){
				p.push_back(channels[i]);
			}
		}
		return p;
	}
	vector<Program> retprogfromtype(string type){
		vector<Program> p;
		int size = channels.size();
		for (int i = 0; i < size; i++){
			channels[i].pushbackprogbtype(p, type);
		}
		return p;
	}
	vector<string> typestoday(){
		vector<string> typtoday;
		string day = currentDate.getday();
		int size = channels.size();
		for (int i = 0; i < size; i++){
			channels[i].pushbacktypetoday(typtoday, day);
		}
		int sizeprog = typtoday.size();
		vector<string> newone;
		for (int i = 0; i < sizeprog; i++){
			if (!repeated(typtoday[i], typtoday, i)){
				newone.push_back(typtoday[i]);
			}
		}
		return newone;
	}
	vector<Program> progsoftypetoday(string type){
		string day = currentDate.getday();
		vector<Program> p;
		vector<Program> test;
		int size = channels.size();
		for (int i = 0; i < size; i++){
			if (channels[i].checkday(day)){
				channels[i].pushbackprogsoftypetoday(p, type, day);
			}
		}
		return p;
	}
	Program retprogondate(string channel, Date date){
		vector<Program> p;
		int size = channels.size();
		for (int i = 0; i < size; i++){
			if (channel == channels[i].retname()){
				p = channels[i].retPrograms();
				int sizeprog = p.size();
				for (int a = 0; a < sizeprog; a++){
					if (p[a].dateinprog(date)){
						return p[a];
					}
				}
			}
		}
		Program prog;
		return prog;
	}
	bool dateinchannel(string channel, string day, int hour, int minutes){
		vector<Program> p;
		int size = channels.size();
		int sizeprog;
		for (int i = 0; i < size; i++){
			if (channels[i].retname() == channel){
				p = channels[i].retPrograms();
				sizeprog = p.size();
				for (int a = 0; a < sizeprog; a++){
					if ((p[a].retday() == day) && (p[a].rethour() == hour) && (p[a].retminutes() == minutes)){
						return true;
					}
				}
			}
		}
		return false;
	}
	vector<Program> getrecordsforchannels(){
		ifstream file;
		file.open("records.txt");
		string text;
		Program p;
		vector<Program> vecret;
		while (getline(file, text)){
			string channel = text;
			getline(file, text);
			string day = text;
			getline(file, text);
			int hour = atoi(text.c_str());
			getline(file, text);
			int minutes = atoi(text.c_str());
			Date data = Date(day, hour, minutes);
			p = retprogondate(channel, data);
			if (p.retname() != ""){
				vecret.push_back(p);
			}
		}
		return vecret;
	}
	vector<Program> verifygetrecords(){
		ifstream file;
		file.open("records.txt");
		string text;
		Program p;
		vector<Program> vecret;
		while (getline(file, text)){
			string channel = text;
			getline(file, text);
			string day = text;
			getline(file, text);
			int hour = atoi(text.c_str());
			getline(file, text);
			int minutes = atoi(text.c_str());
			Date data = Date(day, hour, minutes);
			p = retprogondate(channel, data);
			if (p.retname() != ""){
				vecret.push_back(p);
			}
		}
		return vecret;
	}
	void pushbackchannel(string name){
		Channel c = Channel(name);
		channels.push_back(c);
	}
	vector<Program> getrecords(){
		ifstream file;
		file.open("records.txt");
		string text;
		Program p;
		vector<Program> vecret;
		while (getline(file, text)){
			string channel = text;
			getline(file, text);
			string day = text;
			getline(file, text);
			int hour = atoi(text.c_str());
			getline(file, text);
			int minutes = atoi(text.c_str());
			Date data = Date(day, hour, minutes);
			p = retprogondate(channel, data);
			if (p.retname() != ""){
				vecret.push_back(p);
			}
		}
		return vecret;
	}


	//RECORDS
	void getSeenMovies(){
		vector<Movie> p;
		seenMovies = p;
		ifstream myfile;
		string text;
		myfile.open("moviesseen.txt");
		string movie;
		float price;
		unsigned times;
		while (getline(myfile, text)){
			movie = text;
			getline(myfile, text);
			price = ::atof(text.c_str());
			getline(myfile, text);
			times = atoi(text.c_str());
			Movie m = Movie(movie, price, times);
			seenMovies.push_back(m);
		}

		myfile.close();
	}

	void getMovieClub(){
		ifstream myfile;
		string text;
		string movie;
		float price;
		myfile.open("MovieClub.txt");
		while (getline(myfile, text)){
			movie = text;
			getline(myfile, text);
			price = atoi(text.c_str());
			Movie m = Movie(movie, price, 0);
			movieClub.push_back(m);
		}
		myfile.close();
	}

	void printMoviesClub(){
		clrscr();
		firstscreen();
		int size = movieClub.size();
		if (size == 0){
			cout << "\n\nDoesn't have Movies\n\n";
			system("pause");
		}
		else{
			for (int i = 0; i < size; i++){
				cout << i + 1 << ". " << movieClub.at(i).getName() << " " << movieClub.at(i).getPrice() << endl;
			}
			system("pause");
		}
	}

	void printSeenMovies(){
		clrscr();
		firstscreen();
		cout << "\n\n\tMovies Seen\tSpent: " << moneySpent() << "\n\n";
		vector<Movie> m = seenMovies;
		int size = seenMovies.size();
		if (size == 0){
			cout << "\n\nDoesn't have rented movies\n\n";
			system("pause");
		}
		else{
			for (int i = 0; i < size; i++){
				cout << "\t" << i + 1 << "." << seenMovies.at(i).getName() << "\t Price:" << seenMovies.at(i).getPrice() << "\t Times Rented: " << seenMovies.at(i).timesRented() << endl;
			}
			cout << endl << endl << "\t";
			system("pause");
		}
	}

	float moneySpent(){
		float money = 0;
		for (int i = 0; i < seenMovies.size(); i++){
			money = seenMovies.at(i).timesRented()*seenMovies.at(i).getPrice() + money;
		}
		return money;
	}
	vector<Movie> retmovieclub(){
		return movieClub;
	}
	vector<Movie> retseenmovies(){
		return seenMovies;
	}
	void updateseenmovies(int e){
		seenMovies[e].rented();
	}
	void pushbackmovieclub(Movie m){
		movieClub.push_back(m);
		movieClub;
	}
	void pushbackmovie(Movie m){
		seenMovies.push_back(m);

	}
	void saveseenmovies(){
		ofstream file;
		file.open("moviesseen.txt");
		int size = seenMovies.size();
		for (int i = 0; i < size; i++){
			file << seenMovies[i].getName() << endl;
			file << seenMovies[i].getPrice() << endl;
			file << seenMovies[i].timesRented() << endl;
		}
		file.close();
	}
	void removemoviefromclub(int a){
		movieClub.erase(movieClub.begin() + a);
	}


	bool checkifinmovieclub(Movie m){
		int size = movieClub.size();
		for (int i = 0; i < size; i++){
			if (movieClub[i].getName() == m.getName()){
				return true;
			}
		}
		return false;
	}
	int updatemovieadmin(int position){
		int choice = 10;
		float cost;
		string name;
		while (choice<0 || choice>2){
			clrscr();
			cout << "[0] to go back\n" << movieClub[position].getName();
			cout << endl << endl;
			cout << "1.Change Name\n2.Change Cost\n\n";
			cin >> choice;
			cin.ignore();
			if (choice == 0){
				return 0;
			}
			if (choice == 1){
				clrscr();
				cout << "Set new name: ";
				getline(cin, name);
				movieClub[position].setName(name);
				return 0;
			}
			else if (choice == 2){
				clrscr();
				cout << "The cost is " << movieClub[position].getPrice() << ".\tSet new cost: ";
				cin >> cost;
				cin.ignore();
				movieClub[position].setCost(cost);
				return 0;
			}
		}
	}
	void savechannels(){
		int size = channels.size();
		int sizeprog;
		vector<Program> p;
		ofstream file;
		file.open("channels.txt");
		for (int i = 0; i < size; i++){
			file << channels[i].retname();
			sizeprog = channels[i].retPrograms().size();
			p = channels[i].retPrograms();
			for (int a = 0; a < sizeprog; a++){
				file << endl << p[i].retname();
				file << endl << p[i].retduration();
				file << endl << p[i].retday();
				file << endl << p[i].rethour();
				file << endl << p[i].retminutes();
			}
		}
		file.close();
	}
};


#endif



