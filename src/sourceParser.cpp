// File:  sourceParser.h
// Date:  10/23/2013
// Auth:  K. Loux
// Desc:  Parses source code.

// Standard C++ headers
#include <algorithm>

// Local headers
#include "sourceParser.h"
#include "stringTrimmer.h"

//==========================================================================
// Class:			SourceParser
// Function:		SourceParser
//
// Description:		Constructor for SourceParser class.
//
// Input Arguments:
//		commentIndicators		= const std::vector<std::string>&
//		blockCommentIndicators	= const std::vector<std::pair<std::string, std::string> >&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
SourceParser::SourceParser(const std::vector<std::string> &commentIndicators,
	const std::vector<std::pair<std::string, std::string> > &blockCommentIndicators)
	: commentIndicators(commentIndicators),
	blockCommentIndicators(blockCommentIndicators)
{
	Reset();
}

//==========================================================================
// Class:			SourceParser
// Function:		ParseLine
//
// Description:		Parses the specified line.
//
// Input Arguments:
//		line	= std::string
//
// Output Arguments:
//		None
//
// Return Value:
//		SourceParser::PositionState
//
//==========================================================================
SourceParser::PositionState SourceParser::ParseLine(std::string line)
{
	line = StringTrimmer::Trim(line);
	if (line.empty())
		return PositionWhitespace;

	size_t location;
	switch (state)
	{
	case PositionBlockComment:
		if ((location = LineContainsString(line, blockEndIndicator)) != std::string::npos)
		{
			state = PositionComment;
			line = line.substr(location);
			if (!line.empty())
				state = ParseLine(line);
		}
		// Otherwise - no change to state, we're still in a block comment
		break;

	case PositionComment:
	case PositionCode:
	case PositionWhitespace:
		if (LineStartsWithSingleLineComment(line))
			state = PositionComment;
		else if ((location = LineStartsWithBlockCommentStart(line, blockEndIndicator)) != std::string::npos)
		{
			state = PositionBlockComment;
			line = line.substr(location);
			if (!line.empty())
				ParseLine(line);
		}
		else
		{
			if ((location = LineContainsBlockCommentStart(line, blockEndIndicator)) != std::string::npos)
			{
				state = PositionBlockComment;
				line = line.substr(location);
				ParseLine(line);
				return PositionCode;
			}
			state = PositionCode;
		}
	}

	return state;
}

//==========================================================================
// Class:			SourceParser
// Function:		LineStartsWithSingleLineComment
//
// Description:		Determines if the line starts with a single-line comment
//					indicator.
//
// Input Arguments:
//		line	= const std::string&
//
// Output Arguments:
//		None
//
// Return Value:
//		bool
//
//==========================================================================
bool SourceParser::LineStartsWithSingleLineComment(const std::string &line) const
{
	unsigned int i;
	for (i = 0; i < commentIndicators.size(); i++)
	{
		if (line.find(commentIndicators[i]) == 0)
			return true;
	}

	return false;
}

//==========================================================================
// Class:			SourceParser
// Function:		LineStartsWithBlockCommentStart
//
// Description:		Determines if the line starts with a block comment start
//					indicator.
//
// Input Arguments:
//		line	= const std::string&
//
// Output Arguments:
//		matchingEndIndicator	= std::string&, if true, contains the string
//								  indicating the end of the comment section
//
// Return Value:
//		size_t, indicating of position of last character of indicator string
//
//==========================================================================
size_t SourceParser::LineStartsWithBlockCommentStart(const std::string &line,
	std::string& matchingEndIndicator) const
{
	unsigned int i;
	for (i = 0; i < blockCommentIndicators.size(); i++)
	{
		if (line.find(blockCommentIndicators[i].first) == 0)
		{
			matchingEndIndicator = blockCommentIndicators[i].second;
			return blockCommentIndicators[i].first.length();
		}
	}

	return std::string::npos;
}

//==========================================================================
// Class:			SourceParser
// Function:		LineContainsString
//
// Description:		Determines if the line contains the specified string.
//
// Input Arguments:
//		line	= const std::string&
//		s		= const std::string&
//
// Output Arguments:
//		None
//
// Return Value:
//		size_t, indicating of position of last character of indicator string
//
//==========================================================================
size_t SourceParser::LineContainsString(const std::string &line,
	const std::string &s) const
{
	size_t location = line.find(s);
	if (location == std::string::npos)
		return location;

	return line.find(s) + s.length();
}

//==========================================================================
// Class:			SourceParser
// Function:		LineContainsBlockCommentStart
//
// Description:		Determines if the line contains a block comment start indicator.
//
// Input Arguments:
//		line	= const std::string&
//
// Output Arguments:
//		matchingEndIndicator	= std::string&, if true, contains the string
//								  indicating the end of the comment section
//
// Return Value:
//		size_t, indicating of position of last character of indicator string
//
//==========================================================================
size_t SourceParser::LineContainsBlockCommentStart(const std::string &line,
	std::string& matchingEndIndicator) const
{
	size_t location;
	unsigned int i;
	std::vector<std::pair<size_t, std::string> > endOfSequences;
	for (i = 0; i < blockCommentIndicators.size(); i++)
	{
		if ((location = LineContainsString(line, blockCommentIndicators[i].first)) != std::string::npos)
		{
			endOfSequences.push_back(std::make_pair(location, blockCommentIndicators[i].second));
		}
	}

	if (endOfSequences.size() == 0)
		return std::string::npos;

	std::pair<size_t, std::string> firstMatch = *std::min_element(endOfSequences.begin(), endOfSequences.end());
	matchingEndIndicator = firstMatch.second;
	return firstMatch.first;
}