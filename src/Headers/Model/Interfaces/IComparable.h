#pragma once

class IComparable
{
public:
	//Left = -1 Equal = 0 Right = 1
	virtual int compare(IComparable* other) const = 0;
	virtual ~IComparable() = default;
};