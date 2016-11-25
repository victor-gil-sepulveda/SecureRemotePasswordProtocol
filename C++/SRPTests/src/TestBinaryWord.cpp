/*
 * TestBinaryWord.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: victor
 */

#include <iostream>
#include <vector>
#include "../../SRP/src/sha/Utils.h"
using namespace std;

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Suites
#include <boost/test/unit_test.hpp>

int add(int i, int j)
{
    return i + j;
}

BOOST_AUTO_TEST_SUITE(AlgorithmUtilities)

BOOST_AUTO_TEST_CASE(get_bytes)
{
	vector<unsigned char> bytes = algorithms::utils::get_bytes((long int)591558150); // 0x23427606
	vector<unsigned char> expected_bytes = {0x06, 0x76, 0x42, 0x23};
	cout<<expected_bytes.size()<<" "<<bytes.size()
		<<" "<<algorithms::utils::char_vector_to_hex_str(bytes)
		<<" "<<algorithms::utils::char_vector_to_hex_str(expected_bytes)
	<<endl;

	BOOST_CHECK_EQUAL_COLLECTIONS(
			bytes.begin(),bytes.end(),
			expected_bytes.begin(), expected_bytes.end());
}

BOOST_AUTO_TEST_SUITE_END()

