#include "Multimap.h"
using namespace std;

MultiMap::Iterator::Iterator()
{
	root_BTnode = nullptr;
	cur_BTnode = nullptr;
	cur_LinkedValue = nullptr;
}

MultiMap::Iterator::Iterator(BTnode* i_root, BTnode* i_node)
{
	root_BTnode = i_root;
	cur_BTnode = i_node;
	cur_LinkedValue = i_node->head_LinkedValue;
}

bool MultiMap::Iterator::valid() const
{
	if (cur_BTnode == nullptr)
		return false;
	return true;
}

string MultiMap::Iterator::getKey() const
{
	if (!valid())
		return "INVALID ITERATOR LOCATION";

	return cur_BTnode->m_key;
}

unsigned int MultiMap::Iterator::getValue() const
{
	if (!valid())
		return NULL;
	return cur_LinkedValue->m_value;
}

bool MultiMap::Iterator::next()
{
	if (!valid())
		return false;

	if (cur_LinkedValue->next != nullptr)//If there is another value in the LinkedValue list
	{
		cur_LinkedValue = cur_LinkedValue->next;
		return true;
	}

	//If cur_BTnode is the maximum key and there is no more values in it's LinkedValue list, then set cur_BTnode to nullptr and return false

	BTnode* max_node = root_BTnode;

	while (max_node->pRight != nullptr)
		max_node = max_node->pRight;
	if (cur_BTnode == max_node)
	{
		cur_BTnode = nullptr;
		return false;
	}

	else//there isn't another value in the LinkedValue List
	{
		if (cur_BTnode->pRight != nullptr)//If the current node has a right child, set the current node to the minimum value of the right subtree
		{
			BTnode* min_right = cur_BTnode->pRight;

			if (min_right->pLeft == nullptr)//if the right child of the current node has no left children, then that right child is the minimum value
			{
				cur_BTnode = min_right;
				cur_LinkedValue = cur_BTnode->head_LinkedValue;
				return true;
			}
			//else, it does have left children,
			while (min_right->pLeft != nullptr)
				min_right = min_right->pLeft;

			cur_BTnode = min_right;
			cur_LinkedValue = cur_BTnode->head_LinkedValue;
			return true;
		}

		else //If the current node doesn't have a right child, then create a parent node and while the current node is the right child of the tree, keep shifting up
			//If the current node is not the right child of it's parent, then just set current to parrent node and return
		{
			BTnode* c_parent = cur_BTnode->pParent;

			while (c_parent != nullptr && cur_BTnode == c_parent->pRight)
			{
				cur_BTnode = c_parent;
				c_parent = cur_BTnode->pParent;
			}

			cur_BTnode = c_parent;
			cur_LinkedValue = cur_BTnode->head_LinkedValue;
			return true;
		}
	}
}

bool MultiMap::Iterator::prev()
{
	if (!valid())
		return false;

	if (cur_LinkedValue->prev != nullptr)//If there is another prev value in the LinkedValue list
	{
		cur_LinkedValue = cur_LinkedValue->prev;
		return true;
	}

	BTnode* min_node = root_BTnode;

	while (min_node->pLeft != nullptr)
		min_node = min_node->pLeft;

	if (cur_BTnode == min_node)
	{
		cur_BTnode = nullptr;
		return false;
	}

	else//there isn't another value in the LinkedValue List
	{
		if (cur_BTnode->pLeft != nullptr)//If the current node has a left child, set the current node to the max value of the left subtree
		{
			BTnode* max_left = cur_BTnode->pLeft;

			if (max_left->pRight == nullptr)//if the left child of the current node has no right children, then that left child is the max value
			{
				cur_BTnode = max_left;
				cur_LinkedValue = cur_BTnode->head_LinkedValue;

				while (cur_LinkedValue->next != nullptr)
					cur_LinkedValue = cur_LinkedValue->next;

				return true;
			}
			//else, it does have left children,
			while (max_left->pRight != nullptr)
				max_left = max_left->pRight;

			cur_BTnode = max_left;
			cur_LinkedValue = cur_BTnode->head_LinkedValue;

			while (cur_LinkedValue->next != nullptr)
				cur_LinkedValue = cur_LinkedValue->next;

			return true;
		}

		else //If the current node doesn't have a right child, then create a parent node and while the current node is the right child of the tree, keep shifting up
			//If the current node is not the right child of it's parent, then just set parent to current node and return
		{
			BTnode* c_parent = cur_BTnode->pParent;

			while (c_parent != nullptr && cur_BTnode == c_parent->pLeft)
			{
				cur_BTnode = c_parent;
				c_parent = cur_BTnode->pParent;
			}

			cur_BTnode = c_parent;
			cur_LinkedValue = cur_BTnode->head_LinkedValue;

			while (cur_LinkedValue->next != nullptr)
				cur_LinkedValue = cur_LinkedValue->next;

			return true;
		}
	}

}

MultiMap::MultiMap()
{
	m_root = nullptr;
	m_size = 0;
}

MultiMap::~MultiMap()
{
	clear();
}

void MultiMap::clear()
{
	FreeTree(m_root);
	m_root = nullptr;
}

void MultiMap::insert(string key, unsigned int value)
{
	if (m_root == nullptr)//If the tree starts out empty, create a new BTnode and have the m_root pointer point to it
	{
		m_root = new BTnode(key, value);
		m_size++;
		return;
	}

	BTnode* cur = m_root;

	for (;;)
	{
		if (key == cur->m_key)//If the current BTnode has the same key, merely add another LinkedValue to it (DO NOT CREATE ANOTHER BTnode
		{
			LinkedValue* cur_val = cur->head_LinkedValue;

			while (cur_val != nullptr)//while the current value is not a nullptr
			{
				if (cur_val->next == nullptr)//if the next value is a nullptr
				{
					cur_val->next = new LinkedValue(value);
					cur_val->next->prev = cur_val;
					m_size++;
					return;
				}
				else
					cur_val = cur_val->next;
			}
		}

		else if (key < cur->m_key)//If the inputted key Value is less than the current BTnode value, create a new BTnode, 
			//set the left child pointer of the current node to it, and set the new BTnode's parent to it
		{
			if (cur->pLeft != nullptr)//there is already a left child, then set cur to it's left child and move on
				cur = cur->pLeft;
			else//there isn't a left child
			{
				cur->pLeft = new BTnode(key, value);//set the left child of the current node to the newly created one
				cur->pLeft->pParent = cur;//set the current node's parent pointer to the current node
				m_size++;
				return;
			}
		}

		else if (key > cur->m_key)//If the inputted key Value is greater than the current BTnode value, create a new BTnode,
			//set the right child pointer of hte current node to it, and set the new BTnode's parent to it
		{
			if (cur->pRight != nullptr)//there is already a right child, then set cur to it's right child and move on
				cur = cur->pRight;
			else
			{
				cur->pRight = new BTnode(key, value);
				cur->pRight->pParent = cur;
				m_size++;
				return;
			}

		}
	}

}

MultiMap::Iterator MultiMap::findEqual(std::string key) const
{
	Iterator it1(m_root, m_root);

	Iterator it2;//invalid iterator

	if (m_root->m_key == key)
		return it1;

	BTnode* current = m_root;

	while (current->m_key != key && current != nullptr)//Runs in O(log N) time as the number of nodes to search are cut in half
	{
		if (key < current->m_key && current->pLeft != nullptr)
			current = current->pLeft;
		else if (key > current->m_key && current->pRight != nullptr)
			current = current->pRight;
		else if (key == current->m_key)
		{
			Iterator it3(m_root, current);
			return it3;
		}
		else
			break;
	}

	return it2;
}

MultiMap::Iterator MultiMap::findEqualOrSuccessor(std::string key) const
{
	Iterator invalid;

	if (findEqual(key).valid())
		return findEqual(key);
	else
	{
		// if the given key is greater than the max value in the multimap, return an invalid iterator
		BTnode* max_node = m_root;
		while (max_node->pRight != nullptr)
			max_node = max_node->pRight;
		if (key > max_node->m_key)
		{
			return invalid;
		}

		// if the given key is less than the smallest value in the multimap, return the min node
		BTnode* min_node = m_root;
		while (min_node->pLeft != nullptr)
			min_node = min_node->pLeft;
		if (key < min_node->m_key)
		{
			Iterator it1(m_root, min_node);
			return it1;
		}


		else
		{
			BTnode* t_node = m_root;

			if (key < m_root->m_key)
			{
				while (t_node->pLeft->m_key > key && t_node->pLeft != nullptr) //while the left child is greater than current key, move left
					t_node = t_node->pLeft;//breaks when left child is nullptr or smaller than key

				Iterator it1(m_root, t_node->pLeft);
				while (it1.getKey() < key && it1.next())
				{	/*Do Nothing*/
				}
				return it1;
			}

			else
			{
				while (t_node->pRight->m_key < key && t_node->pRight != nullptr)
					t_node = t_node->pRight;

				Iterator it1(m_root, t_node->pRight);
				while (it1.getKey() > key && it1.prev())
				{	/*Do Nothing*/
				}
				it1.next();
				return it1;
			}
		}
	}
}


MultiMap::Iterator MultiMap::findEqualOrPredecessor(std::string	key) const
{
	Iterator invalid;

	if (findEqual(key).valid())
		return findEqual(key);
	else
	{
		// if the given key is less than the min value in the multimap, return an invalid iterator
		BTnode* min_node = m_root;
		while (min_node->pLeft != nullptr)
			min_node = min_node->pLeft;
		if (key < min_node->m_key)
		{
			return invalid;
		}

		// if the given key is greater than the greatest value in the multimap, return the min node
		BTnode* max_node = m_root;
		while (max_node->pRight != nullptr)
			max_node = max_node->pRight;
		if (key > max_node->m_key)
		{
			Iterator it1(m_root, max_node);
			return it1;
		}

		else
		{
			BTnode* t_node = m_root;

			if (key > m_root->m_key)
			{
				while (t_node->pRight->m_key < key && t_node->pRight != nullptr) //while the right child is less than current key, move right
					t_node = t_node->pRight;//breaks when right child is nullptr or greater than key

				Iterator it1(m_root, t_node->pRight);
				while (it1.getKey() > key && it1.prev())
				{	/*Do Nothin*/
				}
				return it1;
			}

			else
			{
				while (t_node->pLeft->m_key > key && t_node->pLeft != nullptr)
					t_node = t_node->pLeft;

				Iterator it1(m_root, t_node->pLeft);
				while (it1.getKey() < key && it1.next())
				{	/*Do Nothin*/
				}
				it1.prev();
				return it1;
			}
		}
	}
}
