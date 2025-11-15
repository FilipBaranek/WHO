#pragma once
#include <cstdint>
#include <vector>

class IRecord
{
public:
	IRecord() = default;

	virtual bool equals(IRecord* other) = 0;

	virtual int getSize() = 0;

	virtual bool toBytes(std::vector<uint8_t>& bytesOutput) = 0;

	virtual IRecord* fromBytes(uint8_t* bytesInput) = 0;

	virtual ~IRecord() = default;
};