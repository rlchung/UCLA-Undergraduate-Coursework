#include "SalaryMap.h"
#include <iostream>

SalaryMap::SalaryMap()
{
	//create an empty salary map
};

// If an employee with the given name is not currently in the map, 
// there is room in the map, and the salary is not negative, add an
// entry for that employee and salary and return true.  Otherwise
// make no change to the map and return false.
bool SalaryMap::add(std::string name, double salary)
{
	if (!s_map.contains(name) && s_map.size() < DEFAULT_MAX_ITEMS && salary >= 0)
	{
		s_map.insert(name, salary);
		return true;
	}
	else
		return false;
};

// If no employee with the given name is in the map or if pct is less
// than -100, make no change to the map and return false.  Otherwise,
// change the salary of the indicated employee by the given
// percentage and return true.  For example, if pct is 10, the
// employee gets a 10% raise; if it's -5, the salary is reduced by 5%.
bool SalaryMap::raise(std::string name, double pct)
{
	if (!s_map.contains(name) || pct < -100)
		return false;
	else
	{
		double old_salary;
		double new_salary;
		double pct_raise = pct * (.01);

		if (s_map.get(name, old_salary))
		{
			new_salary = old_salary + (old_salary * pct_raise);
			s_map.update(name, new_salary);
		}

		return true;
	}
};

// If an employee with the given name is in the map, return that
// employee's salary; otherwise, return -1.
double SalaryMap::salary(std::string name) const
{
	double retrieved_sal;

	if (s_map.get(name, retrieved_sal))
		return retrieved_sal;
	else
		return -1;
};

// Return the number of employees in the SalaryMap.
int SalaryMap::size() const
{
	return s_map.size();
};

// Write to cout one line for every employee in the map.  Each line
// has the employee's name, followed by one space, followed by that
// employee's salary.
void SalaryMap::print() const
{
	std::string e_name;
	double retrieved_sal = 0;

	for (int x = 0; x < s_map.size(); x++)
	{
		if (s_map.get(x, e_name, retrieved_sal))
		{
			cout << e_name << " " << retrieved_sal << endl;
		}
	}
};