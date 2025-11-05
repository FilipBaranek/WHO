#pragma once
#include <random>
#include <vector>
#include <chrono>
#include <iostream>
#include <sstream>
#include <map>
#include "./TestObjects/Number.h"
#include "../Structures/BST/BinarySearchTree.h"
#include "../Structures/AVL/AVLTree.h"

using namespace std::chrono;

class SpeedTester
{
private:
	static constexpr const unsigned int RANDOM_DATA_COUNT = 10000000;
	static constexpr const unsigned int REMOVE_DATA_COUNT = 2000000;
	static constexpr const unsigned int SEARCH_DATA_COUNT = 5000000;
	static constexpr const unsigned int SEARCH_INTERVAL_COUNT = 1000000;
	static constexpr const unsigned int SEARCH_INTERVAL = 500;
	static constexpr const unsigned int SEARCH_INTERVAL_MAX_EXTENSION = 200;
	static constexpr const unsigned int KEY_SEARCH_COUNT = 2000000;
	std::ostringstream m_oss;
	std::random_device m_rd;
	std::mt19937 m_g{ m_rd() };
	std::vector<Number*> m_randomData;
	std::map<Number*, int, NumberRBComparator> m_rb;
	BinarySearchTree<Number*> m_bst;
	AVLTree<Number*> m_at;

public:
	SpeedTester(bool dataInOrder = false);
	void testInsertion();
	void testRemoval();
	void testPointSearch();
	void testIntervalSearch();
	void testFindMinKey();
	void testFindMaxKey();
	~SpeedTester();
};