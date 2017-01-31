#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Class
{
public:
	Class(string nm) : m_name(nm) {}
	string name() const { return m_name; }
	const vector<Class*>& subclasses() const { return m_subclasses; }
	void add(Class* d) { m_subclasses.push_back(d); }
	~Class();
private:
	string m_name;
	vector<Class*> m_subclasses;
};

Class::~Class()
{
	for (size_t k = 0; k < m_subclasses.size(); k++)
		delete m_subclasses[k];
}

void listAll(const Class* c, string path)  // two-parameter overload
{
	if (c->subclasses().empty() && path != "")//if there is no subclass for the given class and the path is not empty, just print the name and exit
	{
		cout << path << endl;
		return;
	} 
	else if (c->subclasses().empty() && path == "")//if there is no subclass for the given class and the path is empty(single element), just print out the name and return
	{
		cout << c->name() << endl;
		return;
	}


	if (path == "")
	{
		cout << c->name() << endl;
		path = c->name() + "=>";
	}

	else
	{
		cout << path << endl;
		path = path + "=>";
	}

	for (int i = 0; i < c->subclasses().size(); i++)
	{
		listAll(c->subclasses()[i], path + c->subclasses()[i]->name());//switched order of path and c->name()
	}
}

void listAll(const Class* c)  // one-parameter overload
{
	if (c != NULL)
		listAll(c, "");
}

int main()
{
	Class* d1 = new Class("ComplexZumi");
	listAll(d1);
	cout << "====" << endl;
	d1->add(new Class("GeniusZumi"));
	Class* d2 = new Class("Zumi");
	d2->add(new Class("SimpleZumi"));
	d2->add(d1);
	Class* d3 = new Class("Goodie");
	d3->add(new Class("ExtraLifeGoodie"));
	d3->add(new Class("WalkThroughWallsGoodie"));
	d3->add(new Class("IncreaseSimultaneousSprayersGoodie"));
	listAll(d3);
	cout << "====" << endl;
	Class* d4 = new Class("Actor");
	d4->add(d2);
	d4->add(new Class("Player"));
	d4->add(d3);
	listAll(d4);
	delete d4;
}