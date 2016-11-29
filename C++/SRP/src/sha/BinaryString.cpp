/*
 * BinaryString.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#include "BinaryString.h"

#include <boost/assert.hpp>
#include <iostream>

#include "BinaryStringUtils.h"
#include "BinaryWord.h"
#include "Utils.h"
using namespace std;

#define BIT_TO_BYTE_SIZE(x) (x/8)
#define BYTE_TO_BIT_SIZE(x) (x*8)
#define CHUNK_BIT_SIZE 512
#define LAST_CHUNK_BIT_SIZE 448

BinaryString::BinaryString(){}

BinaryString::BinaryString(std::string const& initial_message) {
	// Check assumption: sizeof(char) = 8
	BOOST_ASSERT_MSG(BYTE_TO_BIT_SIZE(sizeof(char)) == 8,
			"This function expects a char size of 1 byte.");

	// -------------------------
	// Preprocess of the message
	// -------------------------
	message = initial_message;

	// Append the bit '1' to the message e.g. by adding 0x80 if message length is a multiple of 8 bits.
	message += 0x80;

	// Append 0 ≤ k < 512 bits '0', such that the resulting message length in bits
	// is congruent to −64 ≡ 448 (mod 512)
	message = algorithms::utils::pad_with_character(message, 0x00,
			BIT_TO_BYTE_SIZE(CHUNK_BIT_SIZE),
			BIT_TO_BYTE_SIZE(LAST_CHUNK_BIT_SIZE));

	// Append ml, the original message length, as a 64-bit big-endian integer. Thus, the total
	// length is a multiple of 512 bits.
	message += algorithms::utils::length_to_string_64b(BYTE_TO_BIT_SIZE(initial_message.size()));

	// ---------------------------------------------------------------------------------------
	// Division in 512b (64B) chunks, each subdivided in words of 32b (4B) words (16 words
	// per chunk). In this case we only need to create the words.
	// ---------------------------------------------------------------------------------------
	for (unsigned int i = 0; i < message.size()/4; ++i){
		this->words.push_back(new BinaryWord(message.substr(i*4, 4)));
	}
}

BinaryString::~BinaryString() {
	// TODO: change by boost::ptr_vector so that no complex
	// deallocation is needed
	for (unsigned int i = 0; i < words.size(); ++i){
		delete words[i];
	}
}

BinaryWord* BinaryString::get_32b_word_at(int chunk_nr, int word_nr){
	unsigned int index = chunk_nr*16+word_nr;
	return words[index];
};
