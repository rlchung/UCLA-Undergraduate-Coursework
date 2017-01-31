#ifndef MULTIMAP_INCLUDED
#define MULTIMAP_INCLUDED

#include <string>

class	MultiMap
{
private :
	struct LinkedValue {
	LinkedValue(unsigned int i_value)
	{
		m_value = i_value;
		next = nullptr;
		prev = nullptr;
	}

	unsigned int m_value;
	LinkedValue* next;
	LinkedValue* prev;
	};

private:
	struct BTnode {
	BTnode(std::string i_key, unsigned int i_value)
	{
		m_key = i_key;

		head_LinkedValue = new LinkedValue(i_value);

		pParent = nullptr;
		pLeft = nullptr;
		pRight = nullptr;
	}

	std::string m_key;

	BTnode* pParent;
	BTnode* pLeft;
	BTnode* pRight;

	//LinekdList of Values
	LinkedValue* head_LinkedValue;//points to the head of the LinkedList
};

public:
	//	You	must	implement	this	public	nested	MultiMap::Iterator	class
	class	Iterator
	{
	public:
		Iterator();		//	You	must	have	a	default	constructor
		Iterator(BTnode* i_root, BTnode* i_node);//User-defined parameters that sets the root and also the given node
		bool	valid()	const;
		std::string	getKey()	const;
		unsigned	int	getValue()	const;
		bool	next();
		bool	prev();
	private:
		BTnode* root_BTnode;
		BTnode* cur_BTnode;
		LinkedValue* cur_LinkedValue;
	};

	MultiMap();
	~MultiMap();
	void	clear();
	void	insert(std::string	key, unsigned	int	value);
	Iterator	findEqual(std::string	key)	const;
	Iterator	findEqualOrSuccessor(std::string	key)	const;
	Iterator	findEqualOrPredecessor(std::string	key)	const;

private:
	//	To	prevent	Multimaps	from	being	copied	or	assigned,	declare	these	members
	//	private	and	do	not	implement	them.
	MultiMap(const	MultiMap&	other);
	MultiMap&	operator=(const	MultiMap&	rhs);

	void FreeTree(BTnode*cur)//For use of clear() and ~MultiMap
	{
		if (cur == NULL)
			return;
		FreeTree(cur->pLeft);
		FreeTree(cur->pRight);

		FreeList(cur);
		delete cur;
	}

	void FreeList(BTnode*cur)
	{
		LinkedValue* cur_LinkedValue = cur->head_LinkedValue;
		while (cur_LinkedValue->next != nullptr)
		{
			cur_LinkedValue = cur_LinkedValue->next;
		}//after loop, cur_LinkedValue will point to the last LinkedValue

		LinkedValue* previous_cur = cur_LinkedValue->prev;

		while (previous_cur != nullptr)
		{
			delete cur_LinkedValue;
			cur_LinkedValue = previous_cur;
			previous_cur = cur_LinkedValue->prev;
		}

		delete cur_LinkedValue;
	}

	BTnode* m_root;
	int m_size;
};

#endif