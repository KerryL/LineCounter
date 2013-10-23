// File:  stringTrimmer.h
// Date:  10/23/2013
// Auth:  K. Loux
// Desc:  String trimming routines.

#ifndef STRING_TRIMMER_H_
#define STRING_TRIMMER_H_

// Standard C++ headers
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace StringTrimmer
{

static inline std::string LeftTrim(std::string s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

static inline std::string RightTrim(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

static inline std::string Trim(std::string s)
{
	return LeftTrim(RightTrim(s));
}

}

#endif// STRING_TRIMMER_H_