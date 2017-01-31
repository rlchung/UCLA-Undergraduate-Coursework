#ifndef STUDENTWORLD_INCLUDED
#define STUDENTWORLD_INCLUDED

#include "GameWorld.h"
#include <vector>
#include "Level.h"

using namespace std;

class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:

	StudentWorld();

	virtual ~StudentWorld();//FREES ANY REMAINING DYNAMICALLY ALLOCATED DATA

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	void setDisplayText();//To set the display text at the top of the screen

	void removeDeadActors();

	unsigned int getDeployed() {
		return num_deployed;
	}

	void incrementDeployed() {
		num_deployed++;
	}

	void decrementDeployed() {
		num_deployed--;
	}

	Actor* getActor(int x, int y) const;

	void addActor(Actor* a);

	Actor* getPlayer() const {
		return p1;
	}

	void getPlayerLocation(int& x, int& y) const;

	void killPlayer();

	unsigned int getBonus() const {
		return num_bonus;
	}

	void reduceLevelBonusByOne();

	int getNumZumis() const {
		return num_zumi;
	}

	void decrementZumi() {
		if (num_zumi > 0)
			num_zumi--;
	}

	void setLevelFinished() {
		levelFinished = true;
	}

	bool getLevelFinished() const {
		return levelFinished;
	}

	Level getLevel() const {
		return lev;
	}

private:
	Actor * p1;//Contains a pointer to a Player object
	vector<Actor*> v1; //contains pointers to actor objects
	Level lev;//To return the level so that it may be used in the actor class
	int num_zumi;//number of total zumis in the level to be used with the Exit function
	int num_level;//For the displayText (increase with each passing of stage)
	unsigned int num_bonus;//For the displayText (decrease with each tick)
	unsigned int num_deployed;//Number of Sprays deployed on the screen
	bool levelFinished;//To advance to the next levl
};

#endif // STUDENTWORLD_INCLUDED
