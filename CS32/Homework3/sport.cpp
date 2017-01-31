class Sport
{
public:
	Sport(string name) : m_name(name) { m_name = name; };
	virtual ~Sport(){};

	string name() const {
		return m_name;
	};

	virtual string icon() const = 0;
	virtual bool isOutdoor() const { 
		return true; };
private:
	string m_name;
};

class FigureSkating : public Sport
{
public:
	FigureSkating(string name) : Sport(name){};
	virtual ~FigureSkating(){ cout << "Destroying the FigureSkating object named " << name() << "." << endl; };

	virtual string icon() const {
		return "a skater in the Biellmann position";
	};
	virtual bool isOutdoor() const { 
		return false; };
private:
};

class Snowboarding : public Sport
{
public:
	Snowboarding(string name) : Sport(name){};
	virtual ~Snowboarding(){ cout << "Destroying the Snowboarding object named " << name() << "." << endl; };

	virtual string icon() const {
		return "a descending snowboarder";
	};
private:
};

class Biathlon : public Sport
{
public:
	Biathlon(string name, double dist) : Sport(name) { m_dist = dist; };
	virtual ~Biathlon(){ cout << "Destroying the Biathlon object named " << name() << ", distance " << m_dist << " km" << "." << endl; };

	virtual string icon() const {
		return "a skier with a rifle";
	};
private:
	double m_dist;
};