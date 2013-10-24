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
	std::vector<std::string> continuationList;

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
	bool AllDirectoriesExist(void) const;
	bool DirectoryExists(const std::string &directory) const;

	template <typename T>
	void RemoveDuplicates(std::vector<T> &v) const;

	bool EnsureTokenUniqueness(void) const;
};

//==========================================================================
// Class:			None
// Function:		operator<< (template)
//
// Description:		Stream operator defined for std::pair.
//
// Input Arguments:
//		out		= std::ostream&
//		value	= const std::pair<T1, T2>&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
template <class T1, class T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& value)
{
	out << value.first << " : " << value.second;
	return out;
}

//==========================================================================
// Class:			LineCounterConfigFile
// Function:		RemoveDuplicates (template)
//
// Description:		Removes duplicate entries from the specified vector.
//
// Input Arguments:
//		v	= std::vector<T>
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
template <typename T>
void LineCounterConfigFile::RemoveDuplicates(std::vector<T> &v) const
{
	unsigned int i, j;
	for (i = 0; i < v.size(); i++)
	{
		for (j = i + 1; j < v.size(); j++)
		{
			if (v[i] == v[j])
			{
				outStream << "Warning:  Removing duplicate entry '" << v[j] << "'" << std::endl;
				v.erase(v.begin() + j);
				j--;
			}
		}
	}
}

#endif// LINE_COUNTER_CONFIG_FILE_H_