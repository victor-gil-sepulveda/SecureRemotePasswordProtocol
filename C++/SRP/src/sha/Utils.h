/*
 * Utils.h
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#ifndef SHA_UTILS_H_
#define SHA_UTILS_H_

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <boost/foreach.hpp>
#include <cstdint>

#define BIT_TO_BYTE_SIZE(x) (x/8)
#define BYTE_TO_BIT_SIZE(x) (x*8)

namespace algorithms {

	namespace utils {
		std::vector<unsigned char> get_bytes(std::uint32_t value);
		std::vector<unsigned char> get_bytes(std::string value);

		std::uint32_t bytes_to_int(std::vector<unsigned char> bytes);

		std::string pad_with_character(std::string const& message, char c,
				unsigned int chunk_byte_size, unsigned int last_chunk_byte_size);

		// FEATURE: partial specialization of functions (can be done via overloading,
		// but this forces a compile-time choice)
		// This implementation would be a better choice :) (http://blog.regehr.org/archives/1063)
		template <int n>
		std::uint32_t circular_left_shift(std::uint32_t val);

		template <typename T>
		std::string to_hex_str(T& s);


		void print_32b_bin(std::uint32_t x, std::uint32_t y);


	} /* namespace utils */

} /* namespace algorithms */

template<typename T>
std::string algorithms::utils::to_hex_str(T& s){
	std::stringstream ss;

	BOOST_FOREACH(unsigned char c, s)
	{
		ss<<"0x"<<std::hex<<std::setfill('0')<<std::setw(2)<<(int)c<<" ";
	}

	std::string result = ss.str();
	return result.substr(0, result.size()-1);
}


template <int n>
std::uint32_t algorithms::utils::circular_left_shift(std::uint32_t val){
	int amount = n % 64;
	if (amount == 0) return val;

	std::uint32_t mask = 0xFFFFFFFF;
	std::uint32_t lmask = mask >> n;
	std::uint32_t hmask = mask << (32 - n);
	uint32_t l = lmask&val;
	uint32_t h = hmask&val;
	return l << n | h >> (32-n);

}



#endif /* SHA_UTILS_H_ */
