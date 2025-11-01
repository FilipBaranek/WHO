#pragma once
#include <stdexcept>
#include <functional>
#include <stack>
#include <queue>
#include <vector>
#include "BSTNode.h"
#include "../../Interfaces/IComparable.h"

template<typename T>
struct RemoveData
{
	BSTNode<T>* m_removedNodeAncestor;
	T m_data;
};

template<typename T>
class BinarySearchTree
{
private:

	unsigned int m_size = 0;
	unsigned int m_depth = 0;

	BSTNode<T>* findNode(T& key)
	{
		if (m_root == nullptr)
		{
			return nullptr;
		}

		BSTNode<T>* currentNode = m_root;
		while (true)
		{
			int cmp = currentNode->getData()->compare(key);

			if (cmp == 0)
			{
				return currentNode;
			}
			else if (cmp == -1)
			{
				if (currentNode->leftChild() == nullptr)
				{
					return nullptr;
				}
				currentNode = currentNode->leftChild();
			}
			else if (cmp == 1)
			{
				if (currentNode->rightChild() == nullptr)
				{
					return nullptr;
				}
				currentNode = currentNode->rightChild();
			}
		}
	}

	BSTNode<T>* findMinKeyNode(BSTNode<T>* startingNode)
	{
		BSTNode<T>* currentNode = startingNode;
		while (currentNode->leftChild() != nullptr)
		{
			currentNode = currentNode->leftChild();
		}

		return currentNode;
	}

	BSTNode<T>* findMaxKeyNode(BSTNode<T>* startingNode)
	{
		BSTNode<T>* currentNode = startingNode;
		while (currentNode->rightChild() != nullptr)
		{
			currentNode = currentNode->rightChild();
		}

		return currentNode;
	}

	BSTNode<T>* findIntervalMinKeyNode(T& key)
	{
		if (m_size == 1)
		{
			return m_root->getData()->compare(key) > 0 ? nullptr : m_root;
		}

		BSTNode<T>* currentNode = m_root;
		BSTNode<T>* minFound = m_root;
		while (true)
		{
			int cmp = currentNode->getData()->compare(key);

			if (cmp == 0)
			{
				return currentNode;
			}
			else if (cmp == -1)
			{
				if (currentNode->leftChild() == nullptr)
				{
					return currentNode;
				}
				minFound = currentNode;
				currentNode = currentNode->leftChild();
			}
			else if (cmp == 1)
			{
				if (currentNode->rightChild() == nullptr)
				{
					if (currentNode->getData()->compare(minFound->getData()) == 1)
					{
						return minFound;
					}
					return currentNode->getAncestor();
				}
				currentNode = currentNode->rightChild();
			}
		}
	}

	BSTNode<T>* findIntervalMaxKeyNode(T& key)
	{
		if (m_size == 1)
		{
			return m_root->getData()->compare(key) < 0 ? nullptr : m_root;
		}

		BSTNode<T>* currentNode = m_root;
		BSTNode<T>* maxFound = m_root;
		while (true)
		{
			int cmp = currentNode->getData()->compare(key);

			if (cmp == 0)
			{
				return currentNode;
			}
			else if (cmp == -1)
			{
				if (currentNode->leftChild() == nullptr)
				{
					if (currentNode->getData()->compare(maxFound->getData()) == -1)
					{
						return maxFound;
					}
					return currentNode->getAncestor();
				}
				currentNode = currentNode->leftChild();
			}
			else if (cmp == 1)
			{
				if (currentNode->rightChild() == nullptr)
				{
					return currentNode;
				}
				maxFound = currentNode;
				currentNode = currentNode->rightChild();
			}
		}
	}

	BSTNode<T>* inorderNext(BSTNode<T>* node, BSTNode<T>*& lastAncestor)
	{
		BSTNode<T>* current = node;

		if (current->leftChild() != nullptr && current != lastAncestor)
		{
			current = findMinKeyNode(current);
		}
		else if (current->rightChild() != nullptr)
		{
			current = current->rightChild();
			current = findMinKeyNode(current);
		}
		else
		{
			while (current->getAncestor() != nullptr && current == current->getAncestor()->rightChild())
			{
				current = current->getAncestor();
			}
			current = current->getAncestor();
			lastAncestor = current;
		}

		return current;
	}

	void postOrderTraversal(std::function<void(BSTNode<T>*)> process)
	{
		if (m_root == nullptr)
		{
			throw std::runtime_error("Trying to process empty tree");
		}

		std::stack<BSTNode<T>*> stack;
		BSTNode<T>* lastNode = nullptr;
		BSTNode<T>* currentNode = m_root;

		while (!stack.empty() || currentNode != nullptr)
		{
			if (currentNode != nullptr)
			{
				stack.push(currentNode);
				currentNode = currentNode->leftChild();
			}
			else
			{
				BSTNode<T>* topNode = stack.top();

				if (topNode->rightChild() != nullptr && lastNode != topNode->rightChild())
				{
					currentNode = topNode->rightChild();
				}
				else
				{
					process(topNode);
					lastNode = topNode;
					stack.pop();
				}
			}
		}
	}

protected:
	BSTNode<T>* m_root;

	template<typename Node>
	BSTNode<T>* insertNode(T data)
	{
		static_assert(std::is_base_of_v<BSTNode<T>, Node>, "Node must inherit from BSTNode<T>");

		if (m_root == nullptr)
		{
			m_root = new Node(data, nullptr);
			m_depth = 1;
			++m_size;
			return m_root;
		}

		unsigned int depth = 1;
		BSTNode<T>* currentNode = m_root;
		BSTNode<T>* newNode;
		while (true)
		{
			int cmp = currentNode->getData()->compare(data);
			if (cmp == -1)
			{
				++depth;
				if (currentNode->leftChild() == nullptr)
				{
					newNode = new Node(data, currentNode);
					currentNode->setLeftChild(newNode);
					break;
				}
				currentNode = currentNode->leftChild();
			}
			else if (cmp == 1)
			{
				++depth;
				if (currentNode->rightChild() == nullptr)
				{
					newNode = new Node(data, currentNode);
					currentNode->setRightChild(newNode);
					break;
				}
				currentNode = currentNode->rightChild();
			}
			else
			{
				return nullptr;
			}
		}

		if (depth > m_depth)
		{
			m_depth = depth;
		}
		++m_size;

		return newNode;
	}

	RemoveData<T> removeNode(T& key)
	{
		BSTNode<T>* currentNode = findNode(key);
		if (currentNode == nullptr)
		{
			return {};
		}

		T data = currentNode->getData();
		BSTNode<T>* ancestor = currentNode->getAncestor();
		int childCount = currentNode->childCount();

		if (childCount == 0)
		{
			if (ancestor == nullptr)
			{
				m_root = nullptr;
			}
			else if (ancestor->leftChild() == currentNode)
			{
				ancestor->setLeftChild(nullptr);
			}
			else
			{
				ancestor->setRightChild(nullptr);
			}
			delete currentNode;
		}
		else if (childCount == 1)
		{
			BSTNode<T>* child = currentNode->leftChild() != nullptr ? currentNode->leftChild() : currentNode->rightChild();
			if (ancestor == nullptr)
			{
				m_root = child;
				child->setAncestor(nullptr);
			}
			else
			{
				if (ancestor->leftChild() == currentNode)
				{
					ancestor->setLeftChild(child);
				}
				else
				{
					ancestor->setRightChild(child);
				}
				child->setAncestor(ancestor);
			}
			delete currentNode;
		}
		else
		{
			BSTNode<T>* swapNode = findMinKeyNode(currentNode->rightChild());
			currentNode->setData(swapNode->getData());
			BSTNode<T>* swapNodeAncestor = swapNode->getAncestor();
			BSTNode<T>* swapNodeChild = swapNode->rightChild();	//Only possible child

			if (swapNodeAncestor->leftChild() == swapNode)
			{
				swapNodeAncestor->setLeftChild(swapNodeChild);
			}
			else
			{
				swapNodeAncestor->setRightChild(swapNodeChild);
			}

			if (swapNodeChild != nullptr)
			{
				swapNodeChild->setAncestor(swapNodeAncestor);
			}

			ancestor = swapNodeAncestor;
			delete swapNode;
		}

		--m_size;
		return { ancestor, data };
	}

public:
	BinarySearchTree()
	{
		m_root = nullptr;
		m_depth = 0;
	}

	unsigned int size()
	{
		return m_size;
	}

	unsigned int depth()
	{
		return m_depth;
	}

	virtual bool insert(T data)
	{
		return insertNode<BSTNode<T>>(data) != nullptr;
	}

	T find(T key)
	{
		if (key == nullptr)
		{
			return nullptr;
		}

		BSTNode<T>* value = findNode(key);
		if (value == nullptr)
		{
			return nullptr;
		}
		return value->getData();
	}

	void find(T lowestKey, T highestKey, std::vector<T>& outputInterval)
	{
		if (m_root == nullptr || lowestKey->compare(highestKey) == -1)
		{
			return;
		}
		else if (lowestKey->compare(highestKey) == 0)
		{
			T output = find(lowestKey);

			if (output != nullptr)
			{
				outputInterval.push_back(output);
			}
			return;
		}

		BSTNode<T>* current = findIntervalMinKeyNode(lowestKey);
		BSTNode<T>* intervalMax = findIntervalMaxKeyNode(highestKey);
		BSTNode<T>* lastAncestor = current != nullptr && current->leftChild() != nullptr ? current : nullptr;

		if (current == nullptr || intervalMax == nullptr || intervalMax->getData()->compare(lowestKey) == 1 ||
			current->getData()->compare(highestKey) == -1 || ((current->rightChild() == intervalMax ||
				intervalMax->leftChild() == current) && current->getData()->compare(lowestKey) == 1 && intervalMax->getData()->compare(highestKey) == -1))
		{
			return;
		}

		while (current != nullptr && current != intervalMax)
		{
			outputInterval.push_back(current->getData());
			current = inorderNext(current, lastAncestor);
		}

		if (current != nullptr)
		{
			outputInterval.push_back(current->getData());
		}
	}

	T findMinKey(T key = nullptr)
	{
		if (key == nullptr)
		{
			if (m_root == nullptr)
			{
				return nullptr;
			}
			return findMinKeyNode(m_root)->getData();
		}

		BSTNode<T>* node = findNode(key);
		if (node == nullptr)
		{
			return nullptr;
		}
		return findMinKeyNode(node)->getData();
	}

	T findMaxKey(T key = nullptr)
	{
		if (key == nullptr)
		{
			if (m_root == nullptr)
			{
				return nullptr;
			}
			return findMaxKeyNode(m_root)->getData();
		}

		BSTNode<T>* node = findNode(key);
		if (node == nullptr)
		{
			return nullptr;
		}
		return findMaxKeyNode(node)->getData();
	}

	virtual T remove(T key)
	{
		return removeNode(key).m_data;
	}

	void processInOrder(std::function<void(T)> process)
	{
		if (m_root == nullptr)
		{
			throw std::runtime_error("Trying to process empty tree");
		}

		BSTNode<T>* currentNode = m_root;
		while (currentNode != nullptr)
		{
			if (currentNode->leftChild() == nullptr)
			{
				process(currentNode->getData());
				currentNode = currentNode->rightChild();
			}
			else
			{
				BSTNode<T>* predecessor = currentNode->leftChild();
				while (predecessor->rightChild() != nullptr && predecessor->rightChild() != currentNode)
				{
					predecessor = predecessor->rightChild();
				}

				if (predecessor->rightChild() == nullptr)
				{
					predecessor->setRightChild(currentNode);
					currentNode = currentNode->leftChild();
				}
				else
				{
					predecessor->setRightChild(nullptr);
					process(currentNode->getData());
					currentNode = currentNode->rightChild();
				}
			}
		}
	}

	void processPostOrder(std::function<void(T)> process)
	{
		postOrderTraversal([&](BSTNode<T>* node) {
			process(node->getData());
			});
	}

	void processPreOrder(std::function<void(T)> process)
	{
		if (m_root == nullptr)
		{
			throw std::runtime_error("Trying to process empty tree");
		}

		std::stack<BSTNode<T>*> stack;
		stack.push(m_root);

		while (!stack.empty())
		{
			BSTNode<T>* currentNode = stack.top();
			stack.pop();

			process(currentNode->getData());

			if (currentNode->rightChild() != nullptr)
			{
				stack.push(currentNode->rightChild());
			}
			if (currentNode->leftChild() != nullptr)
			{
				stack.push(currentNode->leftChild());
			}
		}
	}

	void processLevelOrder(std::function<void(T)> process)
	{
		if (m_root == nullptr)
		{
			throw std::runtime_error("Trying to process empty tree");
		}

		std::queue<BSTNode<T>*> queue;
		queue.push(m_root);

		while (!queue.empty())
		{
			BSTNode<T>* currentNode = queue.front();
			queue.pop();

			process(currentNode->getData());

			if (currentNode->leftChild() != nullptr)
			{
				queue.push(currentNode->leftChild());
			}
			if (currentNode->rightChild() != nullptr)
			{
				queue.push(currentNode->rightChild());
			}
		}
	}

	void clear()
	{
		if (m_size > 0)
		{
			postOrderTraversal([&](BSTNode<T>* node) {
				delete node;
			});
		}

		m_root = nullptr;
		m_size = 0;
		m_depth = 0;
	}

	~BinarySearchTree()
	{
		if (m_size > 0)
		{
			clear();
		}
	}
};