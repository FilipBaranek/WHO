#pragma once
#include "../../Structures/AVL/AVLTree.h"
#include "TestByDateWrapper.h"

class LocationWrapper : public IComparable
{
private:
	unsigned int m_locationId;
	AVLTree<TestByDateWrapper*> m_positiveTests;
	AVLTree<TestByDateWrapper*> m_negativeTests;

public:
	LocationWrapper(unsigned int locationId) : m_locationId(locationId) {};
	inline unsigned int locationId() { return m_locationId; }
	inline AVLTree<TestByDateWrapper*>& positiveTests() { return m_positiveTests; }
	inline AVLTree<TestByDateWrapper*>& negativeTests() { return m_negativeTests; }
	int compare(IComparable* other) const override;
	virtual ~LocationWrapper() = default;
};