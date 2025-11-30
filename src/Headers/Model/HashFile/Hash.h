#pragma once
#include <vector>
#include <string>
#include <type_traits>
#include "../HeapFile/Helpers/ByteConverter.h"

class Hash
{
private:
	static constexpr const uint32_t FNV_PRIME = 16777619;
	static constexpr const uint32_t OFFSET = 2166136261;

public:
	template<typename T>
	static uint32_t hash(T& key)
	{
		std::vector<uint8_t> buffer;
		if constexpr (std::is_same<T, std::string>::value)
		{
			buffer.resize(key.size());
			uint8_t* index = buffer.data();

			for (size_t i{}; i < key.size(); ++i)
			{
				index = ByteConverter::toByteFromPrimitive<char>(key[i], index);
			}
		}
		else
		{
			buffer.resize(sizeof(T));
			ByteConverter::toByteFromPrimitive<T>(key, buffer.data());
		}

		uint32_t hash = OFFSET;
		for (auto& byte : buffer)
		{
			hash ^= byte;
			hash *= FNV_PRIME;
		}

		return hash;
	}

	Hash() = delete;
	~Hash() = delete;
};