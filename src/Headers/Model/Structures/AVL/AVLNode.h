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
	int m_height;		///< Height of this node within the tree

public:
	//!
	//! @brief Constructs a new AVLNode with given data and ancestor
	//! @param [in] data - Pointer to data to store in this node
	//! @param [in] ancestor - Pointer to the parent node (can be nullptr for the root)
	//! 
	AVLNode(T data, BSTNode<T>* ancestor) : BSTNode<T>(data, ancestor)
	{
		m_height = 0;
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
	//! @brief Updates height of the node
	//! 
	void updateHeight()
	{
		int left = leftHeight();
		int right = rightHeight();

		m_height = std::max(left, right) + 1;
	}

	//!
	//! @brief Retrieves height of the left subtree
	//! @return - Height of the left subtree
	//! 
	int leftHeight()
	{
		AVLNode<T>* left = static_cast<AVLNode<T>*>(this->leftChild());
		return left != nullptr ? left->height() : -1;
	}

	//!
	//! @brief Retrieves height of the right subtree
	//! @return - Height of the right subtree
	//! 
	int rightHeight()
	{
		AVLNode<T>* right = static_cast<AVLNode<T>*>(this->rightChild());
		return right != nullptr ? right->height() : -1;
	}

	//!
	//! @brief Retrieves the current balance factor of this node
	//! @return - Balance factor
	//! 
	inline int balanceFactor()
	{
		return leftHeight() - rightHeight();
	}

	//!
	//! @brief Checks whether the node is balanced according to AVL rules
	//! @return - True if the balance factor is between -1 and 1
	//! 
	inline bool isBalanced()
	{
		int balanceFact = balanceFactor();
		return balanceFact >= -1 && balanceFact <= 1;
	}

	//!
	//! @brief Default destructor
	//! 
	inline ~AVLNode() = default;
};