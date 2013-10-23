// File:  lineCounterApplication.cpp
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

// Local headers
#include "lineCounterApplication.h"
#include "traverser.h"

using namespace std;

//==========================================================================
// Class:			None
// Function:		main
//
// Description:		Application entry point.
//
// Input Arguments:
//		argc	= int
//		argv	= char*[]
//
// Output Arguments:
//		None
//
// Return Value:
//		int, 0 for success, 1 otherwise
//
//==========================================================================
int main (int argc, char *argv[])
{
	LineCounterApplication app;
	return app.Run(argc, argv);
}

//==========================================================================
// Class:			LineCounterApplication
// Function:		LineCounterApplication
//
// Description:		Constructor for LineCounterApplication class.
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
LineCounterApplication::LineCounterApplication()
{
	traverser = NULL;
}

//==========================================================================
// Class:			LineCounterApplication
// Function:		~LineCounterApplication
//
// Description:		Destructor for LineCounterApplication class.
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
LineCounterApplication::~LineCounterApplication()
{
	delete traverser;
}

//==========================================================================
// Class:			LineCounterApplication
// Function:		Run
//
// Description:		Main application function.
//
// Input Arguments:
//		argc	= int
//		argv	= char*[]
//
// Output Arguments:
//		None
//
// Return Value:
//		int, 0 for success, 1 otherwise
//
//==========================================================================
int LineCounterApplication::Run(int argc, char *argv[])
{
	if (argc != 2)
	{
		PrintUsageInformation(argv[0]);
		return 1;
	}

	if (!UpdateConfiguration(argv[1]))
		return 1;

	traverser = new Traverser(info.extensionList, info.commentList, info.blockCommentList);
	ParseFiles();
	PrintCodeStatistics();

#ifdef WIN32
	system("PAUSE");
#endif

	return 0;
}

//==========================================================================
// Class:			LineCounterApplication
// Function:		PrintUsageInformation
//
// Description:		Prints usage format.
//
// Input Arguments:
//		name	= std::string, binary name
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void LineCounterApplication::PrintUsageInformation(std::string name)
{
	cout << "Usage:  " << name << " <config file name>" << endl;
}

//==========================================================================
// Class:			LineCounterApplication
// Function:		UpdateConfiguration
//
// Description:		Reads the configuration and updates the member variables.
//
// Input Arguments:
//		name	= std::string, binary name
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
bool LineCounterApplication::UpdateConfiguration(std::string fileName)
{
	LineCounterConfigFile config;
	if (!config.ReadConfiguration(fileName))
	{
		cout << "Failed to read configuration from:  '" << fileName << "'" << endl;
		return false;
	}

	info = config.GetInformation();
	traversalFlags = wxDIR_FILES;
	if (info.recurseIntoSubDirectories)
	{
		traversalFlags |= wxDIR_DIRS;
		cout << "Sub-directories will automatically be added to the search" << endl;
	}

	return true;
}

//==========================================================================
// Class:			LineCounterApplication
// Function:		ParseFiles
//
// Description:		Calls the traverser to perform the search.
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
void LineCounterApplication::ParseFiles(void)
{
	cout << "Beginning search!" << endl;
	unsigned int i;
	for (i = 0; i < info.directoryList.size(); i++)
	{
		// Make sure the directory exists (this is really redundant, since the
		// same check happens in the config file class)
		if (!wxDir::Exists(wxString(info.directoryList[i].c_str(), wxConvUTF8)))
		{
			cout << "Directory '" << info.directoryList[i]
				<< "' does not exist!  Skipping..." << endl;
			continue;
		}

		wxDir directory(wxString(info.directoryList[i].c_str(), wxConvUTF8));
		directory.Traverse(*traverser, wxEmptyString, traversalFlags);
	}
}

//==========================================================================
// Class:			LineCounterApplication
// Function:		PrintCodeStatistics
//
// Description:		Prints code statistics to the screen.
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
void LineCounterApplication::PrintCodeStatistics(void) const
{
	// Calculate  and print the statistics
	LineCounter::Statistics statistics = traverser->GetStatistics();
	unsigned long totalLines = statistics.blankLines +
		+ statistics.commentLines + statistics.codeLines;

	cout << endl;
	cout << "Number of files searched: " << statistics.fileCount << endl;
	cout << "Total number of lines: " << totalLines << endl;

	if (totalLines < 1) totalLines = 1;// To avoid divide by zero

	double percentBlank = (double)statistics.blankLines
		/ (double)totalLines * 100.0;
	double percentComment = (double)statistics.commentLines
		/ (double)totalLines * 100.0;
	double percentCode = (double)statistics.codeLines
		/ (double)totalLines * 100.0;

	cout << "Blank lines: " << statistics.blankLines
		<< " (" << percentBlank << "%)" << endl;
	cout << "Comment lines: " << statistics.commentLines
		<< " (" << percentComment << "%)" << endl;
	cout << "Code lines: " << statistics.codeLines
		<< " (" << percentCode << "%)" << endl;
	cout << endl;
}