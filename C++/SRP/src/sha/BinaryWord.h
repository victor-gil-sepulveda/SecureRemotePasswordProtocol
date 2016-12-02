/*
 * BinaryWord.h
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#ifndef SHA_BINARYWORD_H_
#define SHA_BINARYWORD_H_

#include <cstdint>
#include <string>
#include <vector>

#include "Endianness.h"
/*
 * Immutable object (this was more a challenge than a spec) that stores a 32b word.
 * Inmutability implies to mess with constness.
 *
 * TODO: Clearly endianness could be some sort of strategy class instead of enum+cases. This
 * is indeed a possible future refactoring.
 *
 * TODO: use std:: defined "known sizes" for unsigned char
 */
class BinaryWord {
	public:
		BinaryWord(std::uint32_t value, Endianness e);
		BinaryWord(std::string value, Endianness e);
		BinaryWord(const BinaryWord& b);
		virtual ~BinaryWord();

		std::string to_string() const;
		std::uint32_t to_int() const;

		const std::vector<unsigned char>& get_bytes() const;
		Endianness get_endianness() const;

		static BinaryWord bitw_or(BinaryWord const &lh, BinaryWord const &rh);
		friend BinaryWord operator|(BinaryWord const &lh, BinaryWord const &rh);
		static BinaryWord bitw_xor(BinaryWord const &lh, BinaryWord const &rh);
		friend BinaryWord operator^(BinaryWord const &lh, BinaryWord const &rh);
		static BinaryWord bitw_and(BinaryWord const &lh, BinaryWord const &rh);
		friend BinaryWord operator&(BinaryWord const &lh, BinaryWord const &rh);
		static BinaryWord bitw_not(BinaryWord const &rh);
		friend BinaryWord operator~(BinaryWord const &rh);

	private:
		BinaryWord();

		std::vector<unsigned char> bytes;
		Endianness endianness;

		void assign_bytes(std::vector<unsigned char>& word_bytes);

};

#endif /* SHA_BINARYWORD_H_ */
