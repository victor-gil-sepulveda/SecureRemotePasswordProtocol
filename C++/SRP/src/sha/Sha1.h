/*
 * Sha1.h
 *
 * Calculates the Sha1 of a given variable following the pseudocode in:
 *
 * https://en.wikipedia.org/wiki/SHA-1
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#ifndef SHA_SHA1_H_
#define SHA_SHA1_H_
#include <string>

namespace algorithms {

class Sha1 {
	public:
		Sha1();
		virtual ~Sha1();

		std::string calculate(std::string message);


};

} /* namespace algorithms */

#endif /* SHA_SHA1_H_ */
