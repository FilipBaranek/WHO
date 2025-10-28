#pragma once
#include <vector>
#include <time.h>
#include <iostream>
#include "../Structures/BST/BinarySearchTree.h"
#include "../Structures/AVL/AVLTree.h"
#include "./TestObjects/Number.h"

class FunctionalityTester
{
private:
	static constexpr const unsigned int INSERT = 0;
	static constexpr const unsigned int REMOVE = 1;
	static constexpr const unsigned int FIND_POINT = 2;
	static constexpr const unsigned int FIND_INTERVAL = 3;
	static constexpr const unsigned int TEST_AVL = 4;
	static constexpr const unsigned int FIND_MIN_KEY = 5;
	static constexpr const unsigned int FIND_MAX_KEY = 6;
	static constexpr const unsigned int REPLICATIONS = 10000000;
	static constexpr const unsigned int CHECKPOINT_INDEX = 100000;
	static constexpr const unsigned int UPPER_BOUND_COUNT = 5000;
	static constexpr const unsigned int VALUE_UPPER_BOUND = 500000;
	static constexpr const unsigned int MAX_INTERVAL = 500;
	int m_minimalKey;
	int m_maximalKey;
	std::vector<Number*> m_data;
	BinarySearchTree<Number*> m_bst;
	AVLTree<Number*> m_at;

public:
	FunctionalityTester();
	void insert();
	void remove();
	void find();
	void findInterval();
	void findMinKey();
	void findMaxKey();
	void testAVL();
	inline int generateOperation();
	void runTests();
	~FunctionalityTester();
};