#pragma once
#include <string>

class IStorable
{
public:
	IStorable() = default;
	virtual std::string writeLine() = 0;
	virtual IStorable* loadLine() = 0;
	virtual ~IStorable() = default;
};