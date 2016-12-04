/*
 * TestSha256.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: victor
 */

#include <cstdint>
#include <string>
#include <vector>

#include "../../SRP/src/algorithms/Base64.h"
#include "../../SRP/src/algorithms/sha/Sha1.h"
#include "../../SRP/src/algorithms/sha/Sha256.h"
using namespace std;

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Sha256Test)

BOOST_AUTO_TEST_CASE(Sha256_word_array_init)
{
	vector<std::uint32_t> expected_words = {1215261793, 562036736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 1289909441,
			563150848, 1961604976, 2684907706, 2907377157, 912393753, 3874042359, 2426706493, 3767383525,
			1399392976, 209641460, 1366427552, 3932790304, 706326541, 3718089824, 3838920057, 1548196883, 3152254801, 2171226704,
			558939600, 4174221683, 482795789, 2518607333, 1412835463, 861745984, 75745173, 955848436, 545944981, 602895418,
			3988351662, 1148474959, 2057103765, 1420465797, 41885936, 1184409823, 1448847361, 3385786536, 1659294552,
			4282276324, 664622208, 4182360699, 4140449705, 467051373, 4020938244, 662439865, 2022078611, 251137381, 214561204};

	string initial_message = "Hola!";
	string preprocessed_message = algorithms::Sha1::preprocess_message(initial_message);
	BinaryString bs (preprocessed_message); // ************** TODO: TEST BIN STRING
	vector<uint32_t> words;
	algorithms::Sha256::word_array_init(0, bs, words);

	BOOST_CHECK_EQUAL_COLLECTIONS(
			words.begin(),
			words.end(),
			expected_words.begin(),
			expected_words.end());
}

BOOST_AUTO_TEST_CASE(Sha256_main_loop)
{
	vector<std::uint32_t> words = {1215261793, 562036736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 1289909441,
		563150848, 1961604976, 2684907706, 2907377157, 912393753, 3874042359, 2426706493, 3767383525,
		1399392976, 209641460, 1366427552, 3932790304, 706326541, 3718089824, 3838920057, 1548196883, 3152254801, 2171226704,
		558939600, 4174221683, 482795789, 2518607333, 1412835463, 861745984, 75745173, 955848436, 545944981, 602895418,
		3988351662, 1148474959, 2057103765, 1420465797, 41885936, 1184409823, 1448847361, 3385786536, 1659294552,
		4282276324, 664622208, 4182360699, 4140449705, 467051373, 4020938244, 662439865, 2022078611, 251137381, 214561204};

	vector<std::uint32_t> hs = {
			algorithms::Sha256::InitialHashValues::H0,
			algorithms::Sha256::InitialHashValues::H1,
			algorithms::Sha256::InitialHashValues::H2,
			algorithms::Sha256::InitialHashValues::H3,
			algorithms::Sha256::InitialHashValues::H4,
			algorithms::Sha256::InitialHashValues::H5,
			algorithms::Sha256::InitialHashValues::H6,
			algorithms::Sha256::InitialHashValues::H7
	};

	vector<std::uint32_t> expected_hs = {801506038, 1138488498, 2414180054, 3505073283, 2719467197, 1225944146, 1446775884, 2635859266};

	algorithms::Sha256::main_loop(hs[0], hs[1], hs[2], hs[3], hs[4], hs[5], hs[6], hs[7], words);

	BOOST_CHECK_EQUAL_COLLECTIONS(
			hs.begin(),
			hs.end(),
			expected_hs.begin(),
			expected_hs.end());
}

BOOST_AUTO_TEST_CASE(full_sha256)
{
	// base64.b64encode(hashlib.sha256("Hola!").digest())
	string hash = algorithms::Sha256::calculate_hash(std::string("Hola!"));
	string expected_b64_hash = "L8YC9kPb9LKP5XbW0Ossg6IXxr1JEmxSVjwMTJ0cBUI=";
	string b64_hash = algorithms::Base64::base64_encode((unsigned char*) hash.c_str(), hash.size());
	BOOST_CHECK_EQUAL(b64_hash, expected_b64_hash);

	hash = algorithms::Sha256::calculate_hash(std::string("Adios!"));
	expected_b64_hash = "ns0Bb3yUyzVoY2bZfIWyshH5R4I8T95gfwK65dSDoKo=";
	b64_hash = algorithms::Base64::base64_encode((unsigned char*) hash.c_str(), hash.size());
	BOOST_CHECK_EQUAL(b64_hash, expected_b64_hash);

	hash = algorithms::Sha256::calculate_hash(std::string(
			"This is a message using more than 512b (64 characters). The goal here"
			" is to force the algorithm to process more than one 512b chunk"));
	expected_b64_hash = "ojTxj+TJcM/mQeFEWhA/DhDrT567F+2++KDXnrkfR9I=";
	b64_hash = algorithms::Base64::base64_encode((unsigned char*) hash.c_str(), hash.size());
	BOOST_CHECK_EQUAL(b64_hash, expected_b64_hash);
}

BOOST_AUTO_TEST_SUITE_END()
