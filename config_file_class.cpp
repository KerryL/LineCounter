// config_file_class.cpp
// Created:	4/4/08
// Author:	K. Loux
// This class contains the structure used to pass all of the
// configuration parameters as well as means for parsing the config
// file to make the file more maintainable and easier to read.
//
// History:
//  3/27/2011 - changed to use wxWidgets objects, K. Loux

// Standard C++ headers
#include <fstream>
#include <string>
#include <iostream>
//#include <sstream>

// wxWidgets headers
#include <wx/wx.h>
#include <wx/dir.h>

// Local headers
#include "config_file_class.h"

using namespace std;

// Definitions for fields in config file - the entries in the file must
// have exactly this text for the following item to be read in
// correctly.
const wxString CONFIG_FILE::CONFIG_FIELDS::commentChar		= "#";
const wxString CONFIG_FILE::CONFIG_FIELDS::searchDirectory	= "DIRECTORY";
const wxString CONFIG_FILE::CONFIG_FIELDS::extension		= "EXTENSION";

// Constructor
CONFIG_FILE::CONFIG_FILE(const wxString &fileName)
{
	ifstream dataFile(fileName.c_str(), ios::in);
	if(!dataFile)
	{
		cout << "Error:  '" << fileName <<
			"' not found!  Terminating application..." << endl;

		exit(1);
	}

	// These are used during the read process
	string line;
	wxString field;
	wxString data;

	// Go through the config file line by line until we reach the end
	while (getline(dataFile, line))
	{
		// If we encounter a blank line or a line that starts with a
		// comment character, we ignore the line.  Otherwise we check
		// to make sure that the line contains two separate entries
		// (the field and the data) and we apply the data to the
		// corresponding variable.

		// Make sure it's not a blank line
		if (line.empty())
		{
			// This one comes first, because if the line is empty, we
			// can't make calls like Line.at(0) to see if the line is
			// a comment or contains spaces
		}
		// Make sure it isn't a commented line and that it line
		// contains at least one space (the field and the data are
		// separated by at least one space).
		else if (CONFIG_FIELDS::commentChar.CmpNoCase(line.at(0)) != 0
				&& line.find(" ", 0) != string::npos)
		{
			// Break out the data into a Field and the Data (Data may
			// contain spaces!)
			size_t locationOfFirstSpace = line.find_first_of(" ", 0);
			size_t startOfData = line.find_first_not_of(" ",
				locationOfFirstSpace);
			data = line.substr(startOfData,
				line.length() - startOfData);
			field = line.substr(0, locationOfFirstSpace);

			// Assign the data to the correct variable based on the
			// field
			if (field.compare(CONFIG_FIELDS::searchDirectory) == 0)
				searchDirectories.Add(data);
			else if (field.compare(CONFIG_FIELDS::extension) == 0)
				extensions.Add(data);
			else
				// Tell the user we didn't recognize the field and
				// disregarded the data
				cout << "Unknown field: " << field << endl;
		}
	}

	// Close the config file
	dataFile.close();

	// Make sure the data is OK
	// If it isn't OK, we exit here.  Reasons for exiting are printed
	// to the screen when the checks are being done, so we don't need
	// to do that here.
	if (!VerifyConfigData())
	{
		cout << "Exiting application" << endl;

		exit(1);
	}
}

// Destructor
CONFIG_FILE::~CONFIG_FILE()
{
}

// Runs checks to make sure the data is OK
bool CONFIG_FILE::VerifyConfigData(void)
{
	string error;

	// Check to make sure all of the directories are valide
	unsigned int i;
	for (i = 0; i > searchDirectories.GetCount(); i++)
	{
		if (!wxDir::Exists(searchDirectories[i]))
		{
			// Tell the user what the problem was
			cout << "Error:  Search path '"
				<< searchDirectories[i] << "' is invalid!"
				<< endl;

			return false;
		}
	}

	// Make sure we have at least one directory to search
	if (searchDirectories.GetCount() == 0)
	{
		cout << "Error:  Must have at least one directory to search!"
			<< endl;

		return false;
	}

	// Must have at least one extension
	if (extensions.GetCount() == 0)
	{
		cout << "Error:  Must have at least one file extension!"
			<< endl;

		return false;
	}

	// If we haven't already returned, then we passed all the checks.
	return true;
}