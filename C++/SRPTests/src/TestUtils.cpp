/*
 * TestBinaryWord.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#include <bitset>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "../../SRP/src/binary/tools/BinaryStringUtils.h"
#include "../../SRP/src/binary/tools/Utils.h"
#include "TestTools.h"
using namespace std;

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(AlgorithmUtilities)

BOOST_AUTO_TEST_CASE(to_hex_str)
{
	vector<unsigned char> bytes = {0x06, 0x76, 0x42, 0x23};
	string hola = "Hola";

	BOOST_CHECK_EQUAL(
			algorithms::utils::to_hex_str(bytes),
			"0x06 0x76 0x42 0x23"
	);

	BOOST_CHECK_EQUAL(
			algorithms::utils::to_hex_str(hola),
			"0x48 0x6f 0x6c 0x61"
	);
}

BOOST_AUTO_TEST_CASE(get_bytes)
{
	vector<unsigned char> bytes = algorithms::utils::get_bytes((std::uint32_t) 591558150); // 0x23427606
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
			"0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x28");
}

BOOST_AUTO_TEST_CASE(l_circular_shift)
{
	std::uint32_t val = 0x00000001;
	std::uint32_t result = algorithms::utils::circular_left_shift<1>(val);
	BOOST_CHECK_EQUAL(result, 2);
	IF_VERBOSE(algorithms::utils::print_32b_bin(val, result));

	val = 0x00000001;
	result = algorithms::utils::circular_left_shift<3>(val);
	BOOST_CHECK_EQUAL(result, 8);
	IF_VERBOSE(algorithms::utils::print_32b_bin(val, result));

	val = 0xFF000000;
	result = algorithms::utils::circular_left_shift<4>(val);
	BOOST_CHECK_EQUAL(result, 0xF000000F);
	IF_VERBOSE(algorithms::utils::print_32b_bin(val, result));

	val = 0xFF000000;
	result = algorithms::utils::circular_left_shift<8>(val);
	BOOST_CHECK_EQUAL(result, 0x000000FF);
	IF_VERBOSE(algorithms::utils::print_32b_bin(val, result));

	val = 0x81000000;
	result = algorithms::utils::circular_left_shift<7>(val);
	BOOST_CHECK_EQUAL(result, 0x80000040);
	IF_VERBOSE(algorithms::utils::print_32b_bin(val, result));

	// Non-trivial case
	val = 4023233417;
	uint32_t expected = 2079550178;
	result = algorithms::utils::circular_left_shift<30>(val);
	BOOST_CHECK_EQUAL(result, expected);
	IF_VERBOSE(algorithms::utils::print_32b_bin(val, result));
	IF_VERBOSE(algorithms::utils::print_32b_bin(expected, result));
}

BOOST_AUTO_TEST_CASE(r_circular_shift)
{
	std::uint32_t val = 0x00000002;
	std::uint32_t result = algorithms::utils::circular_right_shift<1>(val);
	BOOST_CHECK_EQUAL(result, 0x00000001);
	IF_VERBOSE(algorithms::utils::print_32b_bin(val, result));

	val = 0x00000008;
	result = algorithms::utils::circular_right_shift<3>(val);
	BOOST_CHECK_EQUAL(result, 0x00000001);
	IF_VERBOSE(algorithms::utils::print_32b_bin(val, result));

	val = 0xF000000F;
	result = algorithms::utils::circular_right_shift<4>(val);
	BOOST_CHECK_EQUAL(result, 0xFF000000);
	IF_VERBOSE(algorithms::utils::print_32b_bin(val, result));

	val = 0x000000FF;
	result = algorithms::utils::circular_right_shift<8>(val);
	BOOST_CHECK_EQUAL(result, 0xFF000000);
	IF_VERBOSE(algorithms::utils::print_32b_bin(val, result));

	val = 0x80000040;
	result = algorithms::utils::circular_right_shift<7>(val);
	BOOST_CHECK_EQUAL(result, 0x81000000);
	IF_VERBOSE(algorithms::utils::print_32b_bin(val, result));

	// Non-trivial case
	val = 2079550178;
	uint32_t expected = 4023233417;
	result = algorithms::utils::circular_right_shift<30>(val);
	BOOST_CHECK_EQUAL(result, expected);
	IF_VERBOSE(algorithms::utils::print_32b_bin(val, result));
	IF_VERBOSE(algorithms::utils::print_32b_bin(expected, result));
}

BOOST_AUTO_TEST_SUITE_END()

