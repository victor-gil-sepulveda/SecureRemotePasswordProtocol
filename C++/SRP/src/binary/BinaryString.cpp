/*
 * BinaryString.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#include "../binary/BinaryString.h"

#include <boost/assert.hpp>
#include <algorithm>
#include <iostream>

#include "BinaryWord.h"
#include "tools/BinaryStringUtils.h"
#include "tools/Utils.h"
using namespace std;


BinaryString::BinaryString(){}

BinaryString::BinaryString(std::string const& initial_message) {

	// ---------------------------------------------------------------------------------------
	// Division in 512b (64B) chunks, each subdivided in words of 32b (4B) words (16 words
	// per chunk). In this case we only need to create the words.
	// ---------------------------------------------------------------------------------------
	if(initial_message.size()%64 != 0){
		throw runtime_error("Error at BinaryString::Constructor: the message is not multiple of 512b.");
	}

	num_512b_chunks = initial_message.size()/64;
	for (unsigned int i = 0; i < initial_message.size()/4; ++i){
		const string message_chunk = initial_message.substr(i*4, 4);
		BinaryWord* bw = new BinaryWord(message_chunk, Endianness::BIG);// TODO: this to little, to_string must be made symmetrical
		this->words.push_back(bw);
		this->uint_words.push_back(bw->to_int());
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
}

unsigned int BinaryString::get_num_512b_chunks() const {
	return num_512b_chunks;
}

void BinaryString::get_words_from_chunk(unsigned int chunk_nr, std::vector<BinaryWord*>& out) {
	out.clear();
	out.resize(16);
	unsigned int start = chunk_nr*16;
	for (unsigned int i = 0; i < 16; ++i) out[i+start] = out[i];
}

void BinaryString::get_words_from_chunk(unsigned int chunk_nr, std::vector<std::uint32_t>& out) {
	out.clear();
	out.resize(16);
	unsigned int start = chunk_nr*16;
	for (unsigned int i = 0; i < 16; ++i) out[i] = this->uint_words[start+i];
}
