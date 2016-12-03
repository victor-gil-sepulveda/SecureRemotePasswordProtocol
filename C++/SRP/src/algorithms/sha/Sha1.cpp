/*
 * Sha1.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#include "../../algorithms/sha/Sha1.h"

#include <boost/assert.hpp>
#include <string>
#include <iostream>

#include "../../binary/BinaryString.h"
#include "../../binary/BinaryWord.h"
#include "../../binary/Endianness.h"
#include "../../binary/tools/BinaryStringUtils.h"
#include "../../binary/tools/Utils.h"
using namespace std;

#define CHUNK_BIT_SIZE 512
#define LAST_CHUNK_BIT_SIZE 448

std::string algorithms::Sha1::preprocess_message(const std::string& initial_message) {
	std::string tmp_message;

	// Check assumption: sizeof(char) = 8
	BOOST_ASSERT_MSG(BYTE_TO_BIT_SIZE(sizeof(char)) == 8,
			"This function expects a char size of 1 byte.");

	// -------------------------
	// Message preprocessing
	// -------------------------
	std::string  message = initial_message;
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

	return message;
}


void algorithms::Sha1::word_array_init(unsigned int chunk_id,
		BinaryString& bin_message, vector<std::uint32_t>& words) {

	bin_message.get_words_from_chunk(chunk_id, words);
	words.resize(80);

	// Extend the sixteen 32-bit words into eighty 32-bit words:
	for (unsigned int i = 16; i < 80; ++i) {
		words[i] = algorithms::utils::circular_left_shift<1>(
				words[i - 3] ^ words[i - 8] ^ words[i - 14] ^ words[i - 16]);
	}
}

void algorithms::Sha1::main_loop(std::uint32_t& h0, std::uint32_t& h1,
		std::uint32_t& h2, std::uint32_t& h3, std::uint32_t& h4,
		const vector<std::uint32_t>& words) {

	//TODO: MUST addition be % 2^32?

	std::uint32_t a, b, c, d, e, f, k, temp;

	// Initialize hash value for this chunk:
	a = h0;
	b = h1;
	c = h2;
	d = h3;
	e = h4;

	// Main loop:
	for (unsigned int i = 0; i < 80; ++i){
		if (i <= 19){
			f = (b & c) | ((~b) & d);
			k = 0x5A827999;
		}
		else{
			if (i >= 20 && i <= 39){
				f = b ^ c ^ d;
				k = 0x6ED9EBA1;
			}
			else{
				if (i>= 40 && i <= 59){
					f = (b & c) | (b & d) | (c & d);
					k = 0x8F1BBCDC;
				}
				else{
					if (i>= 60 && i <= 79){
						f = b ^ c ^ d;
						k = 0xCA62C1D6;
					}
				}
			}
		}
		temp = algorithms::utils::circular_left_shift<5>(a) + f + e + k + words[i];
		e = d;
		d = c;
		c = algorithms::utils::circular_left_shift<30>(b);
		b = a;
		a = temp;
	}
	// From https://www.ipa.go.jp/security/rfc/RFC3174EN.html
		// Better version for parallelization
		/*unsigned int t;
		for (t = 0; t < 20; t++) {
			temp = SHA1CircularShift(5, a) + ((b & c) | ((~b) & d)) + e + words[t]
					+ 0x5A827999;
			e = d;
			d = c;
			c = SHA1CircularShift(30, b);
			b = a;
			a = temp;
		}
		for (t = 20; t < 40; t++) {
			temp = SHA1CircularShift(5,a) + (b ^ c ^ d) + e + words[t] + 0x6ED9EBA1;
			e = d;
			d = c;
			c = SHA1CircularShift(30, b);
			b = a;
			a = temp;
		}
		for (t = 40; t < 60; t++) {
			temp = SHA1CircularShift(5, a) + ((b & c) | (b & d) | (c & d)) + e
					+ words[t] + 0x8F1BBCDC;
			e = d;
			d = c;
			c = SHA1CircularShift(30, b);
			b = a;
			a = temp;
		}
		for (t = 60; t < 80; t++) {
			temp = SHA1CircularShift(5,a) + (b ^ c ^ d) + e + words[t] + 0xCA62C1D6;
			e = d;
			d = c;
			c = SHA1CircularShift(30, b);
			b = a;
			a = temp;
		}*/

	// Add this chunk's hash to result so far:
	h0 += a;
	h1 += b;
	h2 += c;
	h3 += d;
	h4 += e;
}

std::string algorithms::Sha1::calculate_hash(const std::string& initial_message) {

	std::uint32_t h0 = Sha1Constants::H0;
	std::uint32_t h1 = Sha1Constants::H1;
	std::uint32_t h2 = Sha1Constants::H2;
	std::uint32_t h3 = Sha1Constants::H3;
	std::uint32_t h4 = Sha1Constants::H4;

	std::string message = preprocess_message(initial_message);

	// Process the message in successive 512-bit chunks:
	// break message into 512-bit chunks
	BinaryString bin_message(message);
	unsigned int number_of_chunks = bin_message.get_num_512b_chunks();

	vector<std::uint32_t> words;
	for(unsigned int chunk_id = 0; chunk_id < number_of_chunks; ++chunk_id){

		word_array_init(chunk_id, bin_message, words);

		main_loop(h0, h1, h2, h3, h4, words);

	}

	// Produce the final hash value (big-endian) as a 160 bit number:
	string hash = BinaryWord(h0, Endianness::BIG).to_string() +
			BinaryWord(h1, Endianness::BIG).to_string() +
			BinaryWord(h2, Endianness::BIG).to_string() +
			BinaryWord(h3, Endianness::BIG).to_string() +
			BinaryWord(h4, Endianness::BIG).to_string();

	return hash;
}


