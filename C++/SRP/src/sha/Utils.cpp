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
#include <stdexcept>
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
				throw runtime_error("[ERROR get_bytes<string>] The string must have exactly 4 characters");
			}

			vector<unsigned char> result;
			for (unsigned int i = 0; i < 4; ++i){
				result.push_back((unsigned char) value[i]);
			}

			return result;
		}

		unsigned long int bytes_to_int(vector<unsigned char> bytes){
			unsigned long int val = 0;
			for (unsigned int i = 0; i<4; ++i){
				val = (val<<8) | bytes[3-i];
			}
			return val;
		}

		string pad_with_character(string const& message, char c, unsigned int chunk_byte_size, unsigned int last_chunk_byte_size){
			string tmp(message);
			// Append 0 ≤ k < 512 bits '0', such that the resulting message length in bits
			// is congruent to −64 ≡ 448 (mod 512)
			int last_chunk_byte_size_remainder = last_chunk_byte_size - (message.size() % chunk_byte_size);

			if( last_chunk_byte_size_remainder < 0){
				last_chunk_byte_size_remainder = (chunk_byte_size - message.size()) + last_chunk_byte_size;
			}
			tmp += std::string(last_chunk_byte_size_remainder, c);
			return tmp;
		}

	}

}
