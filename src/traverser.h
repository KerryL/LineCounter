// File:  traverser.h
// Date:  3/27/2011
// Auth:  K. Loux
// Desc:  Traverses directory structures searching for files in which
//        lines should be counted and added to the total count.

#ifndef TRAVERSER_H_
#define TRAVERSER_H_

// Standard C++ headers
#include <string>
#include <vector>

// wxWidgets headers
#include <wx/dir.h>

// Local headers
#include "lineCounter.h"

class Traverser : public wxDirTraverser
{
public:
	// Constructor
	Traverser(const std::vector<std::string> &extensions,
		const std::vector<std::string> &commentIndicators,
		const std::vector<std::pair<std::string, std::string> > &blockCommentIndicators);
	virtual ~Traverser() {};

	virtual wxDirTraverseResult OnFile(const wxString &fileName);
	virtual wxDirTraverseResult OnDir(const wxString &directoryName);

	const LineCounter::Statistics GetStatistics(void) const { return counter.GetStatistics(); };

private:
	const std::vector<std::string> extensions;// List of extensions to parse

	LineCounter counter;
};

#endif// TRAVERSER_H_
