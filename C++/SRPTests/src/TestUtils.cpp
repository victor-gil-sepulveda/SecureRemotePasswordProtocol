/*
 * TestBinaryWord.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "../../SRP/src/sha/BinaryStringUtils.h"
#include "../../SRP/src/sha/Utils.h"
using namespace std;

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

	BOOST_CHECK_THROW(
			algorithms::utils::get_bytes("Hola Mundo"),
			runtime_error);
}

BOOST_AUTO_TEST_CASE(to_int)
{
	vector<unsigned char> bytes = {0x06, 0x76, 0x42, 0x23};
	BOOST_CHECK_EQUAL(
			algorithms::utils::bytes_to_int(bytes),
			(int) 591558150);
}

BOOST_AUTO_TEST_CASE(pad_with_zeros)
{
	// From https://tools.ietf.org/html/rfc3174
	// original message :01100001 01100010 01100011 01100100 01100101 ("abcde")
	// final:
	// 61626364 65000000 00000000 00000000
    // 00000000 00000000 00000000 00000000
    // 00000000 00000000 00000000 00000000
    // 00000000 00000000

	string message = "abcde";
	string result = algorithms::utils::pad_with_character(message, 'x', 64, 48);

	BOOST_CHECK_EQUAL(
			result.size(),
			48);

	BOOST_CHECK_EQUAL(
			result,
			"abcdexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

	message = "abcdexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // size > 48
	result = algorithms::utils::pad_with_character(message, 'x', 64, 48);
	BOOST_CHECK_EQUAL(
				result.size()%64,
				48);

}

BOOST_AUTO_TEST_CASE(add_message_length)
{
	// From https://tools.ietf.org/html/rfc3174
	//
	// Original message :
	// 61626364 65000000 00000000 00000000
	// 00000000 00000000 00000000 00000000
	// 00000000 00000000 00000000 00000000
	// 00000000 00000000
	//
	// Final:
	// 61626364 65000000 00000000 00000000
    // 00000000 00000000 00000000 00000000
    // 00000000 00000000 00000000 00000000
    // 00000000 00000000 00000000 00000028
	//
	// length is 0x28 = 40
	string message = "abcde";
	string result = algorithms::utils::length_to_string_64b(message.size()*8);
	BOOST_CHECK_EQUAL(
			result.size(),
			8);

	BOOST_CHECK_EQUAL(
			algorithms::utils::to_hex_str(result),
			"0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x28");
}



BOOST_AUTO_TEST_SUITE_END()

