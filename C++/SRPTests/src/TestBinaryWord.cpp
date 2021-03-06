/*
 * TestBinaryWord.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#include <string>
#include <vector>
#include <iostream>

#include "../../SRP/src/binary/BinaryWord.h"
using namespace std;

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(BinaryWordTest)

BOOST_AUTO_TEST_CASE(BinaryWordConstructor)
{
	vector<unsigned char> expected_bytes = {0x06, 0x76, 0x42, 0x23};
	BinaryWord w1(591558150, Endianness::LITTLE);
	const vector<unsigned char>& bytes = w1.get_bytes();

	BOOST_CHECK_EQUAL_COLLECTIONS(
			bytes.begin(),bytes.end(),
			expected_bytes.begin(), expected_bytes.end());

	vector<unsigned char> hola_expected_bytes = {0x48, 0x6f, 0x6c, 0x61};
	BinaryWord w2("Hola", Endianness::LITTLE);
	const vector<unsigned char>& bytes2 = w2.get_bytes();
	BOOST_CHECK_EQUAL_COLLECTIONS(
			bytes2.begin(),bytes2.end(),
			hola_expected_bytes.begin(), hola_expected_bytes.end());

	vector<unsigned char> hola_expected_bytes_big_endian = {0x61, 0x6c, 0x6f, 0x48};
	BinaryWord w3("Hola", Endianness::BIG);
	const vector<unsigned char>& bytes3 = w3.get_bytes();
	BOOST_CHECK_EQUAL_COLLECTIONS(
			bytes3.begin(),bytes3.end(),
			hola_expected_bytes_big_endian.begin(), hola_expected_bytes_big_endian.end());

	// Copy constructor
	BinaryWord w4(w3);
	const vector<unsigned char>& bytes4 = w4.get_bytes();
	BOOST_CHECK_EQUAL_COLLECTIONS(
			bytes4.begin(),bytes4.end(),
			bytes3.begin(),bytes3.end());
}

BOOST_AUTO_TEST_CASE(to_string)
{
	BinaryWord w1("Hola", Endianness::LITTLE);
	BOOST_CHECK_EQUAL(
			w1.to_string(),
			"Hola"
	);

	BinaryWord w2("Hola", Endianness::BIG);
	BOOST_CHECK_EQUAL(
			w2.to_string(),
			"aloH"
	);
}

BOOST_AUTO_TEST_CASE(logic_operators)
{
	BinaryWord w1(0xFF00FF00, Endianness::LITTLE);
	BinaryWord w2(0x35672304, Endianness::LITTLE);
	BinaryWord result = BinaryWord::bitw_or(w1, w2);
	BOOST_CHECK_EQUAL(
			result.to_int(),
			0xFF67FF04
	);

	result = BinaryWord::bitw_and(w1, w2);
	BOOST_CHECK_EQUAL(
			result.to_int(),
			0x35002300
	);

	result = BinaryWord::bitw_xor(w1, w2);
	BOOST_CHECK_EQUAL(
			result.to_int(),
			0xca67dc04
	);

	result = BinaryWord::bitw_not(w1);
	BOOST_CHECK_EQUAL(
			result.to_int(),
			0x00FF00FF
	);

	result = w1 | w2;
	BOOST_CHECK_EQUAL(
			result.to_int(),
			0xFF67FF04
	);

	result = w1 & w2;
	BOOST_CHECK_EQUAL(
			result.to_int(),
			0x35002300
	);

	result = w1 ^ w2;
	BOOST_CHECK_EQUAL(
			result.to_int(),
			0xca67dc04
	);

	result = ~w1;
	BOOST_CHECK_EQUAL(
			result.to_int(),
			0x00FF00FF
	);
}

BOOST_AUTO_TEST_SUITE_END()

