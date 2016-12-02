/*
 * BinaryString.h
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#ifndef SHA_BINARYSTRING_H_
#define SHA_BINARYSTRING_H_

#include <cstdint>
#include <vector>
#include <string>

class BinaryWord;

/*
 * This class holds the string that is going to be converted to sha1.
 *
 */
class BinaryString {
	public:
		BinaryString(std::string const & initial_message);
		virtual ~BinaryString();

		BinaryWord* get_32b_word_at(int chunk_nr, int word_nr);

		void get_words_from_chunk(unsigned int chunk_nr, std::vector<BinaryWord*>& out);
		void get_words_from_chunk(unsigned int chunk_nr, std::vector<std::uint32_t>& out);

		virtual unsigned int get_num_512b_chunks() const;

	private:
		BinaryString();

		std::vector<BinaryWord*> words;
		std::vector<std::uint32_t> uint_words;
		unsigned int num_512b_chunks;
};

#endif /* SHA_BINARYSTRING_H_ */
