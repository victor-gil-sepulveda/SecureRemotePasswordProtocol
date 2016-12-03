/*
 * TestSha256.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: victor
 */

#include <string>
using namespace std;

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Sha256Test)

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
