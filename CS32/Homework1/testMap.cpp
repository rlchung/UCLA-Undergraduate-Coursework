#include "Map.h"
#include <cassert>

#include <iostream>

using namespace std;

int main()
{
	Map m1;  // maps strings to doubles
	Map m2;
	Map m3;

	KeyType a = "David";
	KeyType b = "Jack";
	KeyType c = "John";
	KeyType d = "Henry";

	ValueType a1 = 1.0;
	ValueType b1 = 2.0;
	ValueType c1 = 3.0;
	ValueType d1 = 4.0;

	//***********************************************
	//TESTING FUNCTIONS: empty(), size(), insert(...)
	//***********************************************

	assert(m1.empty());
	assert(m1.size() == 0);//Check if size == 0
	assert(m1.insert(a, a1));
	assert(m1.size() == 1); //Check if size == 1
	assert(m1.insert(b, b1));
	assert(m1.size() == 2); //Check if size == 2

	assert(m2.empty());
	assert(m2.size() == 0);
	assert(m2.insert(c, c1));
	assert(m2.size() == 1);
	assert(m2.insert(d, d1));
	assert(m2.size() == 2); //Check if size == 2

	//***************************************************
	//TESTING FUNCTIONS: update(...), insertOrUpdate(...)
	//***************************************************

	assert(!m1.update(d, 10.0));//KeyType "d" should not be in m1
	assert(!m2.update(a, 10.0));//KeyType "a" should not be in m2
	assert(m1.update(a, 5.0));
	assert(m1.update(b, 6.0));
	assert(m2.update(c, 7.0));
	assert(m2.update(d, 8.0));

	//******************************************************
	//TESTING FUNCTIONS: erase(...), contains(...), size();
	//******************************************************

	assert(m1.contains(a));
	assert(m1.contains(b));
	assert(m2.contains(c));
	assert(m2.contains(d));
	
	assert(!m1.contains(d));
	assert(!m1.contains(c));
	assert(!m2.contains(b));
	assert(!m2.contains(a));

	assert(!m1.erase(d));
	assert(!m1.erase(c));
	assert(!m2.erase(b));
	assert(!m2.erase(a));

	assert(m1.erase(b));
	assert(m1.size() == 1); //After erasing b, there should only be one element in the map
	assert(m2.erase(c));
	assert(m2.size() == 1); //After erasing c, there should only be one element in the map

	assert(!m1.contains(b)); //After erasing b, element b should not be in m1
	assert(!m2.contains(c)); //After erasing c, element c should not be in m2

	assert(m1.contains(a));
	assert(m2.contains(d));

	//**********************************************************************************************************
	//TESTING FUNCTIONS: get(const std::string& key, double& value), get(int i, std::string& key, double& value)
	//**********************************************************************************************************
	
	ValueType v = -1234.5;
	assert(!m1.get("abc", v) && v == -1234.5); // v unchanged by get failure

	ValueType v1 = 1234.5;
	assert(m1.get(a, v1) && v1 == 5.0); // v1 should be changed to 5.0 (value of a) by success of get function

	KeyType k = "hello";
	
	assert(m1.get(0, k, v) && k == a && v == 5.0);

	m1.swap(m2);
	
	assert(!m1.contains(a));
	assert(!m2.contains(d));

	cout << "Passed all tests" << endl;
}