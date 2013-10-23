// File:  lineCounterApplication.h
// Auth:  K. Loux
// Date:  10/23/2013
// Desc:  Application to count the lines of code in a C++ project.
//        Specifiy directores to visit, and sub-directories will
//		  automatically be searched.  Counts lines in files with
//		  user-specified extensions, and provides statistics including
//		  total lines, comment lines, blank lines, and code lines.

#ifndef LINE_COUNTER_APPLICATION_H_
#define LINE_COUNTER_APPLICATION_H_

// Local headers
#include "lineCounterConfigFile.h"

// Local forward declarations
class Traverser;

class LineCounterApplication
{
public:
	LineCounterApplication();
	~LineCounterApplication();
	int Run(int argc, char *argv[]);

private:
	void PrintUsageInformation(std::string name);
	bool UpdateConfiguration(std::string fileName);
	void ParseFiles(void);
	void PrintCodeStatistics(void) const;

	Traverser *traverser;
	LineCounterInformation info;
	int traversalFlags;
};

#endif// LINE_COUNTER_APPLICATION_H_