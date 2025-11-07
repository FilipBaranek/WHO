#pragma once
#include <algorithm>
#include "../BST/BSTNode.h"

//!
//! @brief Template class representing a single node in an AVL Tree
//! This class extends the functionality of a Binary Search Tree node
//! by including height and balance factor properties used to maintain
//! AVL tree balance after insertions or deletions.
//! @tparam T - The type of data stored in the node. Must inherit from IComparable to ensure comparability between nodes.
//!
template<typename T>
class AVLNode : public BSTNode<T>
{
private:
	int m_balanceFactor;		///< Difference in height between the left and right subtrees
	unsigned int m_height;		///< Height of this node within the tree

public:
	//!
	//! @brief Constructs a new AVLNode with given data and ancestor
	//! @param [in] data - Pointer to data to store in this node
	//! @param [in] ancestor - Pointer to the parent node (can be nullptr for the root)
	//! 
	AVLNode(T data, BSTNode<T>* ancestor) : BSTNode<T>(data, ancestor)
	{
		m_balanceFactor = 0;
		m_height = 1;
	}

	//!
	//! @brief Updates both the height and balance factor of this node
	//! Calculates heights of the left and right subtrees, then updates the node’s own height and balance factor accordingly.
	//! 
	void updateBalanceFactor()
	{
		AVLNode<T>* left = static_cast<AVLNode<T>*>(this->leftChild());
		AVLNode<T>* right = static_cast<AVLNode<T>*>(this->rightChild());

		int leftHeight = left != nullptr ? left->height() : 0;
		int rightHeight = right != nullptr ? right->height() : 0;

		m_height = std::max(leftHeight, rightHeight) + 1;
		m_balanceFactor = leftHeight - rightHeight;
	}

	//!
	//! @brief Checks whether the node is balanced according to AVL rules
	//! @return - True if the balance factor is between -1 and 1
	//! 
	inline bool isBalanced() const
	{
		return m_balanceFactor >= -1 && m_balanceFactor <= 1;
	}

	//!
	//! @brief Retrieves the current balance factor of this node
	//! @return - Balance factor
	//! 
	inline int balanceFactor() const
	{
		return m_balanceFactor;
	}

	//!
	//! @brief Retrieves the current height of this node
	//! @return - Height of the node
	//! 
	inline int height() const
	{
		return m_height;
	}

	//!
	//! @brief Default destructor
	//! 
	inline ~AVLNode() = default;
};