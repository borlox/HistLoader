#include <sstream>

#define BOOST_TEST_DYN_LINK 1

#define BOOST_TEST_MODULE HistLoaderUnitTests
#include <boost/test/unit_test.hpp>

#include "HistLoader.h"

BOOST_AUTO_TEST_CASE(Internal_StartsWithFollowedBy)
{
	extern bool StartsWithFollowedBy(const std::string& str, const std::string& withWhat, const std::string& byWhat);

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

BOOST_AUTO_TEST_SUITE(ParserErrors)

BOOST_AUTO_TEST_CASE(TemplateNoName)
{
	std::stringstream sstr(
		"template \n"
		"name\n"
	);
	HistLoader ld;
	BOOST_CHECK_THROW(ld.LoadFromStream(sstr), HistLoader::ParserError);
}

BOOST_AUTO_TEST_CASE(RangeNoName)
{
	std::stringstream sstr(
		"range(5) \n"
		"name\n"
	);
	HistLoader ld;
	BOOST_CHECK_THROW(ld.LoadFromStream(sstr), HistLoader::ParserError);
}

BOOST_AUTO_TEST_CASE(TemplateAndRange)
{
	std::stringstream sstr(
		"template range(5) name\n"
		"\tattr: val\n"
	);
	HistLoader ld;
	BOOST_CHECK_THROW(ld.LoadFromStream(sstr), HistLoader::ParserError);
}

BOOST_AUTO_TEST_CASE(NoName)
{
	std::stringstream sstr(
		" : p1, p2\n"
		"\tattr1: val1\n"
	);
	HistLoader ld;
	BOOST_CHECK_THROW(ld.LoadFromStream(sstr), HistLoader::ParserError);
}

BOOST_AUTO_TEST_SUITE_END()
