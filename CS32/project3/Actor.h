#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include "GraphObject.h"
#include "Level.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//Actor will serve as the base class from which everything else will derive from
//Base for: Player, Bricks(Permanent and Destroyable)
//Contains: own (x,y) location in maze, what to do during each tick, state of being alive or dead, attackable 
class Actor : public GraphObject
{
public:
	Actor(StudentWorld * world, int imageID, int startX, int startY);

	virtual void doSomething() = 0;

	void set_attackable(bool i_attackable) {
		m_attackable = i_attackable;
	}

	//Retrieves if the actor is attackable or not
	bool get_attackable() const {
		return m_attackable;
	}
	
	//Returns the world object of the given actor
	StudentWorld* getWorld() const;

	//Sets the Actor to dead (Later to be used to remove them from the board, end the game, advance the level, etc.
	virtual void setDead() {
		setVisible(false);
		m_alive = false;
	}

	//Returns bool value if the actor is dead or alive
	bool isAlive() const { 
		return m_alive; 
	}

	/////////////FOR THE PLAYER OBJECT/////////////////////////////
	virtual void setTimeWalkThrough(int i_time) {}

	virtual int getTimeWalkThrough() const { return 0; }

	virtual void setTimeMaxMode(int i_time) {}

	virtual int getTimeMaxMode() const { return 0; }

private:
	StudentWorld* myWorld;
	bool m_alive;
	bool m_attackable;
};

class Player : public Actor
{
public:
	Player(StudentWorld* world, int startX, int startY);

	virtual void doSomething();

	void setTimeWalkThrough(int i_time) {
		time_walk_through = i_time;
	}

	int getTimeWalkThrough() const{
		return time_walk_through;
	}

	void setTimeMaxMode(int i_time) {
		time_maxMode = i_time;
	}

	int getTimeMaxMode() const {
		return time_maxMode;
	}

	//void increaseSimultaneousSprayers(unsigned int max, unsigned int lifetime);

private:
	int time_walk_through;
	int time_maxMode;
	
};

class Brick : public Actor
{
public:
	Brick(StudentWorld* world, int startX, int startY, int ImageID);
	
	virtual void doSomething() {};//does nothing because it's a brick for God's sake...
private:
};

class TimedLifetimeActor : public Actor
{
public:
	TimedLifetimeActor(StudentWorld* world, int startX, int startY, int imageID, unsigned int lifetime);

	virtual void doSomething() = 0;

	int getLifetime() const {
		return m_lifetime;
	}

	void decrementLifetime() {
		if (m_lifetime > 0)
			m_lifetime--;
	}

	virtual void expireImmediately(); //Set remaining lifetime to 0

private:
	unsigned int m_lifetime;//How many ticks it gets before it disappears
};

class BugSprayer : public TimedLifetimeActor
{
public:
	BugSprayer(StudentWorld* world, int startX, int startY, int ImageID);

	void releaseSpray();

	virtual void setDead();

	virtual void doSomething();

private:
};

class BugSpray : public TimedLifetimeActor
{
public:
	BugSpray(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
};


class Exit : public Actor
{
public:
	Exit(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
private:
};

class Goodie : public TimedLifetimeActor
{
public:
	Goodie(StudentWorld* world, int imageID, int startX, int startY);
	virtual void doSomething() = 0; //DOES NOTHING
private:
};

class ExtraLifeGoodie : public Goodie
{
public:
	ExtraLifeGoodie(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
private:
};

class WalkThroughWallsGoodie : public Goodie
{
public:
	WalkThroughWallsGoodie(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
private:
};

class IncreaseSimultaneousSprayersGoodie : public Goodie
{
public:
	IncreaseSimultaneousSprayersGoodie(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
private:
};

class Zumi : public Actor
{
public:
	Zumi(StudentWorld* world, int imageID, int maxActionTick, int startX, int startY);
	
	int getCurrentActionTicks() const {
		return m_currentActionTicks;
	}

	void setCurrentDirection(int i_currentDirection) {
		currentDirection = i_currentDirection;
	}

	int getDirection() const {
		return currentDirection;
	}

	void incrementActionTicks() {
		m_currentActionTicks++;
	}
	
	void resetActionTicks() {
		m_currentActionTicks = 0;
	}

	int getMaxActionTick() const {
		return m_MaxActionTick;
	}

	virtual void doSomething() = 0;
private:
	int currentDirection;//to be set randomly via random number generator 1 - 4 inclusive (1 == LEFT // 2 == RIGHT // 3 == UP // 4 == DOWN)
	int m_MaxActionTick;//to be the comparision value with which
	int m_currentActionTicks;//to be set to 0 at initialization and incremented gradually until it reaches actionTicks, perform something in doSomething, and be reset back to 0
};

class SimpleZumi : public Zumi
{
public:
	SimpleZumi(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
private:
};

class ComplexZumi : public Zumi
{
public:
	ComplexZumi(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
private:
};

#endif