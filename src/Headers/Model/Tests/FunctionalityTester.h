#pragma once
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include "../Structures/BST/BinarySearchTree.h"
#include "../Structures/AVL/AVLTree.h"
#include "./TestObjects/Number.h"

class FunctionalityTester
{
private:
	static constexpr const unsigned int REPLICATIONS = 500000;
	static constexpr const unsigned int CHECKPOINT_INDEX = 10000;
	static constexpr const unsigned int VALUE_UPPER_BOUND = 500000;
	static constexpr const unsigned int MAX_INTERVAL = 500;
	int m_minimalKey;
	int m_maximalKey;
	std::ostringstream m_oss;
	std::vector<Number*> m_data;
	BinarySearchTree<Number*> m_bst;
	AVLTree<Number*> m_at;
	std::random_device m_rd;
	std::mt19937 m_gen{ m_rd() };

public:
	FunctionalityTester(unsigned int pregeneratedDataCount = 0);
	void insert();
	void remove();
	void find();
	void findInterval();
	void findMinKey();
	void findMaxKey();
	void testAVL();
	void runTests();
	std::string outputToString();
	~FunctionalityTester();
};