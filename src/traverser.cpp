// File:  traverser.cpp
// Date:  3/27/2011
// Auth:  K. Loux
// Desc:  Traverses directory structures searching for files in which
//        lines should be counted and added to the total count.

// Standard C++ headers
#include <iostream>
#include <string>
#include <fstream>

// wxWidgets headers
#include <wx/wx.h>

// Local headers
#include "traverser.h"

using namespace std;

//==========================================================================
// Class:			Traverser
// Function:		Traverser
//
// Description:		Constructor for Traverser class.
//
// Input Arguments:
//		extensions				= const std::vector<std::string>&
//		commentIndicators		= const std::vector<std::string>&
//		blockCommentIndicators	= const std::vector<std::pair<std::string, std::string> >&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
Traverser::Traverser(const std::vector<std::string> &extensions,
	const std::vector<std::string> &commentIndicators,
	const std::vector<std::pair<std::string, std::string> > &blockCommentIndicators) :
		extensions(extensions), counter(commentIndicators, blockCommentIndicators)
{
}

//==========================================================================
// Class:			Traverser
// Function:		OnFile
//
// Description:		Called each time a new file is encountered.  Determines
//					if we should parse it and add to the statistics (if
//					extension matches).
//
// Input Arguments:
//		fileName	= const wxString&
//
// Output Arguments:
//		None
//
// Return Value:
//		wxDirTraverseResult
//
//==========================================================================
wxDirTraverseResult Traverser::OnFile(const wxString &fileName)
{
	wxString ext = fileName.AfterLast('.');
	unsigned int i;
	for (i = 0; i < extensions.size(); i++)
	{
		if (wxString(extensions[i].c_str(), wxConvUTF8).CmpNoCase(ext) == 0)
		{
#ifdef __WXMSW__
			wxString shortFileName = fileName.Mid(fileName.find_last_of('\\') + 1);
#elif defined __WXGTK__
			wxString shortFileName = fileName.Mid(fileName.find_last_of('/') + 1);
#else
#error "Unrecognized platform!"
#endif
			cout << "  Parsing file: " << shortFileName.mb_str() << endl;
			if (!counter.ProcessFile(std::string(fileName.mb_str())))
				cout << "  Problems encounterd while parsing '"
				<< shortFileName.mb_str() << "'" << endl;
			break;
		}
	}

	return wxDIR_CONTINUE;
}

//==========================================================================
// Class:			Traverser
// Function:		OnDir
//
// Description:		Called each time a new directory is encountered.  Simply
//					prints the directory name to the screen, for the user's
//					information.
//
// Input Arguments:
//		directoryName	= const wxString&
//
// Output Arguments:
//		None
//
// Return Value:
//		wxDirTraverseResult
//
//==========================================================================
wxDirTraverseResult Traverser::OnDir(const wxString &directoryName)
{
	cout << "Entering directory: " << directoryName.mb_str() << endl;
	return wxDIR_CONTINUE;
}