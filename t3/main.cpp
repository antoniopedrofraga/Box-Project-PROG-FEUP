#include <iostream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include "Date.h"
#include "Box.h"


// Randomly generates a valid day, hour and minute
using namespace std;
Date currentDate(){
	int n, h, m;
	string weekday;
	srand(time(NULL));
	n = rand() % 70 + 1;
	if (n >= 1 && n <= 10){
		weekday = "Monday";
	}
	if (n >= 11 && n <= 20){
		weekday = "Tuesday";
	}
	if (n >= 21 && n <= 30)
	{
		weekday = "Wendnesday";
	}
	if (n >= 31 && n <= 40){
		weekday = "Thursday";
	}
	if (n >= 41 && n <= 50){
		weekday = "Friday";
	}
	if (n >= 51 && n <= 60){
		weekday = "Saturday";
	}
	if (n >= 61 && n <= 70){
		weekday = "Sunday";
	}
	h = rand() % 24;
	m = rand() % 60;
  // to be implemented by the students
	return Date(weekday,h,m);
}

void clrscr();
bool getpassword(string password);
string askpassword(Box &box);
string returnpassword(){
	string password;
	ifstream file;
	file.open("config.txt");
	getline(file, password);
	file.close();
	return password;
}
//check if ther is a config file
void checkifconfig(Box &box){
	string text;
	ifstream rfile;
	rfile.open("config.txt");
	if (!getline(rfile, text)){
		cout << endl << endl << "Welcome to first use of Mico Box";
		cout << endl << "You need to set a password to the privileged user\n\n";
		system("pause");
		string password = askpassword(box);
		while (password == ""){
			cout << "WARNING YOU NEED TO SET A PASSWORD\n";
			system("pause");
			password=askpassword(box);
		}
		ofstream wfile;
		wfile.open("config.txt",'w');
		wfile << password;
		rfile.close();
		wfile.close();
		box.changePassword(password);
	}
}
//SCREEN
void firstscreen();
void menu(Box &box);
//Date date = currentDate();
Date date = Date("Monday", 14, 30); // for test
string datastring = date.tostring();
Box &box = Box(returnpassword(), date);
bool proginchannel(string channel, string day, int hour, int minutes){
	if (box.dateinchannel(channel, day, hour, minutes)){
		return true;
	}
	else{
		return false;
	}
}
vector<Program> records(){
	return box.verifygetrecords();
}


void coutdata(){
	cout << datastring;
}
string retchannelstring(Program p){
	return box.retchannel(p);
}
int main(){
	firstscreen();
	cout << endl << endl << "Loading...\n";
	box.vecchan();
	box.setrecordlist();
	box.getMovieClub();
	box.getSeenMovies();
	checkifconfig(box);
	clrscr();
	firstscreen();
	Sleep(2000);
	menu(box);
	clrscr();
  exit(0);
  return 0;
}

Date getdata(){
	return date;
}

