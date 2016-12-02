/*
 * TestSha1.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: victor
 */

#include <cstdint>
#include <iostream>
#include <string>

#include "../../SRP/src/sha/Sha1.h"
#include "../../SRP/src/sha/Utils.h"
using namespace std;

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Sha1Test)

BOOST_AUTO_TEST_CASE(message_preprocessing)
{
	string initial_message = "Hola!";
	string expected = "0x48 0x6f 0x6c 0x61 0x21 0x80 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x28";
	string result = algorithms::Sha1::preprocess_message(initial_message);

	BOOST_CHECK_EQUAL(algorithms::utils::to_hex_str(result), expected);
}

BOOST_AUTO_TEST_CASE(word_array_init)
{
	vector<std::uint32_t> expected_words = {1215261793, 562036736, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 40, 2430523586, 1124073472, 80, 566079877, 2248146944, 160, 1132159754,
			201326673, 566079685, 16172564, 402653186, 640, 233671721, 805306644, 2806739601,
			2245728416, 46584455, 2315258369, 934687204, 1190576708, 2234370628, 392978434,
			116835893, 402663746, 1479410820, 402657601, 1958160666, 1571916456, 1863732090,
			2885722457, 4254456087, 4053459250, 2193395069, 1388707878, 1869375248, 489638695,
			2081853019, 2550210067, 2093261059, 454602453, 1816755680, 3613816216, 3752548170,
			28195437, 1944507228, 878711396, 75996680, 2537896251, 1811090387, 940654883, 3072921204,
			3739666709, 2521528910, 10574252, 2514032637, 2598449265, 2975920555, 4127208022, 51082279,
			3029344622, 777259672, 1092488617, 2625324688, 935495138};

	string initial_message = "Hola!";
	string preprocessed_message = algorithms::Sha1::preprocess_message(initial_message);
	BinaryString bs (preprocessed_message); // ************** TEST BIN STRING
	vector<uint32_t> words;
	algorithms::Sha1::word_array_init(0, bs, words);

	BOOST_CHECK_EQUAL_COLLECTIONS(
			words.begin(),
			words.end(),
			expected_words.begin(),
			expected_words.end());
}



BOOST_AUTO_TEST_SUITE_END()
