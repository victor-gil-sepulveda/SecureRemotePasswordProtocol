//============================================================================
// Name        : SRP.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>

#include "sha/Sha1.h"

using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	cout<< sizeof(0x67452301)<<endl;
	long long int h0 = 0x67452301;
	cout<< sizeof(h0)<<endl;
	auto h1 = 0x67452301;
	cout<< sizeof(h1)<<" "<<h1<<endl;
	std::string ciao = "ciao";
	cout<<sizeof(ciao)<<" "<<sizeof(ciao.c_str())<<endl;

	algorithms::Sha1 hash;
	hash.calculate("lol");
	return 0;
}
