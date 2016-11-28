/*
 * BinaryWord.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#include "BinaryWord.h"

#include <vector>
#include <algorithm>

#include "Utils.h"
using std::vector;

//TODO use std:: defined "known sizes"

BinaryWord::BinaryWord():endianness(Endianness::LITTLE){}

BinaryWord::BinaryWord(long int value, Endianness e): endianness(e) {
	this->bytes.resize(4);
	vector<unsigned char> word_bytes = algorithms::utils::get_bytes(value);
	assign_bytes(word_bytes, this->endianness);
}

BinaryWord::BinaryWord(std::string value, Endianness e): endianness(e){
	this->bytes.resize(4);
	vector<unsigned char> word_bytes = algorithms::utils::get_bytes(value);
	assign_bytes(word_bytes, this->endianness);
}

void BinaryWord::assign_bytes(vector<unsigned char>& word_bytes, Endianness e){
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
		{}
	}
}

BinaryWord::~BinaryWord() {}

std::string BinaryWord::to_string(){
	std::string result = "    ";
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
		{}
	}
	return result;
}

const std::vector<unsigned char>& BinaryWord::get_bytes(){
	return bytes;
}


