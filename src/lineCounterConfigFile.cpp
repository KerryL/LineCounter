// File:  lineCounterConfigFile.cpp
// Date:  10/23/2013
// Auth:  K. Loux
// Copy:  (c) Copyright 2013
// Desc:  Config file class for line counter application.

// Local headers
#include "lineCounterConfigFile.h"

//==========================================================================
// Class:			LineCounterConfigFile
// Function:		BuildConfigItems
//
// Description:		Builds the list of configuration options to look for.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void LineCounterConfigFile::BuildConfigItems(void)
{
	AddConfigItem("DIRECTORY", info.directoryList);
	AddConfigItem("EXTENSION", info.extensionList);
	AddConfigItem("COMMENT", info.commentList);
	AddConfigItem("BLOCK_COMMENT_START", blockCommentStartList);
	AddConfigItem("BLOCK_COMMENT_END", blockCommentEndList);
	AddConfigItem("RECURSIVE_SEARCH", info.recurseIntoSubDirectories);
}

//==========================================================================
// Class:			LineCounterConfigFile
// Function:		AssignDefaults
//
// Description:		Assigns default values to the member variables.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void LineCounterConfigFile::AssignDefaults(void)
{
	// Just clear out the member vectors, in case we're re-reading
	info.directoryList.clear();
	info.extensionList.clear();
	info.commentList.clear();
	info.blockCommentList.clear();

	blockCommentStartList.clear();
	blockCommentEndList.clear();

	info.recurseIntoSubDirectories = false;
}

//==========================================================================
// Class:			LineCounterConfigFile
// Function:		ConfigIsOK
//
// Description:		Checks to see if the configuration options are valid.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		bool, true for success, false otherwise
//
//==========================================================================
bool LineCounterConfigFile::ConfigIsOK(void)
{
	bool configOK = true;
	if (blockCommentStartList.size() == blockCommentEndList.size())
	{
		unsigned int i;
		for (i = 0; i < blockCommentStartList.size(); i++)
			info.blockCommentList.push_back(std::make_pair(
			blockCommentStartList[i], blockCommentEndList[i]));
	}
	else
	{
		outStream << "Error:  Number of " << GetKey(blockCommentStartList)
			<< " and " << GetKey(blockCommentEndList)
			<< " specified must be equal" << std::endl;
		configOK = false;
	}

	if (info.directoryList.size() == 0)
	{
		outStream << "Error:  Must specify at least one "
			<< GetKey(info.directoryList) << std::endl;
		configOK = false;
	}
	else if (false)// TODO:  Directories exist?
	{
		configOK = false;
	}
	else
	{
		// TODO:  If entry doesn't end with a \, append it
	}

	if (info.extensionList.size() == 0)
	{
		outStream << "Error:  Must specify at least one "
			<< GetKey(info.extensionList) << std::endl;
		configOK = false;
	}

	return configOK;
}