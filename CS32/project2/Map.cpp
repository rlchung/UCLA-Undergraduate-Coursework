#include "Map.h"
//#include <iostream>

Map::Map() : m_size(0)//Set m_size to 0 and set both m_head and m_tail to nullptr
{
	m_head = nullptr;
	m_tail = nullptr;
}

Map::Map(const Map& other)
{
	if (!other.empty())//If other is not empty, proceed
	{
		KeyType add_Key;
		ValueType add_Type;

		Node* traverse = other.m_head;

		while (traverse->next != nullptr)//iterate through the list until traverse is the last element
		{
			add_Key = traverse->m_Key;
			add_Type = traverse->m_Value;
			insert(add_Key, add_Type);//insert retrieved node from other into given map
			traverse = traverse->next;//Move to the next node
		}

		//For adding the last node into the given the map
		add_Key = traverse->m_Key;
		add_Type = traverse->m_Value;
		insert(add_Key, add_Type);
	}

	else//If other is empty, set to empty map
	{
		m_head = nullptr;
		m_tail = nullptr;
	}
}

Map& Map::operator=(const Map& rhs)
{
	//CHECK FOR ALIASING
	if (&rhs == this)
		return *this;
	
	//USE DESTRUCTOR CODE ON LHS MAP TO CLEAR IT ALL
	Node* t_node = m_head;
	while (t_node != nullptr)
	{
		Node* next_node;
		next_node = t_node->next;
		delete t_node;
		t_node = next_node;
	}

	//Reset default private data members to empty
	m_head = nullptr;
	m_tail = nullptr;
	m_size = 0;


	//CALL THE COPY CONSTRUCTOR CODE TO COPY RHS ONTO LHS; restrictions: rhs may be either empty or have elemetns
	
	if (!rhs.empty())//if rhs is not empty
	{
		KeyType add_Key;
		ValueType add_Type;

		Node* traverse = rhs.m_head;

		while (traverse != nullptr)//iterate through the list until traverse is the last element
		{
			add_Key = traverse->m_Key; 
			add_Type = traverse->m_Value;

			insert(add_Key, add_Type);

			traverse = traverse->next;
		}

		return *this;
	}
	else //if rhs is empty
		return *this;
}


Map::~Map()
{
	Node* t_node = m_head;
	while (t_node != nullptr)//Delete current node and set t_node to next_node
	{
		Node* next_node;
		next_node = t_node->next;
		delete t_node;
		t_node = next_node;
	}
}

bool Map::empty() const
{
	if (m_head == nullptr)
		return true;
	else
		return false;
}

int Map::size() const
{
	return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if (contains(key))//If key is in map, return false and do nothing to the map
		return false;

	else if (empty())//If empty, add element to the front and set head pointer and tail pointer to it
	{
		add_front(key, value);

		m_tail = m_head;

		return true;
	}

	else//If there are 1+ elements, add behind it
	{
		add_end(key, value);
		return true;
	}
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	if (!contains(key))
		return false;
	else
	{
		int count = find_position(key);//retrieves the position of the key that will be used to iterate the Doubly-LinkedList
		Node* traverse = m_head;//traverse pointer points to m_head address

		for (int x = 0; x < count; x++)
		{
			if (traverse->m_Key != key)
				traverse = traverse->next;
			else
				traverse->m_Value = value;
		}
		return true;
	}
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	if (contains(key))
		return update(key, value);
	else
		return insert(key, value);
}

bool Map::erase(const KeyType& key)
{
	if (!contains(key))
		return false;
	else
	{
		if (size() == 1)//If the map contains only 1 element
		{
			Node* t_node = m_head;//create a temporary node that points to the first_node
			delete t_node;//remove the node from the List
			m_head = nullptr;//set m_head to nullptr
			m_tail = nullptr;//set m_tail to nullptr
		}

		else if (size() > 1 && find_position(key) == 1)//If there is more than 1 element and key is the first node
		{
			Node* next_node = m_head->next;//create a temporary node that points to the second_node
			next_node->previous = nullptr;//Change the next_node previous pointer to nullptr
			delete m_head;//remove the node from the List
			m_head = next_node;//m_head pointer now points to the next_node
		}

		else if (size() > 1 && find_position(key) == size())//If there is more than 1 element and key is the last node
		{
			Node* previous_node = m_tail->previous;//create a temporary node that points to the node before the one we're going to delete
			previous_node->next = nullptr;//Change the previous_node next pointer to nullptr
			delete m_tail;//remove the node from the list
			m_tail = previous_node;//m_tail pointer now points to the previous_node
		}

		else//If the element is between 2+ nodes
		{
			int count = find_position(key);//retrieves the position of the key that will be used to iterate the Doubly-LinkedList
			Node* traverse = m_head;//traverse pointer points to m_head address

			for (int x = 0; x < count; x++)
			{
				if (traverse->m_Key != key)
					traverse = traverse->next;
				else
				{}
			}

			Node* traverse_previous = traverse->previous;//points to the node before the one we're going to delete
			Node* traverse_next = traverse->next;//points to the node after the one we're going to delete
			delete traverse;
			traverse_previous->next = traverse_next;//node previous to the one we just deleted now changed its next pointer to point to the one after the one we deleted
			traverse_next->previous = traverse_previous;//node next after the one we just deleted now changed its previous pointer to point to the one before the one we deleted
		}

		m_size--;
		return true;
	}
}

bool Map::contains(const KeyType& key) const
{
	if (find_position(key) == -1)
		return false;
	else
		return true;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	if (!contains(key))//if map doesn't contain the key
		return false;
	else
	{
		int count = find_position(key);//retrieves the position of the key that will be used to iterate the Doubly-LinkedList
		Node* traverse = m_head;//traverse pointer points to m_head address

		for (int x = 0; x < count; x++)
		{
			if (traverse->m_Key != key)
				traverse = traverse->next;
			else
				value = traverse->m_Value;
		}
		return true;
	}
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i > size()-1)//if map doesn't contain the key or if i is trying to access an unaccessible node location
		return false;
	else
	{
		int count = i;//retrieves the position of the key that will be used to iterate the Doubly-LinkedList
		Node* traverse = m_head;//traverse pointer points to m_head address

		for (int x = 0; x <= count; x++)
		{
			value = traverse->m_Value;
			key = traverse->m_Key;
			traverse = traverse->next;
		}
		return true;
	}
}

void Map::swap(Map& other)//swap m_head and m_tail pointers and m_size
{
	if (&other == this)//CHECK FOR ALIASING
		return;

	Node* t_node;

	t_node = m_head;//set temporary node to point to m_head's current address
	m_head = other.m_head;//set m_head to point to other.m_head's current address
	other.m_head = t_node;//set other.m_head to point to t_node's current address, which is m_head's previous address

	t_node = m_tail;
	m_tail = other.m_tail;
	other.m_tail = t_node;

	int t_size;

	t_size = m_size;
	m_size = other.m_size;
	other.m_size = t_size;
}

int Map::find_position(const KeyType& key) const//Returns the position of key; 1 = 1st element, 2 = 2nd element...
{
	int position = 0;
	Node* temp;

	temp = m_head;//Points to the first node

	if (empty())//If the Map is empty, return -1 as there are no elements to search
		return -1;

	else if (size() == 1)//If the Map has 1 element, return position if head has key, else return -1
	{
		if (m_head->m_Key == key)
			return 1;
		else
			return -1;
	}

	else//Iterate through the Doubly-LinkedList until temp->next reaches nullpointer/temp
	{
		position++;//Increment as temp already points to the first element in a LinkedList with m_size > 1

		while (temp->next != nullptr)//Will exit once temp is set to the last pointer
		{
			if (temp->m_Key == key)
				return position;
			else
			{
				position++;
				temp = temp->next;
			}
		}

		if (temp->m_Key == key)
			return position;
		else
			return -1;//NOT FOUND AFTER TRAVERSING ALL NODES
	}
}

bool Map::add_front(const KeyType& key, const ValueType& value)
{
	if (contains(key))
		return false;//MAP ALREADY HAS THE KEY
	else
	{
		Node* front_node = new Node;
		front_node->m_Key = key;
		front_node->m_Value = value;

		front_node->previous = nullptr; //set front_node previous pointer to nullptr as it is now the first element in the List
		front_node->next = m_head; //set front_node next pointer to current m_head address
		
		if (size() > 0)//only execute when there is at least one element in the List
			m_head->previous = front_node;//set current m_head previous pointer to front_node

		m_head = front_node;//Link the m_head pointer to front_node address
		m_size++;

		return true;
	}
}

bool Map::add_end(const KeyType& key, const ValueType& value)
{
	if (contains(key))
		return false;
	else
	{
		Node* latest_node = new Node;
		latest_node->m_Key = key;
		latest_node->m_Value = value;

		latest_node->next = nullptr;//set latest_node next pointer to nullptr as it is the last element in the list
		latest_node->previous = m_tail;//set latest_node previous pointer to the current tail pointer
		m_tail->next = latest_node;//set current tail node's next pointer to latest_node

		m_tail = latest_node;//Link the m_tail pointer to latest_node address
		m_size++;

		return true;
	}
}

/**
void Map::print_info() const
{
	int count = 1;
	Node* temp = m_head;

	while (temp->next != nullptr)//Will exit once temp is set to the last pointer
	{
		if (temp != nullptr)
		{
			std::cout << "Node: " << count << "\n" << "Key: " << temp->m_Key << "\n" << "Value: " << temp->m_Value << "\n" << "Key of Next Node: " << temp->next->m_Key << "\n";
			if (temp->previous == nullptr)
				std::cout << "Key of Previous Node: " << "NULL POINTER\n\n";
			else
				std::cout << "Key of Previous Node: " << temp->previous->m_Key << "\n\n";
		}
		temp = temp->next;
		count++;
	}

	if (temp != nullptr)
	{
		std::cout << "Node: " << count << "\n" << "Key: " << temp->m_Key << "\n" << "Value: " << temp->m_Value << "\n";
		
		if (temp->previous == nullptr)
			std::cout << "Key of Previous Node: " << "NULL POINTER\n";
		else
			std::cout << "Key of Previous Node: " << temp->previous->m_Key << "\n";
		
		if (temp->next == nullptr)
			std::cout << "Key of Next Node: " << "NULL POINTER\n\n";
		else
			std::cout << "Key of Next Node: " << temp->next->m_Key << "\n\n";
	}

}
*/

bool combine(const Map &m1, const Map &m2, Map &result)
{
	if (&result == &m1 || &result == &m2 || &m1 == &m2)//FOR ALIASING
		return false;//Do nothing to the map if aliasing occurs

	Map temp_map;
	KeyType add_type;
	ValueType add_value;
	ValueType check_value1;
	ValueType check_value2;
	bool return_bool = true;

	for (int x = 0; x < m1.size(); x++)
	{
		m1.get(x, add_type, add_value);
		temp_map.insert(add_type, add_value);
	}
	
	for (int y = 0; y < m2.size(); y++)
	{
		m2.get(y, add_type, add_value);
		
		temp_map.insert(add_type, add_value);//temp_map will consist of the key/value pair regardless of differing values

		if (m1.get(add_type, check_value1))//if temp_map has an element of add_type, see if its value is different from the add_type (if any) in m2
		{
			if (m2.get(add_type, check_value2))//retrieve the value of add_type in m2 and compare to see if its value differs from temp_map
			{
				if (check_value1 != check_value2)
				{
					temp_map.erase(add_type);
					return_bool = false;
				}
			}
		}			
	}

	result = temp_map;
	return return_bool;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
	if (&result == &m1 || &result == &m2 || &m1 == &m2)//FOR ALIASING
		return; //do nothing to the map if aliasing occurs
	
	Map temp_map;
	KeyType add_type;
	ValueType add_value;

	for (int x = 0; x < m1.size(); x++)
	{
		m1.get(x, add_type, add_value);

		if (!m2.contains(add_type))
			temp_map.insert(add_type, add_value);
	}

	result = temp_map;

	return;
}