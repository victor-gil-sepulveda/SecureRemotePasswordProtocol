/*
 * TestTools.h
 *
 *  Created on: Dec 2, 2016
 *      Author: victor
 */

#ifndef TESTTOOLS_H_
#define TESTTOOLS_H_

//#define VERBOSE_TESTS

#ifdef VERBOSE_TESTS
	#define IF_VERBOSE(x) (x)
#else
	#define IF_VERBOSE(x)
#endif


#endif /* TESTTOOLS_H_ */
