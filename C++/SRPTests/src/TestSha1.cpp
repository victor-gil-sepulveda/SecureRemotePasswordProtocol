/*
 * TestSha1.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: victor
 */

#include <cstdint>
#include <iostream>
#include <string>

#include "../../SRP/src/algorithms/Base64.h"
#include "../../SRP/src/algorithms/sha/Sha1.h"
#include "../../SRP/src/binary/tools/Utils.h"
using namespace std;

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Sha1Test)

/*
 * The intermediate test output data obatined using the implementation in
 * https://www.ipa.go.jp/security/rfc/RFC3174EN.html
 */

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
	BinaryString bs (preprocessed_message); // ************** TODO: TEST BIN STRING
	vector<uint32_t> words;
	algorithms::Sha1::word_array_init(0, bs, words);

	BOOST_CHECK_EQUAL_COLLECTIONS(
			words.begin(),
			words.end(),
			expected_words.begin(),
			expected_words.end());
}

BOOST_AUTO_TEST_CASE(main_loop)
{
	vector<std::uint32_t> words = {1215261793, 562036736, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 40, 2430523586, 1124073472, 80, 566079877, 2248146944, 160, 1132159754,
				201326673, 566079685, 16172564, 402653186, 640, 233671721, 805306644, 2806739601,
				2245728416, 46584455, 2315258369, 934687204, 1190576708, 2234370628, 392978434,
				116835893, 402663746, 1479410820, 402657601, 1958160666, 1571916456, 1863732090,
				2885722457, 4254456087, 4053459250, 2193395069, 1388707878, 1869375248, 489638695,
				2081853019, 2550210067, 2093261059, 454602453, 1816755680, 3613816216, 3752548170,
				28195437, 1944507228, 878711396, 75996680, 2537896251, 1811090387, 940654883, 3072921204,
				3739666709, 2521528910, 10574252, 2514032637, 2598449265, 2975920555, 4127208022, 51082279,
				3029344622, 777259672, 1092488617, 2625324688, 935495138};

	vector<std::uint32_t> hs = {
			algorithms::Sha1::InitialHashValues::H0,
			algorithms::Sha1::InitialHashValues::H1,
			algorithms::Sha1::InitialHashValues::H2,
			algorithms::Sha1::InitialHashValues::H3,
			algorithms::Sha1::InitialHashValues::H4
	};

	vector<std::uint32_t> expected_hs = {4131966402, 3471270902, 3415246752, 1476864369, 764617998};

	algorithms::Sha1::main_loop(hs[0], hs[1], hs[2], hs[3], hs[4],	words);

	BOOST_CHECK_EQUAL_COLLECTIONS(
			hs.begin(),
			hs.end(),
			expected_hs.begin(),
			expected_hs.end());
}

BOOST_AUTO_TEST_CASE(full_sha1)
{
	// Expected results obtained with this Python line:
	// base64.b64encode(hashlib.sha1("<TEXT>").digest())

	string hash = algorithms::Sha1::calculate_hash(std::string("Hola!"));
	string expected_b64_hash = "9kjNws7nY/bLkIegWAcpcS2TJQ4=";
	string b64_hash = algorithms::Base64::base64_encode((unsigned char*) hash.c_str(), hash.size());
	BOOST_CHECK_EQUAL(b64_hash, expected_b64_hash);

	hash = algorithms::Sha1::calculate_hash(std::string("Adios!"));
	expected_b64_hash = "c8wqZwVik9pzoVh2AVw6RfFzMk4=";
	b64_hash = algorithms::Base64::base64_encode((unsigned char*) hash.c_str(), hash.size());
	BOOST_CHECK_EQUAL(b64_hash, expected_b64_hash);

	hash = algorithms::Sha1::calculate_hash(std::string(
			"This is a message using more than 512b (64 characters). The goal here"
			" is to force the algorithm to process more than one 512b chunk"));
	expected_b64_hash = "16Yeg95nRpf8C6Z/3KK7aU6Om0U=";
	b64_hash = algorithms::Base64::base64_encode((unsigned char*) hash.c_str(), hash.size());
	BOOST_CHECK_EQUAL(b64_hash, expected_b64_hash);
}

BOOST_AUTO_TEST_SUITE_END()
