#ifndef TOOLS_H
#define TOOLS_H
#pragma once

#include <vector>

struct SSplitString
{
public:
	static std::vector<std::string> &SSplitString::split(const std::string &s, char delim, std::vector<std::string> &elems);
	static std::vector<std::string> SSplitString::Split(const std::string &s, char delim);
protected:
private:
};
#endif