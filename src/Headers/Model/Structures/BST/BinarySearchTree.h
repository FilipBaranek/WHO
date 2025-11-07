#pragma once
#include <stdexcept>
#include <functional>
#include <stack>
#include <queue>
#include <vector>
#include "BSTNode.h"
#include "../../Interfaces/IComparable.h"


//!
//! @brief Helper structure representing data returned during node removal
//! Stores removed node’s ancestor and its data pointer.
//! @tparam T - Type of the data stored in the node.
//!
template<typename T>
struct RemoveData
{
	BSTNode<T>* m_removedNodeAncestor;	///< Pointer to the ancestor of the removed node
	T m_data;							///< Pointer to the removed data
};

//!
//! @brief Template class implementing a generic Binary Search Tree
//! Provides the basic functionality of a Binary Search Tree, including insertion, removal, and multiple traversal algorithms
//! @tparam T - The type of data stored in the tree nodes.
//! Type T must implement the IComparable interface to ensure data comparability between nodes.
//!
template<typename T>
class BinarySearchTree
{
private:
	unsigned int m_size = 0;	///< Current number of elements in the tree

	//!
	//! @brief Finds a node in the tree matching a given key
	//! @param [in] key - Reference to the key to find
	//! @return - Pointer to the node if found, otherwise nullptr
	//! 
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

	//!
	//! @brief Finds the node with the minimum key in a given subtree
	//! @param [in] startingNode - Subtree root
	//! @return - Pointer to the node with the smallest key
	//! 
	BSTNode<T>* findMinKeyNode(BSTNode<T>* startingNode)
	{
		BSTNode<T>* currentNode = startingNode;
		while (currentNode->leftChild() != nullptr)
		{
			currentNode = currentNode->leftChild();
		}

		return currentNode;
	}

	//!
	//! @brief Finds the node with the maximum key in a given subtree
	//! @param [in] startingNode - Subtree root
	//! @return - Pointer to the node with the largest key
	//! 
	BSTNode<T>* findMaxKeyNode(BSTNode<T>* startingNode)
	{
		BSTNode<T>* currentNode = startingNode;
		while (currentNode->rightChild() != nullptr)
		{
			currentNode = currentNode->rightChild();
		}

		return currentNode;
	}

	//!
	//! @brief Finds the minimum key node within a given key interval
	//! Used in interval queries between two keys.
	//! @return - Pointer to the node with smallest key in interval
	//! 
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

	//!
	//! @brief Finds the maximum key node within a given key interval
	//! Used in interval queries between two keys.
	//! @return - Pointer to the node with largest key in interval
	//! 
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

	//!
	//! @brief Finds the next node in an in order query
	//! @param [in] node - The current node
	//! @param [out] lastAncestor - Reference to the last ancestor visited
	//! @return - Pointer to the next node in sequence
	//! 
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

	//!
	//! @brief Performs a post-order traversal and applies a given function to each node
	//! @param [in] process - Function to apply to each node
	//! @throws std::runtime_error - If the tree is empty
	//! 
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
	BSTNode<T>* m_root;		///< Pointer to the root node of the tree

	//!
	//! @brief Inserts a new node of a given type into the tree
	//! Used internally for both standard BST and derived tree types
	//! @tparam Node - The node type (must inherit from BSTNode<T>)
	//! @param [in] data - Pointer to the data to insert
	//! @return - Pointer to the newly inserted node, or nullptr if a duplicate key was found
	//! 
	template<typename Node>
	BSTNode<T>* insertNode(T data)
	{
		static_assert(std::is_base_of_v<BSTNode<T>, Node>, "Node must inherit from BSTNode<T>");

		if (m_root == nullptr)
		{
			m_root = new Node(data, nullptr);
			++m_size;
			return m_root;
		}

		BSTNode<T>* currentNode = m_root;
		BSTNode<T>* newNode;
		while (true)
		{
			int cmp = currentNode->getData()->compare(data);
			if (cmp == -1)
			{
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
		++m_size;

		return newNode;
	}

	//!
	//! @brief Removes a node with a given key from the tree
	//! Handles all three standard BST deletion cases
	//! @param [in] key - Reference to the key to remove
	//! @return - Structure containing the removed node’s ancestor and data
	//! 
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
	//!
	//! @brief Constructs an empty Binary Search Tree
	//! 
	BinarySearchTree()
	{
		m_root = nullptr;
	}

	//!
	//! @brief Returns the number of nodes currently stored in the tree
	//! @return - The number of nodes in the tree
	//! 
	unsigned int size()
	{
		return m_size;
	}

	//!
	//! @brief Inserts a new node with the given data into the tree
	//! @param [in] data - Pointer to the data to insert
	//! @return - True if the insertion succeeded, false if the key already exists
	//! 
	virtual bool insert(T data)
	{
		return insertNode<BSTNode<T>>(data) != nullptr;
	}

	//!
	//! @brief Finds and returns data matching the given key
	//! @param [in] key - Data key to find
	//! @return - Pointer to the data if found, otherwise nullptr
	//! 
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

	//!
	//! @brief Finds all data within a specified key interval
	//! @param [in] lowestKey - The lower bound key
	//! @param [in] highestKey - The upper bound key
	//! @param [out] outputInterval - Vector to store found data to
	//! 
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

	//!
	//! @brief Finds the smallest key in the tree or a subtree
	//! @param [in] key - Optional key to start the search from (nullptr for global minimum)
	//! @return - Pointer to the smallest data element found, or nullptr if empty
	//! 
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

	//!
	//! @brief Finds the largest key in the tree or a subtree
	//! @param [in] key - Optional key to start the search from (nullptr for global maximum)
	//! @return - Pointer to the largest data element found, or nullptr if empty
	//! 
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

	//!
	//! @brief Removes a node from the tree by its key
	//! @param [in] key - Key of the node to remove
	//! @return - Pointer to the removed data, or nullptr if not found
	//! 
	virtual T remove(T key)
	{
		return removeNode(key).m_data;
	}

	//!
	//! @brief Performs an in order traversal and applies a function to each data element
	//! @param [in] process - Function to apply to each data element
	//! @throws std::runtime_error - If the tree is empty
	//! 
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

	//!
	//! @brief Performs a post order traversal and applies a function to each data element
	//! @param [in] process - Function to apply to each data element
	//! @throws std::runtime_error - If the tree is empty
	//! 
	void processPostOrder(std::function<void(T)> process)
	{
		postOrderTraversal([&](BSTNode<T>* node) {
			process(node->getData());
			});
	}

	//!
	//! @brief Performs a pre order traversal and applies a function to each data element
	//! @param [in] process - Function to apply to each data element
	//! @throws std::runtime_error - If the tree is empty
	//! 
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

	//!
	//! @brief Performs a level order traversal of the tree and applies a function to each data element
	//! @param [in] process - Function to apply to each data element
	//! @throws std::runtime_error - If the tree is empty
	//! 
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

	//!
	//! @brief Removes all nodes from the tree and resets its size
	//! 
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
	}

	//!
	//! @brief Destructor that clears the tree on destruction
	//! 
	~BinarySearchTree()
	{
		if (m_size > 0)
		{
			clear();
		}
	}
};