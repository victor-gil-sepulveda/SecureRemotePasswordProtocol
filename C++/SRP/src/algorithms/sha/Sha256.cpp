/*
 * Sha256.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: victor
 */

#include "Sha256.h"

#include <string>
#include <vector>
#include "../../binary/BinaryString.h"
#include "../../binary/BinaryWord.h"
#include "../../binary/Endianness.h"
#include "../../binary/tools/Utils.h"
#include "Sha1.h"

std::string algorithms::Sha256::calculate_hash(
		const std::string& initial_message) {
	std::uint32_t h0 = algorithms::Sha256::InitialHashValues::H0;
	std::uint32_t h1 = algorithms::Sha256::InitialHashValues::H1;
	std::uint32_t h2 = algorithms::Sha256::InitialHashValues::H2;
	std::uint32_t h3 = algorithms::Sha256::InitialHashValues::H3;
	std::uint32_t h4 = algorithms::Sha256::InitialHashValues::H4;
	std::uint32_t h5 = algorithms::Sha256::InitialHashValues::H5;
	std::uint32_t h6 = algorithms::Sha256::InitialHashValues::H6;
	std::uint32_t h7 = algorithms::Sha256::InitialHashValues::H7;

	std::string message = algorithms::Sha1::preprocess_message(initial_message);

	// Process the message in successive 512-bit chunks:
	// break message into 512-bit chunks
	BinaryString bin_message(message);
	unsigned int number_of_chunks = bin_message.get_num_512b_chunks();

	std::vector<std::uint32_t> words;
	for(unsigned int chunk_id = 0; chunk_id < number_of_chunks; ++chunk_id){

		algorithms::Sha256::word_array_init(chunk_id, bin_message, words);

		algorithms::Sha256::main_loop(h0, h1, h2, h3, h4, h5, h6, h7, words);

	}

	// Produce the final hash value (big-endian) as a 160 bit number:
	std::string hash = BinaryWord(h0, Endianness::BIG).to_string() +
			BinaryWord(h1, Endianness::BIG).to_string() +
			BinaryWord(h2, Endianness::BIG).to_string() +
			BinaryWord(h3, Endianness::BIG).to_string() +
			BinaryWord(h4, Endianness::BIG).to_string() +
			BinaryWord(h5, Endianness::BIG).to_string() +
			BinaryWord(h6, Endianness::BIG).to_string() +
			BinaryWord(h7, Endianness::BIG).to_string();
	return hash;
}

void algorithms::Sha256::word_array_init(unsigned int chunk_id,
		BinaryString& bin_message, std::vector<std::uint32_t>& words) {

//	for i from 16 to 63
//	        s0 := (w[i-15] rightrotate 7) xor (w[i-15] rightrotate 18) xor (w[i-15] rightshift 3)
//	        s1 := (w[i-2] rightrotate 17) xor (w[i-2] rightrotate 19) xor (w[i-2] rightshift 10)
//	        w[i] := w[i-16] + s0 + w[i-7] + s1

	bin_message.get_words_from_chunk(chunk_id, words);
	words.resize(64);
	// Extend the sixteen 32-bit words into eighty 32-bit words:
	std::uint32_t s0, s1;
	for (unsigned int i = 16; i < 64; ++i) {
		s0 = algorithms::utils::circular_right_shift<7>(words[i-15]) ^
				algorithms::utils::circular_right_shift<18>(words[i-15]) ^
				(words[i-15] >> 3);
		s1 = algorithms::utils::circular_right_shift<17>(words[i-2]) ^
				algorithms::utils::circular_right_shift<19>(words[i-2]) ^
				(words[i-2] >> 10);
		words[i] = words[i-16] + s0 + words[i-7] + s1;
	}
}

void algorithms::Sha256::main_loop(std::uint32_t& h0, std::uint32_t& h1,
		std::uint32_t& h2, std::uint32_t& h3, std::uint32_t& h4,
		std::uint32_t& h5, std::uint32_t& h6, std::uint32_t& h7,
		const std::vector<std::uint32_t>& words) {

//	Initialize working variables to current hash value:
//	    a := h0
//	    b := h1
//	    c := h2
//	    d := h3
//	    e := h4
//	    f := h5
//	    g := h6
//	    h := h7
	std::uint32_t a, b, c, d, e, f, g, h, s0, s1, ch, maj, temp1, temp2;
	a = h0;
	b = h1;
	c = h2;
	d = h3;
	e = h4;
	f = h5;
	g = h6;
	h = h7;

//	    Compression function main loop:
//	    for i from 0 to 63
//	        S1 := (e rightrotate 6) xor (e rightrotate 11) xor (e rightrotate 25)
//	        ch := (e and f) xor ((not e) and g)
//	        temp1 := h + S1 + ch + k[i] + w[i]
//	        S0 := (a rightrotate 2) xor (a rightrotate 13) xor (a rightrotate 22)
//	        maj := (a and b) xor (a and c) xor (b and c)
//	        temp2 := S0 + maj
//
//	        h := g
//	        g := f
//	        f := e
//	        e := d + temp1
//	        d := c
//	        c := b
//	        b := a
//	        a := temp1 + temp2
	for (unsigned int i = 0; i < 64; ++i){
	    s1 = algorithms::utils::circular_right_shift<6>(e) ^
	    		algorithms::utils::circular_right_shift<11>(e) ^
				algorithms::utils::circular_right_shift<25>(e);
		ch = (e & f) ^ ((~e) & g);
		temp1 = h + s1 + ch + algorithms::Sha256::ROUND_CONSTANTS[i] + words[i];
		s0 = algorithms::utils::circular_right_shift<2>(a ) ^
				algorithms::utils::circular_right_shift<13>(a) ^
				algorithms::utils::circular_right_shift<22>(a);
		maj = (a & b) ^ (a & c) ^ (b & c);
		temp2 = s0 + maj;

		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
	}

//	    Add the compressed chunk to the current hash value:
	    h0 += a;
	    h1 += b;
	    h2 += c;
	    h3 += d;
	    h4 += e;
	    h5 += f;
	    h6 += g;
	    h7 += h;

}
