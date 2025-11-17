#pragma once
#include <cstdint>
#include "../Interfaces/IRecord.h"

class RecordFactory
{
public:
	template<typename T>
	static T* createInstance(uint8_t* buffer)
	{
		static_assert(std::is_base_of_v<IRecord, T>, "T must inherit from IRecord");

		return T::createInstance(buffer);
	}

	template<typename T>
	static T* createInstance()
	{
		static_assert(std::is_base_of_v<IRecord, T>, "T must inherit from IRecord");
		
		return T::dummyInstance();
	}

	RecordFactory() = delete;
	~RecordFactory() = delete;
};