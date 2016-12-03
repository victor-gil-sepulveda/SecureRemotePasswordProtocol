/*
 * BinaryStringUtils.cpp
 *
 *  Created on: Nov 29, 2016
 *      Author: victor
 */


#include "../binary/tools/BinaryStringUtils.h"

#include <string>

#include "../BinaryWord.h"
using namespace std;

namespace algorithms {
	namespace utils {
		/*
		 * Generates a string representation of the 64b length given as parameter.
		 */
		string length_to_string_64b(unsigned int message_length){
			// Append ml, the original message length, as a 64-bit big-endian integer. Thus, the total
			// length is a multiple of 512 bits.
			std::uint64_t lmask = 0x00000000FFFFFFFF;
			std::uint64_t hmask = 0xFFFFFFFF00000000;

			BinaryWord lower_word(message_length & lmask, // let the compiler do the downcast
					Endianness::BIG);
			BinaryWord higher_word((message_length & hmask)>>32,
					Endianness::BIG);

			auto higher_w_bytes = higher_word.get_bytes();
			string tmp = string(higher_w_bytes.begin(), higher_w_bytes.end());

			auto lower_w_bytes = lower_word.get_bytes();
			tmp += string(lower_w_bytes.begin(), lower_w_bytes.end());

			return tmp;
		}
	}
} /* namespace algorithms */


