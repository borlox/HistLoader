#define BOOST_TEST_DYN_LINK 1

#define BOOST_TEST_MODULE HistLoaderUnitTests
#include <boost/test/unit_test.hpp>

#include "HistLoader.h"

BOOST_AUTO_TEST_CASE(test_StartsWithFollowedBy)
{
	bool StartsWithFollowedBy(const std::string& str, const std::string& withWhat, const std::string& byWhat);

	BOOST_CHECK(
		StartsWithFollowedBy("test more stuff", "test", " ")
	);
	BOOST_CHECK(
		StartsWithFollowedBy("test more stuff", "test", "abc") == false
	);

	BOOST_CHECK(
		StartsWithFollowedBy("templateName", "template", " \t") == false
	);

	BOOST_CHECK(
		StartsWithFollowedBy("range    (12345)\nblubb", "range", " \t(")
	);
}
