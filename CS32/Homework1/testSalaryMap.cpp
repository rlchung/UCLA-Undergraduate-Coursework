#include "SalaryMap.h"
#include <cassert>

#include <iostream>

using namespace std;

int main()
{
	SalaryMap m1;
	SalaryMap m2;

	//*******************************************
	//TESTING FUNCTIONS: add(...), raise(...)
	//*******************************************

	assert(m1.add("Armin", 10.00));
	assert(m1.add("Eren", 100.00));
	assert(m1.add("Mikasa", 1000.00));
	assert(m1.add("Levi", 10000.00));

	assert(!m1.add("Armin", 50.00));
	assert(!m1.add("Eren", 500.00));
	assert(!m1.add("Mikasa", 5000.00));
	assert(!m1.add("Levi", 50000.00));

	assert(m1.raise("Armin", -50.00));
	assert(m1.raise("Eren", 25.00));
	assert(m1.raise("Mikasa", 50.00));
	assert(m1.raise("Levi", 100.00));

	assert(!m1.raise("Marco", 50.00));
	assert(!m1.raise("Sasha", 500.00));
	assert(!m1.raise("Mikasa", -101.00));
	assert(!m1.raise("Levi", -500.00));

	//*******************************************
	//TESTING FUNCTIONS: salary(...), size()
	//********************************************

	assert(m1.size() == 4);

	assert(m1.salary("Armin") == 5);
	assert(m1.salary("Eren") == 125);
	assert(m1.salary("Mikasa") == 1500);
	assert(m1.salary("Levi") == 20000);

	assert(m1.salary("Marco") == -1);
	assert(m1.salary("Sasha") == -1);
	assert(m1.salary("Reiner") == -1);
	assert(m1.salary("Annie") == -1);

	//*****************************************
	//TESTING FUNCTIONS: print()
	//*****************************************

	m1.print();
	/***
	***Armin 5
	***Eren 125
	***Mikasa 1500
	***Levi 20000
	***/

	cout << "Passed all tests" << endl;
}