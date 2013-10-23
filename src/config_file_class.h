// config_file_class.h
// Created:	4/4/08
// Author:	K. Loux
// This class contains the structure used to pass all of the
// configuration parameters as well as means for parsing the config
// file to make the file more maintainable and easier to read.

#ifndef _CONFIG_FILE_CLASS_H_
#define _CONFIG_FILE_CLASS_H_

// wxWidgets headers
#include <wx/wx.h>

class CONFIG_FILE
{
public:
	CONFIG_FILE(const wxString &fileName);
	~CONFIG_FILE();

	// The fields that are contained in the config file.  These are the strings
	// that identify the parameters in the file.
	static const struct CONFIG_FIELDS
	{
		static const wxString commentChar;
		static const wxString searchDirectory;
		static const wxString extension;
	} configFields;

	// Private data accessors
	const wxArrayString &GetSearchDirectories(void)
	{
		return searchDirectories;
	};

	const wxArrayString &GetExtensions(void)
	{
		return extensions;
	};

private:
	// Runs checks to make sure the data is valid
	bool VerifyConfigData(void);

	// List of directories to search
	wxArrayString searchDirectories;

	// List of extensions to search for
	wxArrayString extensions;
};

#endif// _CONFIG_FILE_CLASS_H_
