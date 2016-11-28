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
 * Immutable object (<- this is the challenge) that stores a 32b word.
 */
class BinaryWord {
	public:
		BinaryWord(long int value, Endianness e);
		BinaryWord(std::string value, Endianness e);
		BinaryWord(BinaryWord& b);
		virtual ~BinaryWord();

		std::string to_string();
		std::string to_int();
		const std::vector<unsigned char>& get_bytes();
		Endianness get_endianness();

		//BinaryWord or()
		BinaryWord operator|(BinaryWord& rh);

		//BinaryWord xor()

	private:
		std::vector<unsigned char> bytes;
		Endianness endianness;

		BinaryWord();

		void assign_bytes(std::vector<unsigned char>& word_bytes);

};

#endif /* SHA_BINARYWORD_H_ */
