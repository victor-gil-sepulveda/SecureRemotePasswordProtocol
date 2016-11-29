/*
 * BinaryString.h
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#ifndef SHA_BINARYSTRING_H_
#define SHA_BINARYSTRING_H_

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

		void append_one(){};

		BinaryWord* get_word_at(int position){};

	private:
		BinaryString();
		static void get_bin_words_from_string(std::string s){};
		std::vector<BinaryWord*> words;
		std::string message;
};

#endif /* SHA_BINARYSTRING_H_ */
