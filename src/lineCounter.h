// File:  lineCounter.h
// Date:  10/23/2013
// Auth:  K. Loux
// Desc:  Parses the specified files to generate line count statistics.

#ifndef LINE_COUNTER_H_
#define LINE_COUNTER_H_

// Standard C++ headers
#include <string>
#include <vector>
#include <utility>

// Local headers
#include "sourceParser.h"

class LineCounter
{
public:
	LineCounter(const std::vector<std::string> &commentIndicators,
		const std::vector<std::pair<std::string, std::string> > &blockCommentIndicators);

	bool ProcessFile(std::string fileName);

	struct Statistics
	{
		unsigned int blankLines;
		unsigned int commentLines;
		unsigned int codeLines;
		unsigned int fileCount;
	};

	Statistics GetStatistics(void) const { return statistics; };

private:
	SourceParser parser;
	Statistics statistics;
};

#endif// LINE_COUNTER_H_
