#include "Map.h"

Map::Map()
{
	m_size = 0;
};

bool Map::empty() const
{
	if (size() == 0)
		return true;
	else
		return false;
};

int Map::size() const
{
	return m_size;
};

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if (!contains(key) && size() < DEFAULT_MAX_ITEMS) //(1) if the key is not equal to any key in the map (2) if there is enough space to add the pair
	{
		pair insert_pair;
		insert_pair.p_key = key;
		insert_pair.p_value = value;

		m_array[size()] = insert_pair;

		m_size++;
		
		return true;
	}

	else
		return false;
};

bool Map::update(const KeyType& key, const ValueType& value)
{
	for (int x = 0; x < size(); x++)
	{
		if (key == m_array[x].p_key)
		{
			m_array[x].p_value = value;
			//cout << m_array[x].p_value;
			return true;
		}
	}

	return false;
};

bool Map::insertOrUpdate(const KeyType&key, const ValueType& value)
{
	if (contains(key))
		return update(key, value);
	else
		return insert(key, value);
};

bool Map::erase(const KeyType& key)
{
	if (contains(key))
	{
		int erase_spot;

		for (int x = 0; x < size(); x++)
		{
			if (key == m_array[x].p_key)
				erase_spot = x;
		}

		for (; erase_spot < size(); erase_spot++)
		{
			if (erase_spot == size() - 1)
			{
				pair empty_pair;
				m_array[erase_spot] = empty_pair;
			}
			else
				m_array[erase_spot] = m_array[erase_spot + 1];
		}
		
		m_size--;
		return true;
	}

	else
		return false;
};

bool Map::contains(const KeyType& key) const
{
	for (int x = 0; x < size(); x++)
	{
		if (key == m_array[x].p_key)
			return true;
	}

	return false;
};

bool Map::get(const KeyType& key, ValueType& value) const
{
	if (contains(key))
	{
		for (int x = 0; x < size(); x++)
		{
			if (key == m_array[x].p_key)
			{
				value = m_array[x].p_value;
			}
		}
		return true;
	}

	else
		return false;
};


bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (0 <= i && i < size())
	{
		key = m_array[i].p_key;
		value = m_array[i].p_value;
		return true;
	}

	else
		return false;
};

void Map::swap(Map& other)
{
	pair temp_array[DEFAULT_MAX_ITEMS];
	int temp_size;

	for (int x = 0; x < other.size(); x++)
	{
		temp_array[x] = other.m_array[x];
	}

	for (int y = 0; y < DEFAULT_MAX_ITEMS; y++)
	{
		other.m_array[y] = m_array[y];
	}

	for (int z = 0; z < DEFAULT_MAX_ITEMS; z++)
	{
		m_array[z] = temp_array[z];
	}

	temp_size = other.m_size;
	other.m_size = m_size;
	m_size = temp_size;
};
