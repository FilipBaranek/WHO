#pragma once
#include <algorithm>
#include "../BST/BSTNode.h"

template<typename T>
class AVLNode : public BSTNode<T>
{
private:
	int m_balanceFactor;
	unsigned int m_height;

public:
	AVLNode(T data, BSTNode<T>* ancestor) : BSTNode<T>(data, ancestor)
	{
		m_balanceFactor = 0;
		m_height = 1;
	}

	void updateBalanceFactor()
	{
		AVLNode<T>* left = static_cast<AVLNode<T>*>(this->leftChild());
		AVLNode<T>* right = static_cast<AVLNode<T>*>(this->rightChild());

		int leftHeight = left != nullptr ? left->height() : 0;
		int rightHeight = right != nullptr ? right->height() : 0;

		m_height = std::max(leftHeight, rightHeight) + 1;
		m_balanceFactor = leftHeight - rightHeight;
	}

	inline bool isBalanced() const
	{
		return m_balanceFactor >= -1 && m_balanceFactor <= 1;
	}

	inline int balanceFactor() const
	{
		return m_balanceFactor;
	}

	inline int height() const
	{
		return m_height;
	}

	inline ~AVLNode() = default;
};