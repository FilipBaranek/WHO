#pragma once
#include "../BST/BinarySearchTree.h"
#include "AVLNode.h"

//!
//! @brief Template class representing an AVL Tree
//! The AVLTree extends the basic functionality of a Binary Search Tree by maintaining
//! balance after insertions and deletions through rotations.
//! @tparam T - The type of data stored in the tree. Must inherit from IComparable to ensure node comparability.
//!
template<typename T>
class AVLTree : public BinarySearchTree<T>
{
private:
	//!
	//! @brief Performs a left rotation around the given node.
	//! Used to restore AVL balance when the right subtree of a node becomes too heavy.
	//! Updates ancestor and child relationships, recalculates balance factors.
	//! @param [in,out] a - The node around which the rotation is performed.
	//!
	void rotateLeft(AVLNode<T>* a)
	{
		AVLNode<T>* b = static_cast<AVLNode<T>*>(a->rightChild());
		AVLNode<T>* ancestor = static_cast<AVLNode<T>*>(a->getAncestor());

		if (b->leftChild() != nullptr)
		{
			b->leftChild()->setAncestor(a);
		}

		a->setRightChild(b->leftChild());
		b->setLeftChild(a);
		a->setAncestor(b);
		b->setAncestor(ancestor);

		if (ancestor == nullptr)
		{
			this->m_root = b;
		}
		else if (ancestor->leftChild() == a)
		{
			ancestor->setLeftChild(b);
		}
		else
		{
			ancestor->setRightChild(b);
		}

		a->updateHeight();
		b->updateHeight();
	}

	//!
	//! @brief Performs a right rotation around the given node.
	//! Used to restore AVL balance when the left subtree of a node becomes too heavy.
	//! Updates ancestor and child relationships, recalculates balance factors.
	//! @param [in,out] a - The node around which the rotation is performed.
	//!
	void rotateRight(AVLNode<T>* a)
	{
		AVLNode<T>* b = static_cast<AVLNode<T>*>(a->leftChild());
		AVLNode<T>* ancestor = static_cast<AVLNode<T>*>(a->getAncestor());

		if (b->rightChild() != nullptr)
		{
			b->rightChild()->setAncestor(a);
		}

		a->setLeftChild(b->rightChild());
		b->setRightChild(a);
		a->setAncestor(b);
		b->setAncestor(ancestor);

		if (ancestor == nullptr)
		{
			this->m_root = b;
		}
		else if (ancestor->leftChild() == a)
		{
			ancestor->setLeftChild(b);
		}
		else
		{
			ancestor->setRightChild(b);
		}

		a->updateHeight();
		b->updateHeight();
	}

	//!
	//! @brief Rebalances the AVL tree starting from the given node upward.
	//! Traverses from the specified node up until the tree is balanced, updating balance factors and
	//! performing rotations when necessary to maintain AVL balance properties.
	//! @param [in,out] node - The starting node for rebalancing.
	//! @param [in] insert - Indicates whether the rebalance is triggered by an insertion
	//!
	void rebalance(AVLNode<T>* node, bool insert = false)
	{
		int beforeDeleteBalanceFactor;
		AVLNode<T>* current = node;
		while (current != nullptr)
		{
			if (!insert)
			{
				beforeDeleteBalanceFactor = current->balanceFactor();
			}
			current->updateHeight();

			if (!current->isBalanced())
			{
				if (current->balanceFactor() < -1)
				{
					if (current->rightChild() == nullptr)
					{
						current = static_cast<AVLNode<T>*>(current->getAncestor());
						continue;
					}
					if (static_cast<AVLNode<T>*>(current->rightChild())->balanceFactor() <= 0)
					{
						rotateLeft(current);
					}
					else
					{
						rotateRight(static_cast<AVLNode<T>*>(current->rightChild()));
						rotateLeft(current);
					}
					if (insert || (!insert && beforeDeleteBalanceFactor == 0 && (current->balanceFactor() == 1 || current->balanceFactor() == -1)))
					{
						return;
					}
				}
				else if (current->balanceFactor() > 1)
				{
					if (static_cast<AVLNode<T>*>(current->leftChild()) == nullptr)
					{
						current = static_cast<AVLNode<T>*>(current->getAncestor());
						continue;
					}
					if (static_cast<AVLNode<T>*>(current->leftChild())->balanceFactor() >= 0)
					{
						rotateRight(current);
					}
					else
					{
						rotateLeft(static_cast<AVLNode<T>*>(current->leftChild()));
						rotateRight(current);
					}
					if (insert || (!insert && beforeDeleteBalanceFactor == 0 && (current->balanceFactor() == 1 || current->balanceFactor() == -1)))
					{
						return;
					}
				}
			}
			else if (insert && current != node && current->balanceFactor() == 0)
			{
				return;
			}
			current = static_cast<AVLNode<T>*>(current->getAncestor());
		}
	}

	//!
	//! @brief Recursively verifies AVL tree correctness.
	//! Traverses the tree and validates each node’s height and balance factor
	//! against expected values derived from its children.
	//! @param [in] node - The node to test from
	//! @return - The height of the current subtree.
	//! @throws std::runtime_error - If any node violates AVL properties.
	//!
	int test(AVLNode<T>* node)
	{
		if (node == nullptr)
		{
			return -1;
		}

		int leftHeight = test(static_cast<AVLNode<T>*>(node->leftChild()));
		int rightHeight = test(static_cast<AVLNode<T>*>(node->rightChild()));

		int expectedBalance = leftHeight - rightHeight;
		if (node->balanceFactor() != expectedBalance)
		{
			throw std::runtime_error("Balance factor doesn't match expected balance factor");
		}

		int expectedHeight = std::max(leftHeight, rightHeight) + 1;
		if (node->height() != expectedHeight)
		{
			throw std::runtime_error("Height doesn't match expected height");
		}

		if (expectedBalance < -1 || expectedBalance > 1)
		{
			throw std::runtime_error("Node isn't properly balanced");
		}

		return expectedHeight;
	}

public:
	//!
	//! @brief Inserts a new element into the AVL tree.
	//! If insertion is successful, the tree is automatically rebalanced.
	//! @param [in] data - The value to insert.
	//! @return - True if insertion succeeded, false if the element already exists.
	//!
	bool insert(T data) override
	{
		AVLNode<T>* insertedNode = static_cast<AVLNode<T>*>(this->insertNode<AVLNode<T>>(data));
		if (insertedNode == nullptr)
		{
			return false;
		}

		rebalance(insertedNode, true);
		return true;
	}

	//!
	//! @brief Removes an element from the AVL tree by key.
	//! The tree is automatically rebalanced after deletion.
	//! @param [in] key - The value to remove.
	//! @return - The data stored from the removed node.
	//!
	T remove(T key) override
	{
		RemoveData<T> rd = this->removeNode(key);
		AVLNode<T>* rebalanceStartingNode = static_cast<AVLNode<T>*>(rd.m_removedNodeAncestor);
		rebalance(rebalanceStartingNode);

		return rd.m_data;
	}

	//!
	//! @brief Tests the entire tree for AVL property correctness.
	//! @throws std::runtime_error - If any node is imbalanced or inconsistent.
	//!
	void testTree()
	{
		test(static_cast<AVLNode<T>*>(this->m_root));
	}
};