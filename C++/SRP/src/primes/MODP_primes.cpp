/*
 * MODP_primes.cpp
 *
 *  Created on: Dec 4, 2016
 *      Author: victor
 */

#include "MODP_primes.h"

#include <string>
#include <gmp.h>
#include <stdexcept>

using std::runtime_error;

mpz_t primes::generate_prime(const std::string& prime_str){
	// TODO: Check build options as a initial way to speed up the exe
	// https://gmplib.org/manual/Build-Options.html#Build-Options

	mpz_t prime;
	int op_result =  mpz_init_set_str (
			prime, // the guy
			prime_str.c_str(), //string
			16); // base

	if (op_result != 0){
		throw runtime_error("ERROR primes::generate_prime: The number could not be generated.");
	}

	return prime;
}
