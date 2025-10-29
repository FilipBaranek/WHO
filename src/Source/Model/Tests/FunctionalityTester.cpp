#include "../../../Headers/Model/Tests/FunctionalityTester.h"


FunctionalityTester::FunctionalityTester(unsigned int pregeneratedDataCount)
{
	srand(time(nullptr));

	m_minimalKey = 999999999;
	m_maximalKey = -1;

	while (m_bst.size() != pregeneratedDataCount || m_at.size() != pregeneratedDataCount)
	{
		insert();
	}
}

void FunctionalityTester::insert()
{
	auto number = new Number(rand() % VALUE_UPPER_BOUND);
	bool duplicity = false;
	if (std::find_if(m_data.begin(), m_data.end(), [number](Number* n) { return n->getData() == number->getData(); }) != m_data.end())
	{
		duplicity = true;
	}
	else
	{
		m_data.push_back(number);

		if (number->getData() < m_minimalKey)
		{
			m_minimalKey = number->getData();
		}
		if (number->getData() > m_maximalKey)
		{
			m_maximalKey = number->getData();
		}
	}
	bool btInserted = m_bst.insert(number);
	bool atInserted = m_at.insert(number);

	auto bstFindNum = m_bst.find(number);
	auto atFindNum = m_at.find(number);

	if ((duplicity && (btInserted || atInserted)) ||
		(!duplicity && (bstFindNum->getData() != number->getData() || atFindNum->getData() != number->getData())))
	{
		throw std::runtime_error("Incorrect inserting");
	}

	if (duplicity)
	{
		delete number;
		insert();
	}
}

void FunctionalityTester::remove()
{
	int index = rand() % m_data.size();
	std::swap(m_data[index], m_data.back());
	m_bst.remove(m_data.back());
	m_at.remove(m_data.back());

	if (m_bst.find(m_data.back()) != nullptr || m_at.find(m_data.back()) != nullptr)
	{
		throw std::runtime_error("Incorrect removal");
	}

	delete m_data.back();
	m_data.pop_back();

	m_minimalKey = 999999999;
	for (int i{}; i < m_data.size(); ++i)
	{
		if (m_data[i]->getData() < m_minimalKey)
		{
			m_minimalKey = m_data[i]->getData();
		}
	}
	m_maximalKey = -1000;
	for (int i{}; i < m_data.size(); ++i)
	{
		if (m_data[i]->getData() > m_maximalKey)
		{
			m_maximalKey = m_data[i]->getData();
		}
	}
}

void FunctionalityTester::find()
{
	auto number = m_data[rand() % m_data.size()];
	Number* bstKey = m_bst.find(number);
	Number* atKey = m_at.find(number);

	if (bstKey != number || atKey != number)
	{
		throw std::runtime_error("Incorrect point search");
	}
}

void FunctionalityTester::findInterval()
{
	int interval = (rand() % MAX_INTERVAL) + 1;

	if (m_data.size() > 0)
	{
		int min, max;
		min = rand() % VALUE_UPPER_BOUND;
		max = min + interval;

		std::vector<Number*> numbersInInterval;
		for (auto n : m_data)
		{
			if (n->getData() >= min && n->getData() <= max)
			{
				numbersInInterval.push_back(n);
			}
		}
		std::sort(numbersInInterval.begin(), numbersInInterval.end(), [](Number* a, Number* b) {
			return a->getData() < b->getData();
			});

		auto low = new Number(min);
		auto high = new Number(max);
		std::vector<Number*> btOutput;
		std::vector<Number*> atOutput;
		m_bst.find(low, high, btOutput);
		m_at.find(low, high, atOutput);

		if (numbersInInterval.size() != btOutput.size() || numbersInInterval.size() != atOutput.size())
		{
			throw std::runtime_error("Interval size doesn't match");
		}

		for (int i{}; i < numbersInInterval.size(); ++i)
		{
			if (numbersInInterval[i]->getData() != btOutput[i]->getData() || numbersInInterval[i]->getData() != atOutput[i]->getData())
			{
				throw std::runtime_error("Incorrect interval search");
			}
		}
		delete low;
		delete high;
	}
}

void FunctionalityTester::findMinKey()
{
	if (m_data.size() > 0)
	{
		Number* bstMinKey = m_bst.findMinKey();
		Number* atMinKey = m_at.findMinKey();

		if (bstMinKey->getData() != m_minimalKey || atMinKey->getData() != m_minimalKey)
		{
			throw std::runtime_error("Incorrect minimal key search");
		}
	}
}

void FunctionalityTester::findMaxKey()
{
	if (m_data.size() > 0)
	{
		Number* bstMaxKey = m_bst.findMaxKey();
		Number* atMaxKey = m_at.findMaxKey();

		if (bstMaxKey->getData() != m_maximalKey || atMaxKey->getData() != m_maximalKey)
		{
			throw std::runtime_error("Incorrect maximal key search");
		}
	}
}

void FunctionalityTester::testAVL()
{
	m_at.testTree();
}

void FunctionalityTester::runTests()
{
	m_oss.clear();

	for (int i{}; i < REPLICATIONS; ++i)
	{
		if (i % CHECKPOINT_INDEX == 0)
		{
			m_oss << "OPERATION N." << (i + 1) << "\n"
				  << "BINARY TREE SIZE: " << m_bst.size() << "\n"
				  << "AVL TREE SIZE: " << m_at.size() << "\n\n";

			//std::cout << "OPERATION N." << i << "\n";
			//std::cout << "BINARY TREE SIZE: " << m_bst.size() << "\n";
			//std::cout << "AVL SIZE: " << m_at.size() << "\n";

			if (m_data.size() == 0)
			{
				insert();
			}
			findMinKey();
			findMaxKey();
		}
		else
		{
			int operation = rand() % 100;

			if (operation < 20)
			{
				insert();
			}
			else if (operation < 40)
			{
				if (m_data.size() > 0)
				{
					remove();
				}
			}
			else if (operation < 60)
			{
				if (m_data.size() > 0)
				{
					find();
				}
			}
			else if (operation < 80)
			{
				if (m_data.size() > 0)
				{
					findInterval();
				}
			}
			else
			{
				if (m_data.size() > 0)
				{
					testAVL();
				}
			}
		}
	}

	m_oss << "\nTest ran successfuly\n";
}

std::string FunctionalityTester::outputToString()
{
	return m_oss.str();
}

FunctionalityTester::~FunctionalityTester()
{
	if (m_data.size() > 0)
	{
		for (auto& number : m_data)
		{
			delete number;
		}
	}
}
