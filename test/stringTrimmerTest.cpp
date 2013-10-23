// File:  stringTrimmerTest.cpp
// Date:  10/23/2013
// Auth:  K. Loux
// Desc:  Tests for string trimming routines.

// Google Test headers
#include <gtest/gtest.h>

// Local headers
#include "stringTrimmer.h"

TEST(StringTrimmerTest, TrimsSpacesFromRight)
{
	EXPECT_EQ(std::string("just this"), StringTrimmer::RightTrim("just this    "));
}

TEST(StringTrimmerTest, TrimsTabsFromRight)
{
	EXPECT_EQ(std::string("just this"), StringTrimmer::RightTrim("just this		"));
}

TEST(StringTrimmerTest, TrimsNewLinesFromRight)
{
	EXPECT_EQ(std::string("just this"), StringTrimmer::RightTrim("just this\r\n\r\n"));
}

TEST(StringTrimmerTest, TrimsSpacesFromLeft)
{
	EXPECT_EQ(std::string("just this"), StringTrimmer::LeftTrim("    just this"));
}

TEST(StringTrimmerTest, TrimsTabsFromLeft)
{
	EXPECT_EQ(std::string("just this"), StringTrimmer::LeftTrim("		just this"));
}

TEST(StringTrimmerTest, TrimsNewLinesFromLeft)
{
	EXPECT_EQ(std::string("just this"), StringTrimmer::LeftTrim("\r\n\r\njust this"));
}

//==========================================================================
// Class:			None
// Function:		main
//
// Description:		Application entry point.
//
// Input Arguments:
//		argc	= int
//		argv	= char*[]
//
// Output Arguments:
//		None
//
// Return Value:
//		SourceParser::PositionState
//
//==========================================================================
int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
#ifdef WIN32
	int res = RUN_ALL_TESTS();
	system("PAUSE");
	return res;
#else
	return RUN_ALL_TEST();
#endif
}