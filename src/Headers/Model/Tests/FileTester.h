#pragma once
#include <random>
#include <iostream>
#include <unordered_map>
#include "../HeapFile/HeapFile.h"
#include "../PCRDatabase/Generator/RandomDataGenerator.h"

template<typename T>
class FileTester
{
private:
	static constexpr const int REPLICATIONS = 1000;
	static constexpr const int CHECKPOINT = 10'000;
	
	std::random_device m_rd;
	std::mt19937 m_gen{ m_rd() };
	std::unordered_map<T*, int> m_data;
	HeapFile<T> m_heapFile;

public:
	FileTester(std::string filePath, int clusterSize, int dataCount = 0) : m_heapFile(filePath, clusterSize)
	{
		m_heapFile.open();

		for (int i{}; i < dataCount; ++i)
		{
			Person* person = RandomDataGenerator::generatePerson();

			int address = m_heapFile.insert(person);
			m_data[person] = address;
		}
	}

	void insert()
	{
		Person* person = RandomDataGenerator::generatePerson();

		int address = m_heapFile.insert(person);
		Person* foundPerson = m_heapFile.find(address, person);

		if (!person->is(foundPerson))
		{
			throw std::runtime_error("Incorrect insertion");
		}

		m_data[person] = address;
		delete foundPerson;
	}
	
	void find()
	{
		std::uniform_int_distribution<unsigned int> randomAddressInterval(0, m_data.size() - 1);
		
		auto iterator = m_data.begin();
		std::advance(iterator, randomAddressInterval(m_gen));

		int address = iterator->second;
		Person* personToFind = iterator->first;
		Person* foundPerson = m_heapFile.find(address, personToFind);

		if (foundPerson == nullptr || !personToFind->is(foundPerson) || !personToFind->equals(foundPerson))
		{
			throw std::runtime_error("Incorrect find operation");
		}
		delete foundPerson;
	}
	
	void remove()
	{
		std::uniform_int_distribution<unsigned int> randomAddressInterval(0, m_data.size() - 1);

		auto iterator = m_data.begin();
		std::advance(iterator, randomAddressInterval(m_gen));

		int address = iterator->second;
		Person* personToRemove = iterator->first;
		Person* removedPerson = m_heapFile.remove(address, personToRemove);
		Person* foundPerson = m_heapFile.find(address, personToRemove);

		if (removedPerson == nullptr || !personToRemove->is(removedPerson) || !personToRemove->equals(removedPerson) || foundPerson != nullptr)
		{
			throw std::runtime_error("Incorrect removal");
		}

		m_data.erase(personToRemove);
		delete removedPerson;
		delete foundPerson;
	}
	
	void printOut()
	{

	}
	
	void runTests()
	{
		std::uniform_int_distribution<unsigned int> probability(0, 2);

		for (int i{}; i < REPLICATIONS; ++i)
		{
			if (i % CHECKPOINT == 0)
			{
				std::cout << "Operation " << i + 1 << "\n";
			}
			std::cout << "Operation " << i + 1 << " HeapFile size: " << m_heapFile.size() << "\n";
			
			int operation = probability(m_gen);

			switch (operation)
			{
				case 0:
					insert();
					break;
				case 1:
					if (m_data.size() > 0)
					{
						find();
					}
					break;
				case 2:
				default:
					if (m_data.size() > 0)
					{
						remove();
					}
					break;
			}
		}

		printOut();
	}
	
	~FileTester()
	{
		for (auto& [person, _] : m_data)
		{
			delete person;
		}
	};
};