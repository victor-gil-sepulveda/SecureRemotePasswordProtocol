/*
 * Utils.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#include "Utils.h"

#include <vector>
#include <sstream>
#include <boost/foreach.hpp>
#include <iostream>
#include <bitset>
using namespace std;

namespace algorithms {

	namespace utils {

		vector<unsigned char> get_bytes(unsigned long int value) {
			vector<unsigned char> result;
			unsigned long mask = 0x000000FF; //0000 0000 0000 0000 0000 0000 1111 1111

			for (unsigned int i = 0; i<4; ++i){
				//std::bitset<32> x(mask);
				//cout<<x<<endl;
				//cout<<value<<" "<<mask<<" "<<(value & mask)<<endl;
				result.push_back((unsigned char)((value & mask) >> 8*i));
				mask = mask<<8;
			}

			return result;
		}


		vector<unsigned char> get_bytes(string value) {

			if (value.size() != 4){
				cout<<"[ERROR get_bytes<string>] The string must have exactly 4 characters"<<endl;
				exit(-1);
			}

			vector<unsigned char> result;
			for (unsigned int i = 0; i < 4; ++i){
				result.push_back((unsigned char) value[i]);
			}

			return result;
		}

	}
}
