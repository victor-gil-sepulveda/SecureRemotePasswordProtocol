/*
 * Sha1.h
 *
 * Calculates the Sha1 of a given variable following the pseudocode in:
 *
 * https://en.wikipedia.org/wiki/SHA-1
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#ifndef ALGORITHMS_SHA_SHA1_H_
#define ALGORITHMS_SHA_SHA1_H_
#include <cstdint>
#include <string>
#include <vector>

#include "../../binary/BinaryString.h"


namespace algorithms {

	/*
	 * SHA-1 implementation, following the algorithm in https://en.wikipedia.org/wiki/SHA-1
	 */
	namespace Sha1 {

			std::string calculate_hash(std::string const& initial_message);

			std::string preprocess_message(std::string const& initial_message);

			void word_array_init(unsigned int chunk_id, BinaryString& bin_message,
				std::vector<std::uint32_t>& words);

			void main_loop(std::uint32_t& h0, std::uint32_t& h1, std::uint32_t& h2,
				std::uint32_t& h3, std::uint32_t& h4,
				const std::vector<std::uint32_t>& words);

			enum InitialHashValues:std::uint32_t {
				H0 = 0x67452301,
				H1 = 0xEFCDAB89,
				H2 = 0x98BADCFE,
				H3 = 0x10325476,
				H4 = 0xC3D2E1F0
			};

			enum ChunkSizes{
				CHUNK_BIT_SIZE = 512,
				LAST_CHUNK_BIT_SIZE = 448
			};
	}

} /* namespace algorithms */

#endif /* ALGORITHMS_SHA_SHA1_H_ */
