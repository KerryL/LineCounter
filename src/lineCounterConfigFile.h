// File:  lineCounterConfigFile.h
// Date:  10/23/2013
// Auth:  K. Loux
// Copy:  (c) Copyright 2013
// Desc:  Config file class for line counter application.

#ifndef LINE_COUNTER_CONFIG_FILE_H_
#define LINE_COUNTER_CONFIG_FILE_H_

// Standard C++ headers
#include <utility>

// Local headers
#include "configFile.h"

struct LineCounterInformation
{
	std::vector<std::string> directoryList;
	std::vector<std::string> extensionList;
	std::vector<std::string> commentList;
	std::vector<std::pair<std::string, std::string> > blockCommentList;

	bool recurseIntoSubDirectories;
};

class LineCounterConfigFile : public ConfigFile
{
public:
	virtual ~LineCounterConfigFile() {};

	LineCounterInformation GetInformation(void) const { return info; };

private:
	LineCounterInformation info;
	std::vector<std::string> blockCommentStartList, blockCommentEndList;

	virtual void BuildConfigItems(void);
	virtual void AssignDefaults(void);

	virtual bool ConfigIsOK(void);
};

#endif// LINE_COUNTER_CONFIG_FILE_H_