// File:  traverser.h
// Date:  3/27/2011
// Auth:  K. Loux
// Desc:  Traverses directory structures searching for files in which
//        lines should be counted and added to the total count.

#ifndef _TRAVERSER_H_
#define _TRAVERSER_H_

// wxWidgets headers
#include <wx/dir.h>

class TRAVERSER : public wxDirTraverser
{
public:
	// Constructor
	TRAVERSER(const wxArrayString &_extensions);

	virtual wxDirTraverseResult OnFile(const wxString &filename);
	virtual wxDirTraverseResult OnDir(const wxString &dirname);

	const unsigned int GetBlankCount(void) { return blankLines; };
	const unsigned int GetCommentCount(void) { return commentLines; };
	const unsigned int GetCodeCount(void) { return codeLines; };

	const unsigned int GetFileCount(void) { return fileCount; };

private:
	// List of extensions to consider (ignore all else)
	const wxArrayString &extensions;

	// Counts for the current directory path
	unsigned int blankLines, commentLines, codeLines;
	unsigned int fileCount;
};

#endif// _TRAVERSER_H_
