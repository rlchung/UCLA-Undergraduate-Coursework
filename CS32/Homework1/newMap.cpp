#include "newMap.h"

using namespace std;

Map::Map()
{
	d_array = new pair[DEFAULT_MAX_ITEMS];//DEFAULT CONSTRUCTOR WILL INITIALIZE d_array to be 200 itmes
	d_size = DEFAULT_MAX_ITEMS;
	m_size = 0;
};

Map::Map(int input_size)
{
	d_array = new pair[input_size];
	d_size = input_size;
	m_size = 0;

};

Map::Map(const Map &old)
{
	d_size = old.d_size; //d_array is now the size of the old d_array
	m_size = old.m_size; //set m_size to old m_size

	d_array = new pair[d_size]; //d_array is now an array of size d_size

	for (int x = 0; x < d_size; x++)//Copy over each element in the old d_array into the new d_array
		d_array[x] = old.d_array[x];
};

Map& Map::operator=(const Map&source)
{
	delete[] d_array; //Free memory held at d_array
	d_size = source.d_size; //d_array is now the size of the old d_array
	m_size = source.m_size; //set m_size to old m_size

	d_array = new pair[d_size]; //d_array is now an array of size d_size

	for (int x = 0; x < d_size; x++)//Copy over each element in the old d_array into the new d_array
		d_array[x] = source.d_array[x];
	return (*this);
};

Map::~Map()
{
	delete[] d_array;
}

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


	if (!contains(key) && size() < d_size) //(1) if the key is not equal to any key in the map (2) if there is enough space to add the pair
	{
		pair insert_pair;
		insert_pair.p_key = key;
		insert_pair.p_value = value;

		d_array[size()] = insert_pair;

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
		if (key == d_array[x].p_key)
		{
			d_array[x].p_value = value;
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
			if (key == d_array[x].p_key)
				erase_spot = x;
		}

		for (; erase_spot < size(); erase_spot++)
		{
			if (erase_spot == size() - 1)
			{
				pair empty_pair;
				d_array[erase_spot] = empty_pair;
			}
			else
				d_array[erase_spot] = d_array[erase_spot + 1];
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
		if (key == d_array[x].p_key)
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
			if (key == d_array[x].p_key)
			{
				value = d_array[x].p_value;
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
		key = d_array[i].p_key;
		value = d_array[i].p_value;
		return true;
	}

	else
		return false;
};

void Map::swap(Map& other)
{
	pair * temp_d_array;

	temp_d_array = d_array;
	d_array = other.d_array;
	other.d_array = temp_d_array;

	int temp_m_size = m_size;
	m_size = other.m_size;
	other.m_size = temp_m_size;

	int temp_d_size = d_size;
	d_size = other.d_size;
	other.d_size = temp_d_size;
};
