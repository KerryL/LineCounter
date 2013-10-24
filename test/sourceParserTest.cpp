// File:  sourceParserTest.cpp
// Date:  10/23/2013
// Auth:  K. Loux
// Desc:  Tests for source parser class.

// Google Test headers
#include <gtest/gtest.h>

// Local headers
#include "sourceParser.h"

class SourceParserCppStyleTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		std::vector<std::string> comments(1, "//");
		std::vector<std::pair<std::string, std::string> > blockComments;
		blockComments.push_back(std::make_pair(std::string("/*"), std::string("*/")));
		std::vector<std::string> lineContinuation(1, "\\");
		sp = new SourceParser(comments, blockComments, lineContinuation);
	};

	virtual void TearDown() { delete sp; };

	SourceParser *sp;
};

TEST_F(SourceParserCppStyleTest, IgnoresInlineBlockComments)
{
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"this->LooksLike(\"/*valid C++ code*/\");"));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"/*this->*/LooksLike(\"valid C++ code\");"));
}

TEST_F(SourceParserCppStyleTest, IgnoresMutipleInlineBlockComments)
{
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"this->Looks/*Like*/(\"/*valid C++ code*/\");"));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"/*this->*/LooksLike(\"/*valid C++ code*/\");"));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"/*this->*/LooksLike(\"valid C++ code\");/*with something at the end*/"));
}

TEST_F(SourceParserCppStyleTest, IgnoresEndofLineComments)
{
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"this->LooksLike(\"valid C++ code\");// with a comment at the end"));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"this->LooksLike(\"code\");/*With a comment at the end*/"));
}

TEST_F(SourceParserCppStyleTest, FindsSingleLineComments)
{
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"//this->LooksLike(\"valid C++ code\");"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"//this/*->LooksLike*/(\"valid C++ code\");"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"/*this->LooksLike(\"valid C++ code\");*/"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"    //this/*->LooksLike*/(\"valid C++ code\");"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"    /*this->LooksLike(\"valid C++ code\");*/"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"	//this/*->LooksLike*/(\"valid C++ code\");"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"	/*this->LooksLike(\"valid C++ code\");*/"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"//this/*->LooksLike*/(\"valid C++ code\");    "));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"/*this->LooksLike(\"valid C++ code\");*/    "));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"//this/*->LooksLike*/(\"valid C++ code\");	"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"/*this->LooksLike(\"valid C++ code\");*/	"));
}

TEST_F(SourceParserCppStyleTest, HandlesBlockComments)
{
	EXPECT_EQ(SourceParser::PositionBlockComment, sp->ParseLine(
		"/*Here we start a block comment"));
	EXPECT_EQ(SourceParser::PositionBlockComment, sp->ParseLine(
		"still in the comment"));
	EXPECT_EQ(SourceParser::PositionBlockComment, sp->ParseLine(
		"/* Re-starting, but it doesn't matter"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"now we're ending the comment*/"));
}

TEST_F(SourceParserCppStyleTest, HandlesConsecutiveBlockComments)
{
	EXPECT_EQ(SourceParser::PositionBlockComment, sp->ParseLine(
		"/*Here we start a block comment"));
	EXPECT_EQ(SourceParser::PositionBlockComment, sp->ParseLine(
		"now we're ending the comment*//*but starting a new one"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"and now we end it!*/"));

	EXPECT_EQ(SourceParser::PositionBlockComment, sp->ParseLine(
		"/*Here we start a block comment"));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"now we're ending the comment*/Code.InTheMiddle();/*but starting a new one"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"and now we end it!*/"));

	EXPECT_EQ(SourceParser::PositionBlockComment, sp->ParseLine(
		"/*Here we start a block comment"));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"now we're ending the comment*/Code./*InThe*/Middle();/*but starting a new one"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"and now we end it!*/"));
}

TEST_F(SourceParserCppStyleTest, IgnoresCommentedBlockCommentStart)
{
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"/*This is just /* a single line comment*/"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"//This is just /* a single line comment, too"));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"i++;/*This is just /* a single line comment*/"));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"do.Something();//This is just /* a single line comment, too"));
}

TEST_F(SourceParserCppStyleTest, FindsNormalCode)
{
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"nothing.SpecialGoingOn();"));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"i++;"));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"   ignoreTheWhitespace++;    "));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"   ignoreTheWhitespace++;    \\ "));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"   ignoreTheWhitespace++;    \\"));
}

TEST_F(SourceParserCppStyleTest, FindsWhiteSpaceOutsideComment)
{
	EXPECT_EQ(SourceParser::PositionWhitespace, sp->ParseLine(
		"     "));
	EXPECT_EQ(SourceParser::PositionWhitespace, sp->ParseLine(
		"	"));
	EXPECT_EQ(SourceParser::PositionWhitespace, sp->ParseLine(
		""));
}

TEST_F(SourceParserCppStyleTest, FindsWhiteSpaceInsideComment)
{
	EXPECT_EQ(SourceParser::PositionBlockComment, sp->ParseLine(
		"/*Here we start a block comment"));
	EXPECT_EQ(SourceParser::PositionWhitespace, sp->ParseLine(
		"     "));
	EXPECT_EQ(SourceParser::PositionWhitespace, sp->ParseLine(
		"	"));
	EXPECT_EQ(SourceParser::PositionWhitespace, sp->ParseLine(
		""));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"now we're ending the comment*/"));
}

TEST_F(SourceParserCppStyleTest, HandlesCommentContinuation)
{
	EXPECT_EQ(SourceParser::PositionContinuingComment, sp->ParseLine(
		"//Here we start comment \\"));
	EXPECT_EQ(SourceParser::PositionContinuingComment, sp->ParseLine(
		"and continue it onto the next line \\"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"and even a third line"));

	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"//whitespace at the end means we don't continue \\ "));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"SoThisLine.ShouldBeCode();"));

	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"This.Line(\"is code\");// but the next line \\"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"should be a comment"));

	EXPECT_EQ(SourceParser::PositionContinuingComment, sp->ParseLine(
		"/*This.Line(\"is a comment\");*/// and so the next line \\"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"should be a comment, too"));

	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"/*This.Line(\"is code\");*/Include.ThisCode();// but the next line \\"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"should be a comment"));

	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"/*This.Line(\"is code\");*/Include.ThisCode();// but the next line \\"));
	EXPECT_EQ(SourceParser::PositionContinuingComment, sp->ParseLine(
		"should be a comment \\"));
	EXPECT_EQ(SourceParser::PositionComment, sp->ParseLine(
		"and so should this line"));

	EXPECT_EQ(SourceParser::PositionContinuingComment, sp->ParseLine(
		"// If we have only whitespace on the next line, it's not a comment \\"));
	EXPECT_EQ(SourceParser::PositionWhitespace, sp->ParseLine(
		"   "));
	EXPECT_EQ(SourceParser::PositionCode, sp->ParseLine(
		"backToRegularCode();"));
}

class SourceParserMultiCommentTokenTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		std::vector<std::string> comments;
		comments.push_back("//");
		comments.push_back("'");
		comments.push_back("#");
		std::vector<std::pair<std::string, std::string> > blockComments;
		blockComments.push_back(std::make_pair(std::string("/*"), std::string("*/")));
		blockComments.push_back(std::make_pair(std::string("<!--"), std::string("-->")));
		std::vector<std::string> lineContinuation(1, "\\");
		sp = new SourceParser(comments, blockComments, lineContinuation);
	};

	virtual void TearDown() { delete sp; };

	SourceParser *sp;
};

// TODO:  More testing

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
	return RUN_ALL_TESTS();
#endif
}