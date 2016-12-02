/*
 * Base64.h
 *
 *  Created on: Dec 3, 2016
 *      Author: victor
 */

#ifndef SHA_BASE64_H_
#define SHA_BASE64_H_
#include <string>

namespace algorithms {

	namespace Base64 {

		const std::string base64_chars =
				 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				 "abcdefghijklmnopqrstuvwxyz"
				 "0123456789+/";

		static inline bool is_base64(unsigned char c);
		std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
		std::string base64_decode(std::string const& encoded_string);

	}

} /* namespace algorithms */

#endif /* SHA_BASE64_H_ */
