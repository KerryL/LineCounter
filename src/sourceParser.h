// File:  sourceParser.h
// Date:  10/23/2013
// Auth:  K. Loux
// Desc:  Parses source code.

#ifndef SOURCE_PARSER_H_
#define SOURCE_PARSER_H_

// Standard C++ headers
#include <string>
#include <vector>
#include <utility>

class SourceParser
{
public:
	SourceParser(const std::vector<std::string> &commentIndicators,
		const std::vector<std::pair<std::string, std::string> > &blockCommentIndicators);

	enum PositionState
	{
		PositionCode,
		PositionComment,
		PositionBlockComment,
		PositionWhitespace
	};

	void Reset(void) { state = PositionCode; };// Reset prior to parsing a new file
	PositionState ParseLine(std::string line);

private:
	const std::vector<std::string> commentIndicators;
	const std::vector<std::pair<std::string, std::string> > blockCommentIndicators;

	PositionState state;
	std::string blockEndIndicator;

	bool LineStartsWithSingleLineComment(const std::string &line) const;
	size_t LineStartsWithBlockCommentStart(const std::string &line, std::string& matchingEndIndicator) const;
	size_t LineContainsString(const std::string &line, const std::string &s) const;
	size_t LineContainsBlockCommentStart(const std::string &line, std::string& matchingEndIndicator) const;
};

#endif// SOURCE_PARSER_H_
