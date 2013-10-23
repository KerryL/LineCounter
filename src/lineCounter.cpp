// File:  lineCounter.cpp
// Date:  10/23/2013
// Auth:  K. Loux
// Desc:  Parses the specified files to generate line count statistics.

// Standard C++ headers
#include <fstream>
#include <iostream>
#include <cassert>

// Local headers
#include "lineCounter.h"

using namespace std;

//==========================================================================
// Class:			LineCounter
// Function:		LineCounter
//
// Description:		Constructor for LineCounter class.
//
// Input Arguments:
//		commentIndicators	= const std::vector<std::string>&
//		blockCommentIndicators	= const std::vector<std::pair<std::string, std::string> >&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
LineCounter::LineCounter(const std::vector<std::string> &commentIndicators,
	const std::vector<std::pair<std::string, std::string> > &blockCommentIndicators)
	: parser(commentIndicators, blockCommentIndicators)
{
	statistics.blankLines = 0;
	statistics.codeLines = 0;
	statistics.commentLines = 0;
	statistics.fileCount = 0;
}

//==========================================================================
// Class:			LineCounter
// Function:		ProcessFile
//
// Description:		Parses the specified file and updates our internal statistics.
//
// Input Arguments:
//		fileName	= std::string
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
bool LineCounter::ProcessFile(std::string fileName)
{
	ifstream file(fileName.c_str(), ios::in);
	if (!file.is_open() || !file.good())
	{
		cout << "  Error reading file!  Skipping..." << endl;
		return false;
	}

	statistics.fileCount++;

	string currentLine;
	SourceParser::PositionState state;
	while (getline(file, currentLine))
	{
		state = parser.ParseLine(currentLine);
		if (state == SourceParser::PositionBlockComment ||
			state == SourceParser::PositionComment)
			statistics.commentLines++;
		else if (state == SourceParser::PositionWhitespace)
			statistics.blankLines++;
		else
		{
			assert(state == SourceParser::PositionCode);
			statistics.codeLines++;
		}
	}

	file.close();

	return true;
}
