#pragma once
#include <utility>
#include "../../Interfaces/IComparable.h"


//!
//! @brief Template class representing a single node in a Binary Search Tree
//! Each node stores data, references to its ancestor (parent), and its left/right child nodes
//! @tparam T - The type of data stored in the node. Must inherit from IComparable to ensure comparability between nodes
//!
template<typename T>
class BSTNode
{
	static_assert(std::is_base_of_v<IComparable, std::remove_pointer_t<T>>, "T must inherit from IComparable");

private:
	T m_data;					///< Pointer holding data stored in this node
	BSTNode<T>* m_ancestor;		///< Pointer to the parent node
	BSTNode<T>* m_leftChild;	///< Pointer to the left child node
	BSTNode<T>* m_rightChild;	///< Pointer to the right child node

public:
	//!
	//! @brief Constructs a new BSTNode with given data and ancestor
	//! @param [in] data - Pointer to data to store in this node
	//! @param [in] ancestor - Pointer to the parent node. Can be nullptr for the root
	//! 
	BSTNode(T data, BSTNode<T>* ancestor)
	{
		m_data = data;
		m_ancestor = ancestor;
		m_leftChild = nullptr;
		m_rightChild = nullptr;
	}

	//!
	//! @brief Sets pointer to the data stored in the node
	//! @param [in] data - New pointer value to assign
	//! 
	inline void setData(T data)
	{
		m_data = data; 
	}

	//!
	//! @brief Sets the ancestor node
	//! @param [in] ancestor - Pointer to the new ancestor node
	//! 
	inline void setAncestor(BSTNode* ancestor)
	{
		m_ancestor = ancestor;
	}

	//!
	//! @brief Sets the right child of this node
	//! @param [in] rightChild - Pointer to the new right child node
	//! 
	inline void setRightChild(BSTNode* rightChild)
	{
		m_rightChild = rightChild;
	}

	//!
	//! @brief Sets the left child of this node
	//! @param [in] leftChild - Pointer to the new left child node
	//! 
	inline void setLeftChild(BSTNode* leftChild)
	{
		m_leftChild = leftChild;
	}

	//!
	//! @brief Returns the number of non null children this node has
	//! @return - Count of children
	//! 
	int childCount()
	{
		int count = 0;

		if (m_leftChild != nullptr)
		{
			++count;
		}
		if (m_rightChild != nullptr)
		{
			++count;
		}

		return count;
	}

	//!
	//! @brief Retrieves the data stored in this node
	//! @return - Pointer to the data
	//! 
	inline T getData()
	{
		return m_data;
	}

	//!
	//! @brief Gets a reference to the ancestor pointer
	//! @return - Reference to the ancestor node pointer
	//! 
	inline BSTNode*& getAncestor()
	{
		return m_ancestor;
	}

	//!
	//! @brief Gets a reference to the left child pointer
	//! @return - Reference to the left child node pointer
	//! 
	inline BSTNode*& leftChild()
	{
		return m_leftChild;
	}

	//!
	//! @brief Gets a reference to the right child pointer
	//! @return - Reference to the right child node pointer
	//! 
	inline BSTNode*& rightChild()
	{
		return m_rightChild;
	}

	//!
	//! @brief Default destructor
	//! 
	inline ~BSTNode() = default;
};