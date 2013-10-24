// File:  lineCounterConfigFile.cpp
// Date:  10/23/2013
// Auth:  K. Loux
// Copy:  (c) Copyright 2013
// Desc:  Config file class for line counter application.

#ifdef WIN32
// Windows headers
#include <windows.h>
#else
// *nix headers
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

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
	AddConfigItem("COMMENT_CONTINUATION", info.continuationList);
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
	info.continuationList.clear();

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
	else if (!AllDirectoriesExist())
		configOK = false;
	else
	{
		unsigned int i;
		for (i = 0; i < info.directoryList.size(); i++)
		{
			if (*info.directoryList[i].rbegin() == '\\')
			{
				info.directoryList[i].erase(info.directoryList[i].length() - 1);
				info.directoryList[i].append("/");
			}
			else if (*info.directoryList[i].rbegin() != '/')
				info.directoryList[i].append("/");
		}
	}

	if (info.extensionList.size() == 0)
	{
		outStream << "Error:  Must specify at least one "
			<< GetKey(info.extensionList) << std::endl;
		configOK = false;
	}
	else
	{
		unsigned int i;
		for (i = 0; i < info.extensionList.size(); i++)
		{
			if (*info.extensionList[i].begin() == '.')
				info.extensionList[i].erase(0, 1);
		}
	}

	RemoveDuplicates(info.directoryList);
	RemoveDuplicates(info.extensionList);
	RemoveDuplicates(info.blockCommentList);
	RemoveDuplicates(info.commentList);
	RemoveDuplicates(info.continuationList);

	configOK = EnsureTokenUniqueness() && configOK;

	return configOK;
}

//==========================================================================
// Class:			LineCounterConfigFile
// Function:		AllDirectoriesExist
//
// Description:		Checks to see if the specified directories exist.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		bool, true if all directories exist, false otherwise
//
//==========================================================================
bool LineCounterConfigFile::AllDirectoriesExist(void) const
{
	bool directoriesExist(true);
	unsigned int i;
	for (i = 0; i < info.directoryList.size(); i++)
	{
		if (!DirectoryExists(info.directoryList[i]))
		{
			outStream << "Directory '" << info.directoryList[i]
				<< "' does not exist" << std::endl;
			directoriesExist = false;
		}
	}

	return directoriesExist;
}

//==========================================================================
// Class:			LineCounterConfigFile
// Function:		DirectoryExists
//
// Description:		Checks to see if the specified directory exists.
//
// Input Arguments:
//		directory	= const std::string&
//
// Output Arguments:
//		None
//
// Return Value:
//		bool, true if all directories exist, false otherwise
//
//==========================================================================
bool LineCounterConfigFile::DirectoryExists(const std::string &directory) const
{
#ifdef WIN32
	DWORD ftyp = GetFileAttributesA(directory.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;

  return (ftyp & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
#else
	struct stat st;
	if(stat(directory.c_str(), &st) == -1)
	{
		outStream << "Failed to check for existence of '" << directory << "'" << std::endl;
		return false;
	}

	return S_ISDIR(st.st_mode) == 1;
#endif
}

//==========================================================================
// Class:			LineCounterConfigFile
// Function:		EnsureTokenUniqueness
//
// Description:		Checks to make sure the same token wasn't specified as
//					a comment character and a line continuation character,
//					for example.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		bool, true all tokens are unique, false otherwise
//
//==========================================================================
bool LineCounterConfigFile::EnsureTokenUniqueness(void) const
{
	/*std::vector<std::string> commentList;
	std::vector<std::pair<std::string, std::string> > blockCommentList;
	std::vector<std::string> continuationList;*/

	bool unique(true);

	unsigned int i, j;
	for (i = 0; i < info.commentList.size(); i++)
	{
		for (j = 0; j < info.continuationList.size(); j++)
		{
			if (info.commentList[i].compare(info.continuationList[j]) == 0)
			{
				outStream << "Error:  '" << info.commentList[i] << "' specified for both "
					<< "single-line comment token and comment continuation token" << std::endl;
				unique = false;
			}
		}

		for (j = 0; j < info.blockCommentList.size(); j++)
		{
			if (info.commentList[i].compare(info.blockCommentList[j].first) == 0)
			{
				outStream << "Error:  '" << info.commentList[i] << "' specified for both "
					<< "single-line comment token and block-comment start token" << std::endl;
				unique = false;
			}

			if (info.commentList[i].compare(info.blockCommentList[j].second) == 0)
			{
				outStream << "Error:  '" << info.commentList[i] << "' specified for both "
					<< "single-line comment token and block-comment end token" << std::endl;
				unique = false;
			}
		}
	}

	for (i = 0; i < info.continuationList.size(); i++)
	{
		for (j = 0; j < info.blockCommentList.size(); j++)
		{
			if (info.continuationList[i].compare(info.blockCommentList[j].first) == 0)
			{
				outStream << "Error:  '" << info.continuationList[i] << "' specified for both "
					<< "comment continuation token and block-comment start token" << std::endl;
				unique = false;
			}

			if (info.continuationList[i].compare(info.blockCommentList[j].second) == 0)
			{
				outStream << "Error:  '" << info.continuationList[i] << "' specified for both "
					<< "comment continuation token and block-comment end token" << std::endl;
				unique = false;
			}
		}
	}

	for (i = 0; i < info.blockCommentList.size(); i++)
	{
		for (j = 0; j < info.blockCommentList.size(); j++)
		{
			if (info.blockCommentList[i].first.compare(info.blockCommentList[j].second) == 0)
			{
				outStream << "Error:  '" << info.blockCommentList[i].first << "' specified for both "
					<< "block-comment start token and block-comment end token" << std::endl;
				unique = false;
			}
		}
	}

	return unique;
}
