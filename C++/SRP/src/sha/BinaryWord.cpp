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


BinaryWord::BinaryWord():endianness(Endianness::LITTLE){
	this->bytes.resize(4);
	std::copy(bytes.begin(), bytes.end(), vector<unsigned char>(4,0).begin());
}

BinaryWord::BinaryWord(unsigned long int value, Endianness e): endianness(e) {
	this->bytes.resize(4);
	vector<unsigned char> word_bytes = algorithms::utils::get_bytes(value);
	assign_bytes(word_bytes);
}

BinaryWord::BinaryWord(std::string value, Endianness e): endianness(e){
	this->bytes.resize(4);
	vector<unsigned char> word_bytes = algorithms::utils::get_bytes(value);
	assign_bytes(word_bytes);
}

BinaryWord::BinaryWord(const BinaryWord& b):endianness(b.endianness){
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

std::string BinaryWord::to_string() const{
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

unsigned long int BinaryWord::to_int() const{
	unsigned long int result = 0;
	switch(endianness){
		case Endianness::LITTLE:
		{
			result =  algorithms::utils::bytes_to_int(this->bytes);
			break;
		}
		case Endianness::BIG:
		{
			vector<unsigned char> tmp;
			tmp.resize(4);
			std::reverse_copy(bytes.begin(), bytes.end(), tmp.begin());
			result =  algorithms::utils::bytes_to_int(this->bytes);
			break;
		}
		default:
		{
			throw runtime_error("Reached default at BinaryWord::to_string");
		}
	}
	return result;
}

const std::vector<unsigned char>& BinaryWord::get_bytes() const{
	return bytes;
}

Endianness BinaryWord::get_endianness() const{
	return endianness;
}

BinaryWord BinaryWord::bitw_or(BinaryWord const &lh, BinaryWord const &rh){
	if (lh.get_endianness() != rh.get_endianness()){
		throw runtime_error("Error at BinaryWord::OR operator: both operands must have equal endianness");
	}

	BinaryWord result((lh.to_int() | rh.to_int()) , lh.get_endianness());

	return result;
}

BinaryWord BinaryWord::bitw_xor(BinaryWord const &lh, BinaryWord const &rh){
	if (lh.get_endianness() != rh.get_endianness()){
		throw runtime_error("Error at BinaryWord::XOR operator: both operands must have equal endianness");
	}

	BinaryWord result((lh.to_int() ^ rh.to_int()) , lh.get_endianness());

	return result;
}

BinaryWord BinaryWord::bitw_and(BinaryWord const &lh, BinaryWord const &rh){
	if (lh.get_endianness() != rh.get_endianness()){
		throw runtime_error("Error at BinaryWord::XOR operator: both operands must have equal endianness");
	}

	BinaryWord result((lh.to_int() & rh.to_int()) , lh.get_endianness());

	return result;
}

BinaryWord BinaryWord::bitw_not(BinaryWord const &lh){
	BinaryWord result( ~lh.to_int(), lh.get_endianness());
	return result;
}
