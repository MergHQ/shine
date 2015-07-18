#ifndef OPTION_H
#define OPTION_H

#include <vector>
#include <string>

class Option
{
public:
	std::string m_name;
	std::vector<double> m_values;

	Option(std::string name) : m_name(name) {}
	Option(){}
	~Option(){}

	void setValues(std::vector<double> values)
	{
		m_values = values;
	}
};

#endif