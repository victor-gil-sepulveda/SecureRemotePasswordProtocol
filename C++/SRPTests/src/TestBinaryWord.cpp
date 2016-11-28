/*
 * TestBinaryWord.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#include <string>
#include <vector>
#include "../../SRP/src/sha/Utils.h"

using namespace std;

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Suites
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(AlgorithmUtilities)

BOOST_AUTO_TEST_CASE(to_hex_str)
{
	vector<unsigned char> bytes = {0x06, 0x76, 0x42, 0x23};
	string hola = "Hola";

	BOOST_CHECK_EQUAL(
			algorithms::utils::to_hex_str(bytes),
			"0x6 0x76 0x42 0x23"
	);

	BOOST_CHECK_EQUAL(
			algorithms::utils::to_hex_str(hola),
			"0x48 0x6f 0x6c 0x61"
	);
}

BOOST_AUTO_TEST_CASE(get_bytes)
{
	vector<unsigned char> bytes = algorithms::utils::get_bytes((long int) 591558150); // 0x23427606
	vector<unsigned char> expected_bytes = {0x06, 0x76, 0x42, 0x23};
	BOOST_CHECK_EQUAL_COLLECTIONS(
			bytes.begin(),bytes.end(),
			expected_bytes.begin(), expected_bytes.end());

	vector<unsigned char> hola_bytes = algorithms::utils::get_bytes("Hola");
	vector<unsigned char> hola_expected_bytes = {0x48, 0x6f, 0x6c, 0x61};
	BOOST_CHECK_EQUAL_COLLECTIONS(
			hola_bytes.begin(),hola_bytes.end(),
			hola_expected_bytes.begin(), hola_expected_bytes.end());
}

BOOST_AUTO_TEST_SUITE_END()

