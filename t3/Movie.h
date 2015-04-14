#ifndef _MOVIE
#define _MOVIE

#include<iostream>
#include<string>

using namespace std;


class Movie{
	string title;
	//  string type; // removed in the final version of the work text
	float cost;
	unsigned times;
public:
	Movie(string title, float cost, unsigned times);
	string getName();
	float getPrice();
	unsigned int timesRented();

	void rented(){
		times = times++;
	}
	void setName(string name){
		this->title = name;
	}

	void setCost(float cost){
		this->cost = cost;
	}


};

#endif


