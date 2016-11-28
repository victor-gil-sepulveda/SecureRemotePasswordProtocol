/*
 * BinaryWord.h
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#ifndef SHA_BINARYWORD_H_
#define SHA_BINARYWORD_H_

#include <string>
#include <vector>

enum class Endianness {LITTLE, BIG};

/*
 * Immutable object that stores a 32b word.
 */
class BinaryWord {
	public:
		BinaryWord(long int value, Endianness e);
		BinaryWord(std::string value, Endianness e);
		virtual ~BinaryWord();

		std::string toString();

		//BinaryWord or()
		//BinaryWord xor()

	private:
		std::vector<unsigned char> bytes;
		Endianness endianness;

		BinaryWord();

		void assign_bytes(std::vector<unsigned char>& word_bytes, Endianness e);

};

#endif /* SHA_BINARYWORD_H_ */
