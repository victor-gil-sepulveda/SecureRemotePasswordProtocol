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
#include <boost/foreach.hpp>

namespace algorithms {

	namespace utils {
		std::vector<unsigned char> get_bytes(unsigned long int value);
		std::vector<unsigned char> get_bytes(std::string value);

		unsigned long int bytes_to_int(std::vector<unsigned char> bytes);

		template<typename T>
		std::string to_hex_str(T& s);

		std::string pad_with_character(std::string const& message, char c,
				unsigned int chunk_byte_size, unsigned int last_chunk_byte_size);

	} /* namespace utils */

} /* namespace algorithms */

template<typename T>
std::string algorithms::utils::to_hex_str(T& s){
	std::stringstream ss;

	BOOST_FOREACH(unsigned char c, s)
	{
		ss<<std::hex<<"0x"<<(int)c<<" ";
	}

	std::string result = ss.str();
	return result.substr(0, result.size()-1);
}

#endif /* SHA_UTILS_H_ */
