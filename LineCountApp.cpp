// File:  LineCountApp.cpp
// Auth:  K. Loux
// Date:  12/5/2008
// Desc:  Application to count the lines of code in a C++ project.
//        Specifiy directores to visit, and sub-directories will
//		  automatically be searched.  Counts lines in files with
//		  user-specified extensions, and provides statistics including
//		  total lines, comment lines, blank lines, and code lines.

// Standard C++ headers
#include <iostream>

// wxWidgets headers
#include <wx/dir.h>

// LineCount headers
#include "config_file_class.h"
#include "traverser.h"

using namespace std;

// Entry point
int main (int argc, char *argv[])
{
	// Declare the config file
	wxString configFilename;

	// If an argument was specified, use that as the config file path
	if (argc > 1)
		//configFilename.Append(argv[1]);
		configFilename = _T(argv[1]);
	else
		configFilename = _T("config.ini");

	// Tell the user what we're doing
	cout << endl << "Reading configuration from '"
		<< configFilename << "'" << endl;

	// Read the config file
	CONFIG_FILE config(configFilename);

	// Create the objects required to traverse the directores
	wxArrayString directoryList = config.GetSearchDirectories();
	TRAVERSER traverser(config.GetExtensions());

	// Begin the process of searching the current directory
	cout << "Beginning search!" << endl;

	unsigned int i;
	for (i = 0; i < directoryList.GetCount(); i++)
	{
		// Make sure the directory exists
		if (!wxDir::Exists(directoryList[i]))
		{
			cout << "Directory '" << directoryList[i]
				<< "' does not exist!  Skipping..." << endl;
			continue;
		}

		// Traverse the directory (we give our own options here because we want to ignore hidden directories)
		wxDir directory(directoryList[i]);
		directory.Traverse(traverser, wxEmptyString, wxDIR_FILES | wxDIR_DIRS);
	}

	// For readability
	cout << endl;

	// Calculate  and print the statistics
	unsigned long totalLines = traverser.GetBlankCount()
		+ traverser.GetCommentCount() + traverser.GetCodeCount();

	cout << "Number of files searched: "
		<< traverser.GetFileCount() << endl;
	cout << "Total number of lines: " << totalLines << endl;

	if (totalLines < 1) totalLines = 1;// To avoid divide by zero

	double percentBlank = (double)traverser.GetBlankCount()
		/ (double)totalLines * 100.0;
	double percentComment = (double)traverser.GetCommentCount()
		/ (double)totalLines * 100.0;
	double percentCode = (double)traverser.GetCodeCount()
		/ (double)totalLines * 100.0;

	cout << "Blank lines: " << traverser.GetBlankCount()
		<< " (" << percentBlank << "%)" << endl;
	cout << "Comment lines: " << traverser.GetCommentCount()
		<< " (" << percentComment << "%)" << endl;
	cout << "Code lines: " << traverser.GetCodeCount()
		<< " (" << percentCode << "%)" << endl;

	// For readability
	cout << endl;

	return 0;
}