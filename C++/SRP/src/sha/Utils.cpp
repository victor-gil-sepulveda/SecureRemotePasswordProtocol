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
				std::bitset<32> x(mask);
				cout<<x<<endl;
				cout<<value<<" "<<mask<<" "<<(value & mask)<<endl;
				result.push_back((unsigned char)((value & mask) >> 8*i));
				mask = mask<<8;
			}

			return result;
		}


		std::string char_vector_to_hex_str(vector<unsigned char>& s){
			std::stringstream ss;

			BOOST_FOREACH(unsigned char c, s)
			{
				ss<<std::hex<<"0x"<<(int)c<<" ";
			}

			return ss.str();
		}


		std::string string_to_hex_str(std::string s){
			std::stringstream ss;

			BOOST_FOREACH(unsigned char c, s)
			{
				ss<<std::hex<<"0x"<<(int)c<<" ";
			}

			return ss.str();
		}

	}
}
