#pragma once
#include "../BST/BinarySearchTree.h"
#include "AVLNode.h"

template<typename T>
class AVLTree : public BinarySearchTree<T>
{
	//Destructor intentionaly inherited

private:
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

		a->updateBalanceFactor();
		b->updateBalanceFactor();
	}

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

		a->updateBalanceFactor();
		b->updateBalanceFactor();
	}

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
			current->updateBalanceFactor();

			if (!current->isBalanced())
			{
				AVLNode<T>* parent = static_cast<AVLNode<T>*>(current->getAncestor());
				if (current->balanceFactor() < -1)
				{
					if (current->rightChild() == nullptr)
					{
						current = parent;
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
						current = parent;
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
				current = parent;
				continue;
			}
			else if (insert && current != node && current->balanceFactor() == 0)
			{
				return;
			}
			current = static_cast<AVLNode<T>*>(current->getAncestor());
		}
	}

	int test(AVLNode<T>* node)
	{
		if (node == nullptr)
		{
			return 0;
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

	T remove(T& key) override
	{
		RemoveData<T> rd = this->removeNode(key);
		AVLNode<T>* rebalanceStartingNode = static_cast<AVLNode<T>*>(rd.m_removedNodeAncestor);
		rebalance(rebalanceStartingNode);

		return rd.m_data;
	}

	void testTree()
	{
		test(static_cast<AVLNode<T>*>(this->m_root));
	}
};