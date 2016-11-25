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

namespace algorithms {

	namespace utils {
		std::vector<unsigned char> get_bytes(unsigned long int value);
		std::string char_vector_to_hex_str(std::vector<unsigned char>& s);
		std::string string_to_hex_str(std::string s);
	} /* namespace utils */

} /* namespace algorithms */

#endif /* SHA_UTILS_H_ */
