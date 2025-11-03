#pragma once
#include "../../Structures/AVL/AVLTree.h"
#include "../../Interfaces/IStorable.h"
#include "TestWrapper.h"

class LocationWrapper : public IComparable, public IStorable
{
private:
	unsigned int m_locationId;
	AVLTree<TestWrapper*> m_positiveTests;
	AVLTree<TestWrapper*> m_negativeTests;

public:
	LocationWrapper(unsigned int locationId) : m_locationId(locationId) {};
	inline unsigned int locationId() { return m_locationId; }
	inline AVLTree<TestWrapper*>& positiveTests() { return m_positiveTests; }
	inline AVLTree<TestWrapper*>& negativeTests() { return m_negativeTests; }
	int compare(IComparable* other) const override;
	void writeLine() override;
	IStorable* loadLine() override;
	virtual ~LocationWrapper() = default;
};