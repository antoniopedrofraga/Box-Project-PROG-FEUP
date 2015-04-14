#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include "Date.h"
#include "Box.h"

using namespace std;

void firstscreen();
void menu(Box &box);
void clrscr();
void coutdata();
Date currentDate();
int progbyday(Box &box);
int progbydayall(Box &box);
bool admin = false;


//BOX STUFFF
Box::Box(string passwd, Date date) :currentDate(date){
	password = passwd;
}

void Box::vecchan(){
	vector <Channel> canais;
	string info;
	ifstream file;
	file.open("Channels.txt");
	if (file){
		getline(file, info);
		Channel chan = Channel(info);
		while (getline(file, info)){
			while (info[0] != 'c'){
				Program p;
				p=p.getproginfo(file, info);
				chan.setChannel(p);
				if (!getline(file, info)){
					info = "c";
					canais.push_back(chan);
				}
				else if (info[0] == 'c'){
					canais.push_back(chan);
					chan = Channel(info);
				}
			}
		}
	}
	else{
		clrscr();
		cout << "ERROR!! Channels were not found!!\n";
		system("pause");
		exit(1);
	}
	this->channels = canais;
}

bool repeated(string a,vector<string> p,int position){ //check if string is repeated on vector
	int size = p.size();
	for (int i = position+1; i < size; i++){
		if (a == p[i]){
			return true;
		}
	}
	return false;
}

Date Box::todate(){
	return currentDate;
}

//PARTES GRAFICAS
//http://www.cplusplus.com/articles/4z18T05o/#OSSpecificWays

//Ordenar um vector de programas com bublesort
vector<Program> bublesortprog(vector<Program> vec){
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

void clrscr()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

const int ENTER = 13; const int BACKSPACE = 8; const int ESC = 27;

bool getpassword(string password){ //check if password is on txt
	string check = "";
	ifstream myfile;
	myfile.open("config.txt");
	if (myfile){
		while (getline(myfile, check)){
			if (password.compare(check) == 0){
				return true;
			}
		}
		return false;
	}
	else{
		clrscr();
		cout << "\n ERROR \n\n THERE IS NO PASSWORD FILE\n\n";
		system("pause");
		exit(0);
	}
}

string askpassword(Box &box){
	clrscr();
	firstscreen();
	cout << endl << "[ESC] to go back\n"<<endl<< "Password: ";
	unsigned char a;
	string password = "";
	a = _getch();
	while (a != ENTER){
		if (a == ESC){
			clrscr();
			return "";
		}
		if (a == 0 || a == 0xE0){
			a = _getch();
		}
		else{
			if (a != BACKSPACE) {
				password.push_back(a);
				cout << '*';
			}
			else if (a == BACKSPACE){
				clrscr();
				if (password.length() > 0){
					password.pop_back(); //apaga uma caracter da string caso backspace
				}
				firstscreen();
				cout << endl << "[ESC] to go back\n" << endl << "Password: ";
				for (int unsigned b = 0; b < password.length(); b++){ //display dos asteriscos
					cout << "*";
				}
			}
		}
		a = _getch();
	}
	return password;
}

void firstscreen(){
	string text;
	ifstream myfile;
	myfile.open("images.txt");
	if (myfile){
		while (getline(myfile, text)){
			cout << "\t\t" << text << endl;
		}
		myfile.close();
		cout << endl << "\t\t\t\t\t\t\t";
		coutdata();
		cout << endl << endl;
	}
	else{
		clrscr();
		cout << "\n ERROR \n\n THERE IS NO IMAGES FILE\n\n";
		system("pause");
		exit(0);
	}
}
//!!!!!!ABA TODAY!!!!
//!!!!!!!!!!!!!!!!!!!
int proginchan(Box &box,int chan){
	clrscr();
	firstscreen();
	Date data = box.todate();
	string c = box.channelname(chan);
	vector<Program> p = box.listByChannel(c,data.getday());
	int sizevectprog = p.size();
	cout << "\n\t[ESC] to go back\n\n\t";
	box.printchan(chan);
	cout << endl << endl;
	if (!sizevectprog){
		cout << "\n\n\tTheres no programs in this channel today\n\n" << "\tPress any key to go back...\n\n";
		_getch();
		return 0;
	}
	for (int i = 0; i < sizevectprog; i++){
		cout << "\t" << i + 1 << "." << p[i].retname() << endl;
	}
	int proggetch = _getch();
	if (proggetch != ESC){
		proggetch = proggetch - 48;
		proggetch--; //Para ficar na posiçao dos vetores
		while ((proggetch >= sizevectprog) || (proggetch < 0)){
			clrscr();
			cout << "\tWARNING!!! its not a valid value, try again \n";
			cout << "\tPress any key to continue...\n\t";
			_getch();
			clrscr();
			firstscreen();
			cout << "\n\t[ESC] to go back\n\n\t";
			box.printchan(chan);
			cout << endl << endl;
			for (int i = 0; i < sizevectprog; i++){
				cout << "\t" << i + 1 << "." << p[i].retname() << endl;
			}
			proggetch = _getch();
			if (proggetch != ESC){
				proggetch = proggetch - 48;
				proggetch--; //Para ficar na posiçao dos vetores
			}
			else{
				return 0;
			}
		}
		p[proggetch].printproginfo();
		proginchan(box, chan);
	}
	else{
		return 0;
	}
	return 0;
}
//programas por canais
int progbychan(Box &box){
	int chan;
	clrscr();
	firstscreen();
	Date data = box.todate();
	string c = data.getday();
	vector<Channel> p = box.retchanonday(c);
	cout << "\n\t[ESC] to go back\n\n\tToday/By Channels\n\n";
	int size = p.size();
	if (!size){
		cout << "\tThere is no channels on air today\n\n" << "\tPress any key to go back...\n\n";
		_getch();
		return 0;
	}
	for (int i = 0; i < size; i++){
		cout << "\t" << i + 1 << "." << p[i].retname() << endl;
	}
	cout << endl << "\t";
	chan = getch();
	if (chan != ESC){
		chan = chan - 48;
		int size = p.size();
		while ((chan <1) || (chan > size)){
			clrscr();
			cout << "\tWARNING!!! its not a valid value, try again \n";
			cout << "\tPress any key to continue...\n\t";
			_getch();
			clrscr();
			firstscreen();
			cout << "\n\t[ESC] to go back\n\n\tToday/By Channels\n\n";
			for (int i = 0; i < size; i++){
				cout << "\t" << i + 1 << "." << p[i].retname() << endl;
			}
			cout << endl << "\t";
			chan = getch();
			if (chan != ESC){
				chan = chan - 48;
			}
			else{
				return 0;
			}
		}
		if (chan != ESC){//ver os programas do canal
			chan = chan--;
			proginchan(box, chan);
		}
		else{
			return 0;
		}
		progbychan(box);
	}
	return 0;
}
//programas por dia
int dipsprogbyday(Box &box, string day){
	clrscr();
	firstscreen();
	cout << "\n\t[ESC] to go back\n\n\t" << day <<"\n\n";
	vector<Program> v;
	Program p = Program();
	v = box.listByDay(day);
	int size = v.size();
	if (size == 0){
		cout << "\n\tThere is no programs for " << day << endl;
		cout << "\n\tPress any key to go back...\n";
		_getch();
		return 0;
	}
	else{
		for (int i = 0; i < size; i++){
			cout << "\t" << i + 1 << "." << v[i].retname() << "  (from " << box.retchannel(v[i]) << ")\n";
		}
		int getch = _getch();
		if (getch != ESC){
			getch = getch - 48;
			getch--;
			while ((getch < 0) || (getch >= size)){
				clrscr();
				cout << "\tWARNING!!! its not a valid value, try again \n";
				cout << "\tPress any key to continue...\n\t";
				_getch();
				clrscr();
				firstscreen();
				cout << "\n\t[ESC] to go back\n\n\t" << day << "\n\n";
				for (int i = 0; i < size; i++){
					cout << "\t" << i + 1 << "." << v[i].retname() << "  (from " << box.retchannel(v[i]) << ")\n";
				}
				getch = _getch();
				if (getch != ESC){
					getch = getch - 48;
					getch--;
				}
				else{
					return 0;
				}
			}
			v[getch].printproginfo();
			dipsprogbyday(box, day);
		}
		else{
			return 0;
		}
		
		return 0;
	}
	return 0;
}
int progsoftypetodaymenu(Box &box,string type){
	vector<Program> p = box.progsoftypetoday(type);
	clrscr();
	firstscreen();
	cout << endl << "\t[ESC] to go back\n\n\tToday from type " << type << "\n";
	for (int i = 0; i < p.size(); i++){
		
			cout << "\n\t" << i + 1 << "." << p[i].retname() << "  (from " << box.retchannel(p[i]) << ")";
		
	}
	int a = _getch();
	if (a == ESC){
		return 0;
	}
	a = a - 49;
	while (a < 0 || a >= p.size()){
		clrscr();
		cout << "\tWARNING!!! its not a valid value, try again \n";
		cout << "\tPress any key to continue...\n\t";
		_getch();
		clrscr();
		firstscreen();
		cout << endl << "\t[ESC] to go back\n\n\tToday from type " << type << "\n";
		for (int i = 0; i < p.size(); i++){
			
				cout << "\n\t" << i + 1 << "." << p[i].retname() << "  (from " << box.retchannel(p[i]) << ")";
			
			
		}
		 a = _getch();
		if (a == ESC){
			return 0;
		}
		a = a - 49;
	}
	p[a].printproginfo();
	progsoftypetodaymenu(box, type);
}
int progbytype(Box &box){
	vector<string> typestoday = box.typestoday();
	int size = typestoday.size();
	clrscr();
	firstscreen();
	cout << "\n\t[ESC] to go back\n\n\tTypes of programs today\n";
	if (!size){
		cout << "\n\n\tThere's no programs on air today.\n\t";
		system("pause");
		return 0;
	}
	for (int i = 0; i < size; i++){
			cout << "\n\t" << i + 1 << "." << typestoday[i];
		
		
	}
	cout << "\n\n\t";
	int a = _getch();
	if (a == ESC){
		return 0;
	}
	a = a - 49;
	while (a < 0 || a >= size){
		clrscr();
		cout << "\tWARNING!!! its not a valid value, try again \n";
		cout << "\tPress any key to continue...\n\t";
		_getch();
		clrscr();
		firstscreen();
		cout << "\n\t[ESC] to go back\n\n\tTypes of programs today\n";
		for (int i = 0; i < size; i++){
				cout << "\n\t" << i + 1 << "." << typestoday[i];
		}
		cout << "\n\n\t";
		a = _getch();
		if (a == ESC){
			return 0;
		}
		a = a - 49;
	}
	progsoftypetodaymenu(box,typestoday[a]);
	progbytype(box);
	return 0;
}
int channels(Box &box){
	clrscr();
	firstscreen();
	cout << "\n\t[ESC] to go back\n\n\tToday\n\n\t1.Channels on air today" << "\n\t2.Programs today" << "\n\t3.Programs today by type\n";
	int chan = _getch();
	if (chan != ESC){
		chan = chan - 48;
		clrscr();
		int size = 3;
		cout << "\n\t[ESC] to go back\n\n\tToday\n\n\t1.Channels on air today" << "\n\t2.Programs today" << "\n\t3.Programs today by type\n";
		while ((chan <1) || (chan > size) && (chan != ESC)){
			clrscr();
			cout << "\tWARNING!!! its not a valid value, try again \n";
			cout << "\tPress any key to continue...\n\t";
			_getch();
			clrscr();
			firstscreen();
			cout << "\n\t[ESC] to go back\n\n\tToday\n\n\t1.Channels on air today" << "\n\t2.Programs today" << "\n\t3.Programs today by type\n";
			chan = getch();
			if (chan != ESC){
				chan = chan - 48;
			}
			else{
				return 0;
			}
		}
		if (chan == 1){
			progbychan(box);
		}
		else if (chan == 2){
			Date a = box.todate();
			dipsprogbyday(box, a.getday());
		}
		else if (chan == 3){
			progbytype(box);
		}
		clrscr();
		channels(box);
	}
	else{
		return 0;
	}
}

//!!!ABA ALL WEEK!!!!
//!!!!!!!!!!!!!!!!!!!
int proginchanall(Box &box, int chan){
	clrscr();
	firstscreen();
	Date data = box.todate();
	string c = box.channelname(chan);
	int sizevectprog = box.vecprogsize(chan);
	cout << "\n\t[ESC] to go back\n\n\t";
	box.printchan(chan);
	cout << endl << endl;
	box.printprog(chan);
	int proggetch = _getch();
	if (proggetch != ESC){
		proggetch = proggetch - 48;
		proggetch--; //Para ficar na posiçao dos vetores
		while ((proggetch >= sizevectprog) || (proggetch < 0)){
			clrscr();
			cout << "\tWARNING!!! its not a valid value, try again \n";
			cout << "\tPress any key to continue...\n\t";
			_getch();
			clrscr();
			firstscreen();
			cout << "\n\t[ESC] to go back\n\n\t";
			box.printchan(chan);
			cout << endl << endl;
			box.printprog(chan);
			proggetch = _getch();
			if (proggetch != ESC){
				proggetch = proggetch - 48;
				proggetch--; //Para ficar na posiçao dos vetores
			}
			else{
				return 0;
			}
		}
		box.DispProgInfoWithChanAndProg(chan, proggetch);
		proginchanall(box, chan);
	}
	else{
		return 0;
	}
	return 0;
}
//programas por canais
int progbychanall(Box &box){
	int chan;
	clrscr();
	firstscreen();
	cout << "\n\t[ESC] to go back\n\n\tAll week/By Channels\n\n";
	box.printallchan();
	cout << endl << "\t";
	chan = getch();
	if (chan != ESC){
		chan = chan - 48;
		int size = box.vecchansize();
		while ((chan <1) || (chan > size)){
			clrscr();
			cout << "\tWARNING!!! its not a valid value, try again \n";
			cout << "\tPress any key to continue...\n\t";
			_getch();
			clrscr();
			firstscreen();
			cout << "\n\t[ESC] to go back\n\n\tAll week/By Channels\n\n";
			box.printallchan();
			cout << endl << "\t";
			chan = getch();
			if (chan != ESC){
				chan = chan - 48;
			}
			else{
				return 0;
			}
		}
		if (chan != ESC){//ver os programas do canal
			chan = chan--;
			proginchanall(box, chan);
		}
		else{
			return 0;
		}
		progbychanall(box);
	}
	return 0;
}
int dipsprogbydayall(Box &box, string day){
	clrscr();
	firstscreen();
	cout << "\n\t[ESC] to go back\n\n\t" << day << "\n\n";
	vector<Program> v;
	Program p = Program();
	v = box.listByDay(day);
	int size = v.size();
	if (size == 0){
		cout << "\n\tThere is no programs for " << day << endl;
		cout << "\tPress any key to go back...\n";
		_getch();
		progbydayall(box);
		return 0;
	}
	else{
		for (int i = 0; i < size; i++){
			cout << "\t" << i + 1 << "." << v[i].retname() << "  (from " << box.retchannel(v[i]) << ")\n";
		}
		int getch = _getch();
		if (getch != ESC){
			getch = getch - 48;
			getch--;
			while ((getch < 0) || (getch > size)){
				clrscr();
				cout << "\tWARNING!!! its not a valid value, try again \n";
				cout << "\tPress any key to continue...\n\t";
				_getch();
				clrscr();
				firstscreen();
				cout << "\n\t[ESC] to go back\n\n\t" << day << "\n\n";
				for (int i = 0; i < size; i++){
					cout << "\t" << i + 1 << "." << v[i].retname() << "  (from " << box.retchannel(v[i]) << ")\n";
				}
				getch = _getch();
				if (getch != ESC){
					getch = getch - 48;
					getch--;
				}
				else{
					progbydayall(box);
					return 0;
				}
			}
			v[getch].printproginfo();
			dipsprogbydayall(box, day);
		}
		else{
			progbydayall(box);
			return 0;
		}
		progbydayall(box);
		return 0;
	}
	progbydayall(box);
	return 0;
}
int progbydayall(Box &box){
	int chan;
	clrscr();
	firstscreen();
	cout << "\n\t[ESC] to go back\n\n\tAll week/By Days\n" << "\n\t1.Sunday\n\t2.Monday\n\t3.Tuesday\n\t4.Wendnesday\n\t5.Thursday\n\t6.Friday\n\t7.Saturday\n";
	cout << endl << "\t";
	chan = getch();
	if (chan != ESC){
		chan = chan - 48;
		int size = 7;
		while ((chan <1) || (chan > size) && (chan != ESC)){
			clrscr();
			cout << "\tWARNING!!! its not a valid value, try again \n";
			cout << "\tPress any key to continue...\n\t";
			_getch();
			clrscr();
			firstscreen();
			cout << "\n\t[ESC] to go back\n\n\tAll week/By Days\n" << "\n\t1.Sunday\n\t2.Monday\n\t3.Tuesday\n\t4.Wendnesday\n\t5.Thursday\n\t6.Friday\n\t7.Saturday\n";
			cout << endl << "\t";
			chan = getch();
			if (chan != ESC){
				chan = chan - 48;
			}
			else{
				return 0;
			}
		}if (chan != ESC){//ver os programas do canal
			if (chan == 1){
				dipsprogbyday(box, "Sunday");
			}
			else if (chan == 2){
				dipsprogbyday(box, "Monday");
			}
			else if (chan == 3){
				dipsprogbyday(box, "Tuesday");
			}
			else if (chan == 4){
				dipsprogbyday(box, "Wendnesday");
			}
			else if (chan == 5){
				dipsprogbyday(box, "Thursday");
			}
			else if (chan == 6){
				dipsprogbyday(box, "Friday");
			}
			else if (chan == 7){
				dipsprogbyday(box, "Saturday");
			}
		}
		else{
			return 0;
			progbydayall(box);
		}
		progbydayall(box);
		return 0;
	}
}
int allprogramsoftype(Box &box, string type){
	vector<Program> progbytype = box.retprogfromtype(type);
	int size = progbytype.size();
	clrscr();
	firstscreen();
	cout << "\n\t[ESC] to go back\n\n\tPrograms of type " << type << "\n";
	for (int i = 0; i < size; i++){
			cout << "\n\t" << i + 1 << "." << progbytype[i].retname() << "  (from " << box.retchannel(progbytype[i]) << ")";
		
	}
	cout << "\n\n";
	int a = getch();
	if (a == ESC){
		return 1;
	}
	else{
		a = a - 49;
		while ((a < 0) || (a >= progbytype.size())){
			clrscr();
			cout << "\tWARNING!!! its not a valid value, try again \n";
			cout << "\tPress any key to continue...\n\t";
			_getch();
			clrscr();
			firstscreen();
			cout << "\n\t[ESC] to go back\n\n\tPrograms of type " << type << "\n";
			for (int i = 0; i < size; i++){
				
					cout << "\n\t" << i + 1 << "." << progbytype[i].retname() << "  (from " << box.retchannel(progbytype[i]) << ")";
				
			}
			cout << "\n\n";
			a = getch();
			if (a == ESC){
				return 0;
			}
			else{
				a = a - 49;
			}
		}
		//
		progbytype[a].printproginfo();
		return 0;
	}
	return 0;
}
int progbytypeall(Box &box){
	vector<string> s = box.existingTypes();
	int size = s.size();
	clrscr();
	firstscreen();
	cout << "\n\t[ESC] to go back\n\n\tAll types of programs\n";
	for (int i = 0; i < size; i++){
		if ((i == 0) || (i % 2 == 0)){
			cout << "\n\t" << i + 1 << "." << s[i];
		}
		else{
			cout << "\t\t" << i + 1 << "." << s[i];
		}
	}

	cout << "\n\n\t";
	int a = getch();
	if (a == ESC){
		return 1;
	}
	else{
		a = a - 49;
		while ((a < 0) || (a >= s.size())){
			clrscr();
			cout << "\tWARNING!!! its not a valid value, try again \n";
			cout << "\tPress any key to continue...\n\t";
			_getch();
			clrscr();
			firstscreen();
			cout << "\n\t[ESC] to go back\n\n\tAll types of programs\n";
			for (int i = 0; i < size; i++){
				if ((i == 0) || (i % 2 == 0)){
					cout << "\n\t" << i + 1 << "." << s[i];
				}
				else{
					cout << "\t\t" << i + 1 << "." << s[i];
				}
			}
			a = getch();
			if (a == ESC){
				return 0;
			}
			else{
				a = a - 49;
			}
		}
		allprogramsoftype(box, s[a]);
		progbytypeall(box);
	}
	return 0;
}
int channelsall(Box &box){
	clrscr();
	firstscreen();
	cout << "\n\t[ESC] to go back\n\n\tAll week\n\n\t1.List Programs by Channel" << "\n\t2.List Programs by Day" << "\n\t3.List Programs by Type\n";
	int chan = _getch();
	if (chan != ESC){
		chan = chan - 48;
		clrscr();
		int size = 3;
		cout << "\n\t[ESC] to go back\n\n\tAll week \n\n\t1.List Programs by Channel" << "\n\t2.List Programs by Day" << "\n\t3.List Programs by Type\n";
		while ((chan <1) || (chan > size) && (chan != ESC)){
			clrscr();
			cout << "\tWARNING!!! its not a valid value, try again \n";
			cout << "\tPress any key to continue...\n\t";
			_getch();
			clrscr();
			firstscreen();
			cout << "\n\t[ESC] to go back\n\n\tAll week\n\n\t1.List Programs by Channel" << "\n\t2.List Programs by Day" << "\n\t3.List Programs by Type\n";
			chan = getch();
			if (chan != ESC){
				chan = chan - 48;
			}
			else{
				return 0;
			}
		}
		if (chan == 1){
			progbychanall(box);
		}
		else if (chan == 2){
			progbydayall(box);
		}
		else if (chan == 3){
			progbytypeall(box);
		}
		clrscr();
		channelsall(box);
	}
	else{
		return 0;
	}
}
int printrecords(Box &box){
	clrscr();
	firstscreen();
	vector<Program> p = box.getrecords();
	if (!p.size()){
		cout << "\n\n\tThere is no records at the moment\nPress any key to go pack...\n\n";
		_getch();
		return 0;
	}
	cout << "\n\t[ESC] to go back\n\n\tRecords\n";
	for (int i = 0; i < p.size(); i++){
		cout << "\n\t" << i + 1 << "." << p[i].retname() << "  (from " << box.retchannel(p[i]) << ") ";
		if (p[i].checkifonair() == "future"){
			cout << " will be recorded";
		}
	}
	int a = _getch();
	if (a == ESC){
		return 0;
	}
	a = a - 49;
	while (a < 0 || a >= p.size()){
		clrscr();
		cout << "\tWARNING!!! its not a valid value, try again \n";
		cout << "\tPress any key to continue...\n\t";
		_getch();
		clrscr();
		firstscreen();
		cout << "\n\t[ESC] to go back\n\n\tRecords\n";
		for (int i = 0; i < p.size(); i++){
			cout << "\n\t" << i + 1 << "." << p[i].retname() << "  (from " << box.retchannel(p[i]) << ") ";
			if (p[i].checkifonair() == "future"){
				cout << " will be recorded";
			}
		}
		a = _getch();
		if (a == ESC){
			return 0;
		}
		a = a - 49;
	}
	p[a].printproginfo();
	printrecords(box);
}
int updateseenmovies(Box &box,Movie m){
	int sizeseen = box.retseenmovies().size();
	for (int e = 0; e < sizeseen; e++){
		if (m.getName() == box.retseenmovies()[e].getName()){
			box.updateseenmovies(e);
			box.saveseenmovies();
			return 0;
		}
	}
	m.rented();
	box.pushbackmovie(m);
	box.saveseenmovies();
	return 0;

}
int movieclub(Box &box){
	int size = box.retmovieclub().size();
	int a = -1;
	while (a < 0 || a >= size){
		clrscr();
		firstscreen();
		cout << "\n\t[ESC] to go back\n\n\tMovie Club\tSpent: " << box.moneySpent() << "\n";
		for (int i = 0; i < size; i++){
			cout << "\n\t" << i + 1 << "." << box.retmovieclub()[i].getName();
		}
		a = _getch();
		if (a == ESC){
			return 0;
		}
		a = a - 49;
	}
	clrscr();
	firstscreen();
    Movie l = box.retmovieclub()[a];
	updateseenmovies(box,l);
	cout << "\n\t[ESC] to go back\n\n\tMovie Club\n";
	for (int i = 0; i < size; i++){
		if (i == a){
			cout << "\n\t" << i + 1 << "." << box.retmovieclub()[i].getName() << "\t[RENTED]";
		}
		else{
			cout << "\n\t" << i + 1 << "." << box.retmovieclub()[i].getName();
		}
	}
	cout << "\n\n\tLoading...";
	Sleep(2000);
	return 0;
}
int movies(Box &box){
	int a = -1;
	while (a<1 || a>2){
		clrscr();
		firstscreen();
		cout << endl << "\n\t[ESC] to go back...\n\n\tMovies\n\n\t1.Movie Club\n\t2.Movies Seen\n\n";
		a = _getch();
		if (a == ESC){
			return 0;
		}
		a = a - 48;

	}
	if (a == 2){
		box.printSeenMovies();
		movies(box);
	}
	else{
		movieclub(box);
		movies(box);
	}
}
//menu de escolha
//menu de admin
string changepassword(Box &box){
	clrscr();
	firstscreen();
	cout << endl << "[ESC] to go back\n" << endl << "New Password: ";
	unsigned char a;
	string password = "";
	a = _getch();
	while (a != ENTER){
		if (a == ESC){
			clrscr();
			return "";
		}
		if (a == 0 || a == 0xE0){
			a = _getch();
		}
		else{
			if (a != BACKSPACE) {
				password.push_back(a);
				cout << '*';
			}
			else if (a == BACKSPACE){
				clrscr();
				if (password.length() > 0){
					password.pop_back(); //apaga uma caracter da string caso backspace
				}
				firstscreen();
				cout << endl << "[ESC] to go back\n" << endl << "New Password: ";
				for (int unsigned b = 0; b < password.length(); b++){ //display dos asteriscos
					cout << "*";
				}
			}
		}
		a = _getch();
	}
	if ((a == ENTER) &&(password.size()!=0)){
		ofstream file("config.txt");
		file << password;
		file.close();
		if (file && box.changePassword(password)){
			cout << "\n\nPassword changed sucessfuly\n";
			Sleep(2000);
		}
	}
	return password;
}
int createmovie(Box &box){
	clrscr();
	firstscreen();
	string name;
	float cost;
	cin.clear();
	cin.ignore();
	cout << "\n\tGive your movie a name: ";
	getline(cin, name);
	cout << "\n\tGive " << name << " a cost: ";
	cin >> cost;
	Movie m = Movie(name, cost, 0);
	if (box.checkifinmovieclub(m)){
		cout << "\n\n\t[ERROR] This Movie already exists in MovieClub...\n\n";
		getch();
		return 1;
	}
	box.pushbackmovieclub(m);
	vector<Movie> vec = box.retmovieclub();
	ofstream file("MovieClub.txt");
	for (int i = 0; i < vec.size(); i++){
		file << vec[i].getName() << endl;
		if (vec.size() - 1 == i){
			file << vec[i].getPrice();
		}
		else{
			file << vec[i].getPrice() << endl;
		}
	}
	file.close();
	cout << "\n\tMovie Created Sucessfully";
	getch();
}
int deletemovie(Box &box){
	vector<Movie> MovieClub = box.retmovieclub();
	string a = "-1";
	int m = atoi(a.c_str());
	while (m<0 || m>MovieClub.size()-1){
		clrscr();
		firstscreen();
		cout << "\n\t[q] to go back\n\tDelete a movie\n";
		for (int i = 0; i < MovieClub.size(); i++)
		{
			cout <<"\t" << i + 1 << "." << MovieClub[i].getName() << endl;
		}
		cin >> a;
	    cin.ignore();
		if (a == "q"){
			return 1;
		}
		m = atoi(a.c_str());
		m= m - 1;
	}
	box.removemoviefromclub(m);
	vector<Movie> vec = box.retmovieclub();
	ofstream file("MovieClub.txt");
	for (int i = 0; i < vec.size(); i++){
		file << vec[i].getName() << endl;
		if (vec.size() - 1 == i){
			file << vec[i].getPrice();
		}
		else{
			file << vec[i].getPrice() << endl;
		}
	}
	file.close();
}
int updatemovie(Box &box){
	vector<Movie> MovieClub = box.retmovieclub();
	string a = "-1";
	int m = atoi(a.c_str());
	while (m<0 || m>MovieClub.size() - 1){
		clrscr();
		firstscreen();
		cout << "\n\t[q] to go back\n\tUpdate movie\n";
		for (int i = 0; i < MovieClub.size(); i++)
		{
			cout << "\t" << i + 1 << "." << MovieClub[i].getName() << endl;
		}
		cin >> a;
		cin.ignore();
		if (a == "q"){
			return 1;
		}
		m = atoi(a.c_str());
		m = m - 1;
	}
	box.updatemovieadmin(m);
	vector<Movie> vec = box.retmovieclub();
	ofstream file("MovieClub.txt");
	for (int i = 0; i < vec.size(); i++){
		file << vec[i].getName() << endl;
		if (vec.size() - 1 == i){
			file << vec[i].getPrice();
		}
		else{
			file << vec[i].getPrice() << endl;
		}
	}
	file.close();
}
int createchannel(Box &box){
	string name;
	clrscr();
	cout << "\n[q] to go back\n\nInsert channel name: ";
	getline(cin, name);
	cin.ignore();
	box.pushbackchannel(name);
	box.savechannels();
	return 0;
}

int adminmenu(Box &box){
	int a = -1;
	while (a<0 || a>7){
		clrscr();
		firstscreen();
		cout << "\n\t[ESC] to go back\n\tAdmin menu\n\n\tChannels:\n\t1.Create Channel\t2.Update Channel\t3.Delete Channel\n";
		cout << "\n\tMovies\n\t4.Create Movie \t\t" << "5.Update Movies \t6.Delete Movies\n";
		cout << "\n\tPassword\n\t7.Change Password\n\n";
		a = _getch();
		if (a == ESC){
			return 1;
		}
		a = a - 48;
	}
	if (a == 7){
		changepassword(box);
		adminmenu(box);
	}
	else if(a==4){
		createmovie(box);
		adminmenu(box);
	}
	else if (a == 6){
		deletemovie(box);
		adminmenu(box);
	}
	else if (a == 5){
		updatemovie(box);
		adminmenu(box);
	}
	else if (a == 1){
		createchannel(box);
		adminmenu(box);
	}
	return 0;
}
void escolha(int num, Box &box){

		if (num == 5){
			string password = askpassword(box);
			if (password == ""){
				clrscr();
				firstscreen();
				menu(box);
			}
			else if (getpassword(password)){
				cout << "\n\nACCESS GRANTED \n";
				system("pause");
				admin = true;
				adminmenu(box);
				menu(box);
			}
			else{
				cout << "\n\nACCESS DENIED!!!! \n";
				system("pause");
				menu(box);
			}

		}
		else if (num == 1){
			channels(box);
			clrscr();
			firstscreen();
			menu(box);
		}
		else if (num == 2){
			channelsall(box);
			clrscr();
			firstscreen();
			menu(box);
		}
		else if (num == 3){
			printrecords(box);
			clrscr();
			firstscreen();
			menu(box);
		}
		else if (num == 4){
			movies(box);
			menu(box);
		}
	}

void menu(Box &box){
	clrscr();
	firstscreen();
	int choice = 0;
	string channels = "1.Today\t";
	string listall = "2.All week";
	string movies = "4.Movies";
	string records = "3.Records";
	string premium = "5.Get Admin Rights";
	std::cout << "\n\t[ESC] to shutdown" << endl << endl;
	std::cout << "\t" << channels << "\t" << listall << "\t" << records << "\t" << movies << "\n\n\t" << premium;
	std::cout << endl << endl;
	std::cout << "\t";
	unsigned char a = _getch();
		if (a == ESC){
			clrscr();
			exit(1);
		}
		if (a == 0 || a == 0xE0){
			a = _getch();
		}
		if (a == 53){
			choice = 5;
		}
		else if (a == 49){
			choice = 1;
		}
		else if (a == 50){
			choice = 2;
		}
		else if(a==51){
			choice = 3;
		}
		else if (a == 52){
			choice = 4;
		}
		else{
			menu(box);
		}

		std::cout << endl << endl;
		std::cout << "\t";
		escolha(choice, box);
		choice = 0;
		clrscr();
		firstscreen();
		menu(box);
}

bool typeisonvec(vector<string> p, string type){
	int size = p.size();
	for (int i = 0; i < size; i++){
		if (p[i] == type){
			return true;
		}
	}
	return false;
}