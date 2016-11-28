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
 * Immutable object (this was more a challenge than a spec) that stores a 32b word.
 * Inmutability implies to mess with constness.
 * Clearly endianness could be some sort of strategy class instead of enum+cases. This
 * is indeed a possible future refactoring.
 *
 * TODO: use std:: defined "known sizes"
 */
class BinaryWord {
	public:
		BinaryWord(unsigned long int value, Endianness e);
		BinaryWord(std::string value, Endianness e);
		BinaryWord(const BinaryWord& b);
		virtual ~BinaryWord();

		std::string to_string() const;
		unsigned long int to_int() const;

		const std::vector<unsigned char>& get_bytes() const;
		Endianness get_endianness() const;

		static BinaryWord bitw_or(BinaryWord const &lh, BinaryWord const &rh);

		BinaryWord& operator=(BinaryWord rh);

		//BinaryWord xor()

	private:
		BinaryWord();

		std::vector<unsigned char> bytes;
		Endianness endianness;

		void assign_bytes(std::vector<unsigned char>& word_bytes);

};

#endif /* SHA_BINARYWORD_H_ */
