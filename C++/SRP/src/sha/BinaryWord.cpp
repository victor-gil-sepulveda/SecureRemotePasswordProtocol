/*
 * BinaryWord.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#include "BinaryWord.h"

#include <vector>
#include <algorithm>
#include <stdexcept>

#include "Utils.h"
using std::runtime_error;
using std::vector;

// TODO: use std:: defined "known sizes"

BinaryWord::BinaryWord():endianness(Endianness::LITTLE){}

BinaryWord::BinaryWord(long int value, Endianness e): endianness(e) {
	this->bytes.resize(4);
	vector<unsigned char> word_bytes = algorithms::utils::get_bytes(value);
	assign_bytes(word_bytes);
}

BinaryWord::BinaryWord(std::string value, Endianness e): endianness(e){
	this->bytes.resize(4);
	vector<unsigned char> word_bytes = algorithms::utils::get_bytes(value);
	assign_bytes(word_bytes);
}

BinaryWord::BinaryWord(BinaryWord& b):endianness(b.endianness){
	this->bytes.resize(4);
	vector<unsigned char>& word_bytes = const_cast<vector<unsigned char>& >(b.get_bytes());
	// Straight copy of the bytes
	std::copy(word_bytes.begin(), word_bytes.end(), bytes.begin());
}

void BinaryWord::assign_bytes(vector<unsigned char>& word_bytes){
	switch(endianness){
		case Endianness::LITTLE:
		{
			// "Least significant byte goes into the Lowest-addressed slot"
			std::copy(word_bytes.begin(), word_bytes.end(), bytes.begin());
			break;
		}
		case Endianness::BIG:
		{
			std::reverse_copy(word_bytes.begin(), word_bytes.end(), bytes.begin());
			break;
		}
		default:
		{
			throw runtime_error("Reached default at BinaryWord::assign_bytes");
		}
	}
}

BinaryWord::~BinaryWord() {}

std::string BinaryWord::to_string(){
	std::string result;
	result.resize(4);
	switch(endianness){
		case Endianness::LITTLE:
		{
			std::copy(bytes.begin(), bytes.end(), result.begin());
			break;
		}
		case Endianness::BIG:
		{
			std::reverse_copy(bytes.begin(), bytes.end(), result.begin());
			break;
		}
		default:
		{
			throw runtime_error("Reached default at BinaryWord::to_string");
		}
	}
	return result;
}

const std::vector<unsigned char>& BinaryWord::get_bytes(){
	return bytes;
}

Endianness BinaryWord::get_endianness(){
	return endianness;
}

