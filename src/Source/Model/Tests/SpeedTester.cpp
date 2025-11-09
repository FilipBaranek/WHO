#include "../../../Headers/Model/Tests/SpeedTester.h"

SpeedTester::SpeedTester(bool dataInOrder)
{
	m_orderedData = dataInOrder;
	for (int i = 1; i <= RANDOM_DATA_COUNT; ++i)
	{
		m_randomData.push_back(new Number(i));
	}

	if (!m_orderedData)
	{
		std::shuffle(m_randomData.begin(), m_randomData.end(), m_g);
	}
}

void SpeedTester::testInsertion()
{
	std::cout << "\nINSERTION\n";
	long long duration = 0;
	long long durationAT = 0;
	long long durationRB = 0;

	for (auto& num : m_randomData)
	{
		auto start = high_resolution_clock::now();
		m_bst.insert(num);
		auto end = high_resolution_clock::now();
		duration += duration_cast<nanoseconds>(end - start).count();

		auto startAT = high_resolution_clock::now();
		m_at.insert(num);
		auto endAT = high_resolution_clock::now();
		durationAT += duration_cast<nanoseconds>(endAT - startAT).count();

		auto startRB = high_resolution_clock::now();
		m_rb[num] = num->getData();
		auto endRB = high_resolution_clock::now();
		durationRB += duration_cast<nanoseconds>(endRB - startRB).count();
	}

	std::cout << "Binary search tree\n"
			  << duration / 1000000.0 << " milliseconds\n"
			  << "AVL tree\n"
			  << durationAT / 1000000.0 << " milliseconds\n"
			  << "RB tree\n"
			  << durationRB / 1000000.0 << " milliseconds\n";
}

void SpeedTester::testRemoval()
{
	if (!m_orderedData)
	{
		std::shuffle(m_randomData.begin(), m_randomData.end(), m_g);
	}

	int startIndex = m_randomData.size() - REMOVE_DATA_COUNT;
	int endIndex = m_randomData.size();

	std::cout << "\nREMOVAL\n";
	long long duration = 0;
	long long durationAT = 0;
	long long durationRB = 0;

	for (int i = startIndex; i < endIndex; ++i)
	{
		auto start = high_resolution_clock::now();
		m_bst.remove(m_randomData[i]);
		auto end = high_resolution_clock::now();
		duration += duration_cast<nanoseconds>(end - start).count();

		auto startAT = high_resolution_clock::now();
		m_at.remove(m_randomData[i]);
		auto endAT = high_resolution_clock::now();
		durationAT += duration_cast<nanoseconds>(endAT - startAT).count();

		auto startRB = high_resolution_clock::now();
		m_rb.erase(m_randomData[i]);
		auto endRB = high_resolution_clock::now();
		durationRB += duration_cast<nanoseconds>(endRB - startRB).count();
	}

	std::cout << "Binary search tree\n"
			  << duration / 1000000.0 << " milliseconds\n"
			  << "AVL tree\n"
			  << durationAT / 1000000.0 << " milliseconds\n"
			  << "RB tree\n"
			  << durationRB / 1000000.0 << " milliseconds\n";

	for (int i{}; i < REMOVE_DATA_COUNT; ++i)
	{
		Number* data = m_randomData[m_randomData.size() - 1];
		m_randomData.pop_back();
		delete data;
	}
}

void SpeedTester::testPointSearch()
{
	if (!m_orderedData)
	{
		std::shuffle(m_randomData.begin(), m_randomData.end(), m_g);
	}

	std::cout << "\nPOINT SEARCH\n";
	long long duration = 0;
	long long durationAT = 0;
	long long durationRB = 0;

	for (int i{}; i < SEARCH_DATA_COUNT; ++i)
	{
		auto start = high_resolution_clock::now();
		m_bst.find(m_randomData[i]);
		auto end = high_resolution_clock::now();
		duration += duration_cast<nanoseconds>(end - start).count();

		auto startAT = high_resolution_clock::now();
		m_at.find(m_randomData[i]);
		auto endAT = high_resolution_clock::now();
		durationAT += duration_cast<nanoseconds>(endAT - startAT).count();

		auto startRB = high_resolution_clock::now();
		m_rb[m_randomData[i]];
		auto endRB = high_resolution_clock::now();
		durationRB += duration_cast<nanoseconds>(endRB - startRB).count();
	}

	std::cout << "Binary search tree\n"
			  << duration / 1000000.0 << " milliseconds\n"
			  << "AVL tree\n"
			  << durationAT / 1000000.0 << " milliseconds\n"
			  << "RB tree\n"
			  << durationRB / 1000000.0 << " milliseconds\n";
}

void SpeedTester::testIntervalSearch()
{
	std::map<Number*, Number*> keys;
	for (int i{}; i < SEARCH_INTERVAL_COUNT; ++i)
	{
		int intervalWidth = SEARCH_INTERVAL + (rand() % SEARCH_INTERVAL_MAX_EXTENSION);
		int min, max;
		min = RANDOM_DATA_COUNT;
		while (min > RANDOM_DATA_COUNT - intervalWidth)
		{
			min = rand() % (RANDOM_DATA_COUNT + 1);
		}
		max = min + intervalWidth;

		keys[new Number(min)] = new Number(max);
	}

	std::vector<Number*> interval;
	interval.reserve(SEARCH_INTERVAL + SEARCH_INTERVAL_MAX_EXTENSION + 1);

	std::cout << "\nINTERVAL SEARCH\n";
	long long duration = 0;
	long long durationAT = 0;
	long long durationRB = 0;

	for (const auto& keyPair : keys)
	{
		auto start = high_resolution_clock::now();
		m_bst.find(keyPair.first, keyPair.second, interval);
		auto end = high_resolution_clock::now();
		duration += duration_cast<nanoseconds>(end - start).count();
		interval.clear();

		auto startAT = high_resolution_clock::now();
		m_at.find(keyPair.first, keyPair.second, interval);
		auto endAT = high_resolution_clock::now();
		durationAT += duration_cast<nanoseconds>(endAT - startAT).count();
		interval.clear();

		auto startRB = high_resolution_clock::now();
		auto itLow = m_rb.lower_bound(keyPair.first);
		auto itHigh = m_rb.upper_bound(keyPair.second);
		for (auto it = itLow; it != itHigh; ++it)
		{
			interval.push_back(it->first);
		}
		auto endRB = high_resolution_clock::now();
		durationRB += duration_cast<nanoseconds>(endRB - startRB).count();
		interval.clear();
	}

	std::cout << "Binary search tree\n"
			  << duration / 1000000.0 << " milliseconds\n"
			  << "AVL tree\n"
			  << durationAT / 1000000.0 << " milliseconds\n"
			  << "RB tree\n"
			  << durationRB / 1000000.0 << " milliseconds\n";

	for (const auto& keyPair : keys)
	{
		delete keyPair.first;
		delete keyPair.second;
	}
}

void SpeedTester::testFindMinKey()
{
	std::cout << "\nSEARCH FOR MIN KEY\n";
	long long duration = 0;
	long long durationAT = 0;
	long long durationRB = 0;

	for (int i{}; i < KEY_SEARCH_COUNT; ++i)
	{
		auto start = high_resolution_clock::now();
		m_bst.findMinKey(m_randomData[rand() % m_randomData.size()]);
		auto end = high_resolution_clock::now();
		duration += duration_cast<nanoseconds>(end - start).count();

		auto startAT = high_resolution_clock::now();
		m_at.findMinKey(m_randomData[rand() % m_randomData.size()]);
		auto endAT = high_resolution_clock::now();
		durationAT += duration_cast<nanoseconds>(endAT - startAT).count();

		auto startRB = high_resolution_clock::now();
		m_rb.begin()->first;
		auto endRB = high_resolution_clock::now();
		durationRB += duration_cast<nanoseconds>(endRB - startRB).count();
	}

	std::cout << "Binary search tree\n"
			  << duration / 1000000.0 << " milliseconds\n"
			  << "AVL tree\n"
			  << durationAT / 1000000.0 << " milliseconds\n"
			  << "RB tree\n"
			  << durationRB / 1000000.0 << " milliseconds\n";
}

void SpeedTester::testFindMaxKey()
{
	std::cout << "\nSEARCH FOR MAX KEY\n";
	long long duration = 0;
	long long durationAT = 0;
	long long durationRB = 0;

	for (int i{}; i < KEY_SEARCH_COUNT; ++i)
	{
		auto start = high_resolution_clock::now();
		m_bst.findMaxKey(m_randomData[rand() % m_randomData.size()]);
		auto end = high_resolution_clock::now();
		duration += duration_cast<nanoseconds>(end - start).count();

		auto startAT = high_resolution_clock::now();
		m_at.findMaxKey(m_randomData[rand() % m_randomData.size()]);
		auto endAT = high_resolution_clock::now();
		durationAT += duration_cast<nanoseconds>(endAT - startAT).count();

		auto startRB = high_resolution_clock::now();
		m_rb.rbegin()->first;
		auto endRB = high_resolution_clock::now();
		durationRB += duration_cast<nanoseconds>(endRB - startRB).count();
	}

	std::cout << "Binary search tree\n"
			  << duration / 1000000.0 << " milliseconds\n"
			  << "AVL tree\n"
			  << durationAT / 1000000.0 << " milliseconds\n"
			  << "RB tree\n"
			  << durationRB / 1000000.0 << " milliseconds\n";
}

SpeedTester::~SpeedTester()
{
	for (auto& num : m_randomData)
	{
		delete num;
	}
}