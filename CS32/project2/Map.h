#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <string>

typedef std::string KeyType;
typedef double ValueType;

class Map
{
public:
	Map();
	//COPY, ASSIGNMENT, CONSTRUCTORS AND DESTRUCTOR
	Map(const Map& other);
	Map& operator=(const Map& rhs);
	~Map();
	bool empty() const;
	int size() const;
	bool insert(const KeyType& key, const ValueType& value);
	bool update(const KeyType& key, const ValueType& value);
	bool insertOrUpdate(const KeyType& key, const ValueType& value);
	bool erase(const KeyType& key);
	bool contains(const KeyType& key) const;
	bool get(const KeyType& key, ValueType& value) const;
	bool get(int i, KeyType& key, ValueType& value) const;
	void swap(Map& other);

private:
	struct Node {
		KeyType m_Key;
		ValueType m_Value;

		Node* next;
		Node* previous;
	};

	Node* m_head;//Points to the first element of the List
	Node* m_tail;//Points to the second element of the List

	int m_size;//Refers to the total amount of objects/elements within the Doubly-LinkedList

	//*****************************************
	//PRIVATE MEMBER FUNCTIONS*****************
	//*****************************************

	bool add_front(const KeyType& key, const ValueType& value);
	bool add_end(const KeyType& key, const ValueType& value);
	int find_position(const KeyType& key) const;// Return -1 if not found, return location of position if found (1 = 1st element, 2 = 2nd element, etc.)
	//void print_info() const;
};

//ADDITIONAL MAP ALGORITHMS FOR DOUBLY LINKEDLISTS
bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif