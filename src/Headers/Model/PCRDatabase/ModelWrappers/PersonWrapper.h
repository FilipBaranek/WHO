#pragma once
#include <vector>
#include <chrono>
#include "../../Interfaces/IStorable.h"
#include "../../Interfaces/IComparable.h"
#include "../../Structures/AVL/AVLTree.h"
#include "../Models/Person.h"
#include "../ModelWrappers/TestByDateWrapper.h"

class PersonWrapper : public IComparable, public IStorable
{
private:
	Person* m_person;
	AVLTree<TestByDateWrapper*> m_tests;

public:
	PersonWrapper(Person* person) : m_person(person) {};
	inline Person* getData() { return m_person; }
	inline AVLTree<TestByDateWrapper*>& tests() { return m_tests; }
	int compare(IComparable* other) const override;
	std::string writeLine() override;
	static IStorable* loadLine(std::string line);
	~PersonWrapper() = default;
};