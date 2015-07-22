#ifndef OPTION_H
#define OPTION_H
#pragma once

#include <vector>
#include <string>

class Option
{
public:
	Option(std::string name) : m_name(name) {}
	Option(){}
	~Option(){}

	std::string m_name;
	std::vector<double> m_values;

	void setValues(std::vector<double> values) { m_values = values; }
protected:
private:
};
#endif