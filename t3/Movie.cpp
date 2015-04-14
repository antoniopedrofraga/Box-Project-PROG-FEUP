#include "Movie.h"

Movie::Movie(string title, float cost, unsigned timeswatched){
	this->title = title;
	this->cost = cost;
	this->times = timeswatched;
}

string Movie::getName(){
	return title;
}

float Movie::getPrice(){
	return cost;
}

unsigned int Movie::timesRented(){
	return times;
}
