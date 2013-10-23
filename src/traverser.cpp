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

// Constructor
TRAVERSER::TRAVERSER(const wxArrayString &_extensions) :
		extensions(_extensions)
{
	// Initialize the counts
	blankLines = 0;
	commentLines = 0;
	codeLines = 0;
	fileCount = 0;
}

// Process each file
wxDirTraverseResult TRAVERSER::OnFile(const wxString &filename)
{
	// Check to see if this file has a valid extension
	wxString ext = filename.AfterLast('.');
	bool validExtension = false;
	unsigned int i;
	for (i = 0; i < extensions.GetCount(); i++)
	{
		if (extensions[i].CmpNoCase(ext) == 0)
		{
			validExtension = true;
			break;
		}
	}

	// Don't go any further if we're not interested in this extension
	if (!validExtension)
		return wxDIR_CONTINUE;

	// Tell the user what file we're parsing
#ifdef __WXMSW__
	wxString shortFilename = filename.Mid(filename.find_last_of('\\') + 1);
#elif defined __WXGTK__
	wxString shortFilename = filename.Mid(filename.find_last_of('/') + 1);
#else
#error "Unrecognized platform!"
#endif
	cout << "  Parsing file: " << shortFilename << endl;

	// Open the file
	ifstream file(filename.c_str(), ios::in);

	// Make sure the file opened successfully
	if (!file.is_open() || !file.good())
	{
		cout << "    Error reading file!  Skipping..." << endl;
		return wxDIR_IGNORE;
	}

	// Do the parsing
	string currentLine;
	bool isInCommentSection = false;
	while (getline(file, currentLine))
	{
		// Check to see if the line is empty
		if (currentLine.empty())
			blankLines++;
		else// not empty (but could still be blank!)
		{
			bool containsNonBlankCharacters = false;
			bool couldBeCommentLine = false;
			bool couldBeEndOfCommentSection = false;
			bool isCommentLine = false;
			bool commentSectionEndedWithThisLine = false;
			unsigned int i;
			for (i = 0; i < currentLine.length(); i++)
			{
				// Only do this part if the only characters we have encountered so far are blank
				if (!containsNonBlankCharacters)
				{
					// Check to see if the current character is non-blank
					if (currentLine.at(i) != ' ' && currentLine.at(i) != '\t')
					{
						containsNonBlankCharacters = true;

						// Check to see if we could be on a comment line
						if (currentLine.at(i) == '/')
							couldBeCommentLine = true;
						else if (currentLine.at(i) == '*' && isInCommentSection)
							couldBeEndOfCommentSection = true;
					}
				}
				else if (isInCommentSection)
				{
					// We are looking for the end of the comment section
					isCommentLine = true;

					if (couldBeEndOfCommentSection && currentLine.at(i) == '/')
					{
						isInCommentSection = false;
						commentSectionEndedWithThisLine = true;
					}
					if (currentLine.at(i) == '*')
						couldBeEndOfCommentSection = true;
				}
				else// We have encountered a character that is not blank and we are not in a comment section
				{
					// Check to see if we think we might be starting a comment line
					if (couldBeCommentLine)
					{
						// Check for line comment
						if (currentLine.at(i) == '/')
						{
							isCommentLine = true;
							break;
						}
						else if (currentLine.at(i) == '*')
						{
							isCommentLine = true;
							isInCommentSection = true;
						}
					}
					else if (commentSectionEndedWithThisLine && isCommentLine)// and we're still here!
					{
						// If the comment just ended, and we have stuff after the end of the comment that is not
						// blank, then this is no longer a comment line, it is a code line!
						if (currentLine.at(i) != ' ' && currentLine.at(i) != '\t')
							isCommentLine = false;
					}

					// Reset the "we think we might be a..." flags
					couldBeCommentLine = false;
					couldBeEndOfCommentSection = false;
				}
			}

			// If the line contains only non-blank characters, add it to the blank line count
			if (!containsNonBlankCharacters)
				blankLines++;
			else if (isCommentLine)
				commentLines++;
			else
				codeLines++;
		}
	}

	// Increment our file counter
	fileCount++;

	// Close the file
	file.close();

	return wxDIR_CONTINUE;// FIXME!!!
}

// Process each directory
wxDirTraverseResult TRAVERSER::OnDir(const wxString &dirname)
{
	// Tell the user what directory we're descending into
	cout << "Entering directory: " << dirname << endl;
	return wxDIR_CONTINUE;
}
