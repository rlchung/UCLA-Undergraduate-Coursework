#include "Actor.h"
#include "GraphObject.h"
#include "GameConstants.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <queue>
#include <sstream>

#include "StudentWorld.h"
#include "Level.h"

using namespace std;

Actor::Actor(StudentWorld* world, int imageID, int startX, int startY) : GraphObject(imageID, startX, startY)
{
	if (imageID != IID_PLAYER && imageID != IID_SIMPLE_ZUMI && imageID != IID_COMPLEX_ZUMI && imageID != IID_EXIT && imageID != IID_BUGSPRAYER && imageID != IID_BUGSPRAY && imageID != IID_EXTRA_LIFE_GOODIE &&
		imageID != IID_WALK_THRU_GOODIE && imageID != IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE && imageID != IID_PERMA_BRICK && imageID != IID_DESTROYABLE_BRICK)
	{
		cout << "INVALID IMAGE ENTERED\n";
		exit(1);
	}

	myWorld = world;
	m_alive = true;
	set_attackable(false);//For the bugspray
	setVisible(true);//For the exit object, make sure to first set it to false
}

StudentWorld* Actor::getWorld() const
{
	return myWorld;
}

Brick::Brick(StudentWorld* world, int imageID, int startX, int startY) : Actor(world, imageID, startX, startY)
{
	if (imageID != IID_PERMA_BRICK && imageID != IID_DESTROYABLE_BRICK)
	{
		cout << "INVALID IMAGE ENTERED FOR BRICK\n";
		exit(1);
	}

	//VALUE OF ATTACKABLE IS TRUE OR FALSE DEPENDING ON WHETHER OR NOT IT IS PERMANENT OR NOT
	if (imageID == IID_DESTROYABLE_BRICK)
		set_attackable(true);
}

Player::Player(StudentWorld* world, int startX, int startY) : Actor(world, IID_PLAYER, startX, startY)
{
	setTimeMaxMode(0);
	setTimeWalkThrough(0);
	set_attackable(true);
}

void Player::doSomething()
{
	if (!isAlive())
		return;

	else
	{
		int ch;

		if (getWorld()->getKey(ch))
		{
			//user hit a key this tick!

			switch (ch)
			{
			case KEY_PRESS_LEFT:
				//The block to which the player is moving to must either be: a nullptr/empty space, is a destroyable brick and player has walkThrough on, or it is a BugSprayer
				if (getWorld()->getActor(getX() - 1, getY()) == nullptr || getWorld()->getActor(getX() - 1, getY())->getID() == IID_BUGSPRAYER ||
					getWorld()->getActor(getX() - 1, getY())->getID() == IID_DESTROYABLE_BRICK && getTimeWalkThrough() > 0 ||
					getWorld()->getActor(getX() - 1, getY())->getID() == IID_EXIT || getWorld()->getActor(getX() - 1, getY())->getID() == IID_EXTRA_LIFE_GOODIE ||
					getWorld()->getActor(getX() - 1, getY())->getID() == IID_WALK_THRU_GOODIE || getWorld()->getActor(getX() - 1, getY())->getID() == IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE ||
					getWorld()->getActor(getX() - 1, getY())->getID() == IID_SIMPLE_ZUMI || getWorld()->getActor(getX() - 1, getY())->getID() == IID_COMPLEX_ZUMI)
					moveTo(getX() - 1, getY());
				break;
			case KEY_PRESS_RIGHT:
				if (getWorld()->getActor(getX() + 1, getY()) == nullptr || getWorld()->getActor(getX() + 1, getY())->getID() == IID_BUGSPRAYER ||
					getWorld()->getActor(getX() + 1, getY())->getID() == IID_DESTROYABLE_BRICK && getTimeWalkThrough() > 0 ||
					getWorld()->getActor(getX() + 1, getY())->getID() == IID_EXIT || getWorld()->getActor(getX() + 1, getY())->getID() == IID_EXTRA_LIFE_GOODIE ||
					getWorld()->getActor(getX() + 1, getY())->getID() == IID_WALK_THRU_GOODIE || getWorld()->getActor(getX() + 1, getY())->getID() == IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE ||
					getWorld()->getActor(getX() + 1, getY())->getID() == IID_SIMPLE_ZUMI || getWorld()->getActor(getX() + 1, getY())->getID() == IID_COMPLEX_ZUMI)
					moveTo(getX() + 1, getY());
				break;
			case KEY_PRESS_UP:
				if (getWorld()->getActor(getX(), getY() + 1) == nullptr || getWorld()->getActor(getX(), getY() + 1)->getID() == IID_BUGSPRAYER ||
					getWorld()->getActor(getX(), getY() + 1)->getID() == IID_DESTROYABLE_BRICK && getTimeWalkThrough() > 0 ||
					getWorld()->getActor(getX(), getY() + 1)->getID() == IID_EXIT || getWorld()->getActor(getX(), getY() + 1)->getID() == IID_EXTRA_LIFE_GOODIE ||
					getWorld()->getActor(getX(), getY() + 1)->getID() == IID_WALK_THRU_GOODIE || getWorld()->getActor(getX(), getY() + 1)->getID() == IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE ||
					getWorld()->getActor(getX(), getY() + 1)->getID() == IID_SIMPLE_ZUMI || getWorld()->getActor(getX(), getY() + 1)->getID() == IID_COMPLEX_ZUMI)
					moveTo(getX(), getY() + 1);
				break;
			case KEY_PRESS_DOWN:
				if (getWorld()->getActor(getX(), getY() - 1) == nullptr || getWorld()->getActor(getX(), getY() - 1)->getID() == IID_BUGSPRAYER ||
					getWorld()->getActor(getX(), getY() - 1)->getID() == IID_DESTROYABLE_BRICK && getTimeWalkThrough() > 0 ||
					getWorld()->getActor(getX(), getY() - 1)->getID() == IID_EXIT || getWorld()->getActor(getX(), getY() - 1)->getID() == IID_EXTRA_LIFE_GOODIE ||
					getWorld()->getActor(getX(), getY() - 1)->getID() == IID_WALK_THRU_GOODIE || getWorld()->getActor(getX(), getY() - 1)->getID() == IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE ||
					getWorld()->getActor(getX(), getY() - 1)->getID() == IID_SIMPLE_ZUMI || getWorld()->getActor(getX(), getY() - 1)->getID() == IID_COMPLEX_ZUMI)
					moveTo(getX(), getY() - 1);
				break;
			case KEY_PRESS_SPACE:
				if (getWorld()->getActor(getX(), getY()) == nullptr && getTimeMaxMode() == 0 && getWorld()->getDeployed() < 2)
					getWorld()->addActor(new BugSprayer(getWorld(), getX(), getY(), IID_BUGSPRAYER));
				else if (getWorld()->getActor(getX(), getY()) == nullptr && getTimeMaxMode() > 0 && getWorld()->getDeployed() < getWorld()->getLevel().getOptionValue(optionMaxBoostedSprayers))
					getWorld()->addActor(new BugSprayer(getWorld(), getX(), getY(), IID_BUGSPRAYER));
				break;
			}		
		}
		
		if (getTimeWalkThrough() > 0)
		{
			time_walk_through--;
		}

		//If WalkThroughGoodie runs out while on a brick, player dies
		if (getTimeWalkThrough() == 0 && (getWorld()->getActor(getX(), getY()) != nullptr && getWorld()->getActor(getX(), getY())->getID() == IID_DESTROYABLE_BRICK))
		{
			setDead();
			getWorld()->GameWorld::playSound(SOUND_PLAYER_DIE);
			return;
		}

		if (getTimeMaxMode() > 0)
		{
			time_maxMode--;
		}

		//If the Player steps on BugSpray, player dies
		if (getWorld()->getActor(getX(), getY()) != nullptr && getWorld()->getActor(getX(), getY())->getID() == IID_BUGSPRAY)
		{
			setDead();
			getWorld()->GameWorld::playSound(SOUND_PLAYER_DIE);
			return;
		}

		if (getWorld()->getActor(getX(), getY()) != nullptr && (getWorld()->getActor(getX(), getY())->getID() == IID_SIMPLE_ZUMI || getWorld()->getActor(getX(), getY())->getID() == IID_COMPLEX_ZUMI))
		{
			setDead();
			getWorld()->GameWorld::playSound(SOUND_PLAYER_DIE);
			return;
		}
	}
}

TimedLifetimeActor::TimedLifetimeActor(StudentWorld* world, int startX, int startY, int imageID, unsigned int lifetime)
	: Actor(world, imageID, startX, startY)
{
	m_lifetime = lifetime;
	set_attackable(false);//true only for: BugSprayer
}

void TimedLifetimeActor::expireImmediately()
{
	m_lifetime = 0;
}

BugSprayer::BugSprayer(StudentWorld* world, int startX, int startY, int imageID)
	: TimedLifetimeActor(world, startX, startY, imageID, 40)
{
	getWorld()->incrementDeployed();
	set_attackable(true);
}

void BugSprayer::doSomething()
{
	if (isAlive() == false)
		return;

	decrementLifetime();

	if (getLifetime() == 0)
	{
		releaseSpray();
		TimedLifetimeActor::setDead();
		getWorld()->decrementDeployed();
		getWorld()->GameWorld::playSound(SOUND_SPRAY);
	}
}


void BugSprayer::setDead()
{
	expireImmediately();
}

void BugSprayer::releaseSpray()
{

	getWorld()->addActor(new BugSpray(getWorld(), getX(), getY()));

	//***********RIGHT***********//
	for (int i = 1; i < 3; i++)
	{
		//IF THE ITEM IS A PERMA BRICK, BREAK FROM LOOP
		if (getWorld()->getActor(getX() + i, getY()) != nullptr && getWorld()->getActor(getX() + i, getY())->getID() == IID_PERMA_BRICK)
			break;

		//IF THE FIRST ITEM IS DESTROYABLE BRICK AND THE SECOND IS EITHER NULL OR NOT A DESTROYABLE BRICK, ADD SPRAY ON FIRST ITEM AND BREAK
		if (getWorld()->getActor(getX() + 1, getY()) != nullptr && getWorld()->getActor(getX() + 1, getY())->getID() == IID_DESTROYABLE_BRICK && (
			getWorld()->getActor(getX() + 2, getY()) == nullptr || getWorld()->getActor(getX() + 2, getY())->getID() != IID_DESTROYABLE_BRICK))
		{
			getWorld()->addActor(new BugSpray(getWorld(), getX() + 1, getY()));
			break;
		}
		//IF THE ITEM IS NOT A PERMABRICK, ADD SPRAY
		if (getWorld()->getActor(getX() + i, getY()) != nullptr && getWorld()->getActor(getX() + i, getY())->getID() != IID_PERMA_BRICK)
			getWorld()->addActor(new BugSpray(getWorld(), getX() + i, getY()));

		//IF THE ITEM IS A NULLPTR, ADD SPRAY
		if (getWorld()->getActor(getX() + i, getY()) == nullptr)
			getWorld()->addActor(new BugSpray(getWorld(), getX() + i, getY()));
	}

	//***********LEFT***********//
	for (int i = 1; i < 3; i++)
	{
		//IF THE ITEM IS A PERMA BRICK, BREAK FROM LOOP
		if (getWorld()->getActor(getX() - i, getY()) != nullptr && getWorld()->getActor(getX() - i, getY())->getID() == IID_PERMA_BRICK)
			break;

		//IF THE FIRST ITEM IS DESTROYABLE BRICK AND THE SECOND IS EITHER NULL OR NOT A DESTROYABLE BRICK, ADD SPRAY ON FIRST ITEM AND BREAK
		if (getWorld()->getActor(getX() - 1, getY()) != nullptr && getWorld()->getActor(getX() - 1, getY())->getID() == IID_DESTROYABLE_BRICK && (
			getWorld()->getActor(getX() - 2, getY()) == nullptr || getWorld()->getActor(getX() - 2, getY())->getID() != IID_DESTROYABLE_BRICK))
		{
			getWorld()->addActor(new BugSpray(getWorld(), getX() - 1, getY()));
			break;
		}
		//IF THE ITEM IS NOT A PERMABRICK, ADD SPRAY
		if (getWorld()->getActor(getX() - i, getY()) != nullptr && getWorld()->getActor(getX() - i, getY())->getID() != IID_PERMA_BRICK)
			getWorld()->addActor(new BugSpray(getWorld(), getX() - i, getY()));

		//IF THE ITEM IS A NULLPTR, ADD SPRAY
		if (getWorld()->getActor(getX() - i, getY()) == nullptr)
			getWorld()->addActor(new BugSpray(getWorld(), getX() - i, getY()));
	}

	//***********DOWN***********//
	for (int i = 1; i < 3; i++)
	{
		//IF THE ITEM IS A PERMA BRICK, BREAK FROM LOOP
		if (getWorld()->getActor(getX(), getY() + i) != nullptr && getWorld()->getActor(getX(), getY() + i)->getID() == IID_PERMA_BRICK)
			break;

		//IF THE FIRST ITEM IS DESTROYABLE BRICK AND THE SECOND IS EITHER NULL OR NOT A DESTROYABLE BRICK, ADD SPRAY ON FIRST ITEM AND BREAK
		if (getWorld()->getActor(getX(), getY() + 1) != nullptr && getWorld()->getActor(getX() , getY() + 1)->getID() == IID_DESTROYABLE_BRICK && (
			getWorld()->getActor(getX(), getY() + 2) == nullptr || getWorld()->getActor(getX() , getY() + 2)->getID() != IID_DESTROYABLE_BRICK))
		{
			getWorld()->addActor(new BugSpray(getWorld(), getX() , getY() + 1));
			break;
		}
		//IF THE ITEM IS NOT A PERMABRICK, ADD SPRAY
		if (getWorld()->getActor(getX(), getY() + i) != nullptr && getWorld()->getActor(getX() , getY() + i)->getID() != IID_PERMA_BRICK)
			getWorld()->addActor(new BugSpray(getWorld(), getX() , getY() + i));

		//IF THE ITEM IS A NULLPTR, ADD SPRAY
		if (getWorld()->getActor(getX(), getY() + i) == nullptr)
			getWorld()->addActor(new BugSpray(getWorld(), getX() , getY() + i));
	}

	//***********UP***********//
	for (int i = 1; i < 3; i++)
	{
		//IF THE ITEM IS A PERMA BRICK, BREAK FROM LOOP
		if (getWorld()->getActor(getX(), getY() - i) != nullptr && getWorld()->getActor(getX(), getY() - i)->getID() == IID_PERMA_BRICK)
			break;

		//IF THE FIRST ITEM IS DESTROYABLE BRICK AND THE SECOND IS EITHER NULL OR NOT A DESTROYABLE BRICK, ADD SPRAY ON FIRST ITEM AND BREAK
		if (getWorld()->getActor(getX(), getY() - 1) != nullptr && getWorld()->getActor(getX(), getY() - 1)->getID() == IID_DESTROYABLE_BRICK && (
			getWorld()->getActor(getX(), getY() - 2) == nullptr || getWorld()->getActor(getX(), getY() - 2)->getID() != IID_DESTROYABLE_BRICK))
		{
			getWorld()->addActor(new BugSpray(getWorld(), getX(), getY() - 1));
			break;
		}
		//IF THE ITEM IS NOT A PERMABRICK, ADD SPRAY
		if (getWorld()->getActor(getX(), getY() - i) != nullptr && getWorld()->getActor(getX(), getY() - i)->getID() != IID_PERMA_BRICK)
			getWorld()->addActor(new BugSpray(getWorld(), getX(), getY() - i));

		//IF THE ITEM IS A NULLPTR, ADD SPRAY
		if (getWorld()->getActor(getX(), getY() - i) == nullptr)
			getWorld()->addActor(new BugSpray(getWorld(), getX(), getY() - i));
	}
}

BugSpray::BugSpray(StudentWorld* world, int startX, int startY) : TimedLifetimeActor(world, startX, startY, IID_BUGSPRAY, 3)
{
}

void BugSpray::doSomething()
{
	if (!isAlive())
		return;

	decrementLifetime();

	if (getLifetime() == 0)
		setDead();
	else
	{	
		//If it is the player or a brick, just setDead()
		if (getWorld()->getActor(getX(), getY()) != nullptr && getWorld()->getActor(getX(), getY())->get_attackable() && (getWorld()->getActor(getX(), getY())->getID() != IID_SIMPLE_ZUMI ||
			getWorld()->getActor(getX(), getY())->getID() != IID_COMPLEX_ZUMI))
			getWorld()->getActor(getX(), getY())->setDead();

		//If it is a Zumi that is killed, perform death procedure
		if (getWorld()->getActor(getX(), getY()) != nullptr && getWorld()->getActor(getX(), getY())->get_attackable() && (getWorld()->getActor(getX(), getY())->getID() == IID_SIMPLE_ZUMI ||
			getWorld()->getActor(getX(), getY())->getID() == IID_COMPLEX_ZUMI))
		{
			getWorld()->decrementZumi();//So that the exit may appear when there are 0 zumis left
			getWorld()->getActor(getX(), getY())->setDead();
			getWorld()->increaseScore(100);
			getWorld()->playSound(SOUND_ENEMY_DIE);

			srand((unsigned int)time(NULL));
			int rand_num = rand() % 100;

			if ((unsigned int)(rand() % 100) < getWorld()->getLevel().getOptionValue(optionProbOfGoodieOverall))
			{
				if (rand_num < (int)getWorld()->getLevel().getOptionValue(optionProbOfExtraLifeGoodie))
					getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY()));
				else if (rand_num < (int)getWorld()->getLevel().getOptionValue(optionProbOfMoreSprayersGoodie))
					getWorld()->addActor(new IncreaseSimultaneousSprayersGoodie(getWorld(), getX(), getY()));
				else if (rand_num < (int)getWorld()->getLevel().getOptionValue(optionProbOfWalkThruGoodie))
					getWorld()->addActor(new WalkThroughWallsGoodie(getWorld(), getX(), getY()));
			}
		}
	}
}

Exit::Exit(StudentWorld* world, int startX, int startY) : Actor(world, IID_EXIT, startX, startY)
{
	setVisible(false);///TRUE FOR TESTING
}

void Exit::doSomething()
{
	int p1X, p1Y;
	getWorld()->getPlayerLocation(p1X, p1Y);

	if (getWorld()->getNumZumis() == 0)
		setVisible(true);
	if (p1X == getX() && p1Y == getY() && isVisible())//And condition that number of Live Zumi's in level is == 0
	{
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->increaseScore(getWorld()->getBonus());
		getWorld()->setLevelFinished();
	}
}

Goodie::Goodie(StudentWorld* world, int imageID, int startX, int startY) : TimedLifetimeActor(world, startX, startY, imageID, world->getLevel().getOptionValue(optionGoodieLifetimeInTicks))
{
	setVisible(true);//SETS TRUE FOR TESTING
}

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, IID_EXTRA_LIFE_GOODIE, startX, startY)
{
}

void ExtraLifeGoodie::doSomething()
{
	if (!isAlive())
		return;

	decrementLifetime();

	if (getLifetime() == 0)
		setDead();
	else
	{
		int p1X, p1Y;
		getWorld()->getPlayerLocation(p1X, p1Y);

		if (getX() == p1X && getY() == p1Y)
		{
			getWorld()->incLives();
			getWorld()->increaseScore(1000);
			setDead();
			getWorld()->playSound(SOUND_GOT_GOODIE);
		}
		else
			;//do nothing
	}
}

WalkThroughWallsGoodie::WalkThroughWallsGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, IID_WALK_THRU_GOODIE, startX, startY)
{
}

void WalkThroughWallsGoodie::doSomething()
{
	if (!isAlive())
		return;

	decrementLifetime();

	if (getLifetime() == 0)
		setDead();
	else
	{
		int p1X, p1Y;
		getWorld()->getPlayerLocation(p1X, p1Y);

		if (getX() == p1X && getY() == p1Y)
		{
			getWorld()->getPlayer()->setTimeWalkThrough(getWorld()->getLevel().getOptionValue(optionWalkThruLifetimeTicks));
			getWorld()->increaseScore(1000);
			setDead();
			getWorld()->playSound(SOUND_GOT_GOODIE);
		}
		else
			;//do nothing
	}
}

IncreaseSimultaneousSprayersGoodie::IncreaseSimultaneousSprayersGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, startX, startY)
{
}

void IncreaseSimultaneousSprayersGoodie::doSomething()
{
	if (!isAlive())
		return;

	decrementLifetime();

	if (getLifetime() == 0)
		setDead();
	else
	{
		int p1X, p1Y;
		getWorld()->getPlayerLocation(p1X, p1Y);

		if (getX() == p1X && getY() == p1Y)
		{
			getWorld()->getPlayer()->setTimeMaxMode(getWorld()->getLevel().getOptionValue(optionBoostedSprayerLifetimeTicks));
			getWorld()->increaseScore(1000);
			setDead();
			getWorld()->playSound(SOUND_GOT_GOODIE);
		}

	}
}

Zumi::Zumi(StudentWorld* world, int imageID, int actionTicks, int startX, int startY) : Actor(world, imageID, startX, startY)
{
	set_attackable(true);
	srand((unsigned int)time(NULL));
	currentDirection = rand() % 4 + 1;
	m_currentActionTicks = 0;
	m_MaxActionTick = actionTicks;
}

SimpleZumi::SimpleZumi(StudentWorld* world, int startX, int startY) : Zumi(world, IID_SIMPLE_ZUMI, world->getLevel().getOptionValue(optionTicksPerSimpleZumiMove), startX, startY)
{
}

void SimpleZumi::doSomething() {
	
	if (!isAlive())
		return;
	
	int p1X, p1Y;
	getWorld()->getPlayerLocation(p1X, p1Y);

	incrementActionTicks();

	if (getCurrentActionTicks() == getMaxActionTick())//if it has incremented to the last tick of the retrieved optiontick
	{
		switch (getDirection())
		{
		case 1:
			//If the block that the simple zumi is moving to is not a Bug Sprayer or Perma Brick or a Temporary brick, move
			//If it is, it must do nothing, pick a new direction, and reset it's current Action Ticks

			if (getWorld()->getActor(getX() - 1, getY()) == nullptr || (getWorld()->getActor(getX() - 1, getY())->getID() != IID_BUGSPRAYER &&
				getWorld()->getActor(getX() - 1, getY())->getID() != IID_DESTROYABLE_BRICK && getWorld()->getActor(getX() - 1, getY())->getID() != IID_PERMA_BRICK))
				moveTo(getX() - 1, getY());
			if (getWorld()->getActor(getX() - 1, getY()) != nullptr && (getWorld()->getActor(getX() - 1, getY())->getID() == IID_BUGSPRAYER ||
				getWorld()->getActor(getX() - 1, getY())->getID() == IID_DESTROYABLE_BRICK || getWorld()->getActor(getX() - 1, getY())->getID() == IID_PERMA_BRICK))
			{
				srand((unsigned int)time(NULL));
				setCurrentDirection(rand() % 4 + 1);
				resetActionTicks();
				return;
			}

			resetActionTicks();
			break; 
		case 2:
			if (getWorld()->getActor(getX() + 1, getY()) == nullptr || (getWorld()->getActor(getX() + 1, getY())->getID() != IID_BUGSPRAYER &&
				getWorld()->getActor(getX() + 1, getY())->getID() != IID_DESTROYABLE_BRICK && getWorld()->getActor(getX() + 1, getY())->getID() != IID_PERMA_BRICK))
				moveTo(getX() + 1, getY());
			if (getWorld()->getActor(getX() + 1, getY()) != nullptr && (getWorld()->getActor(getX() + 1, getY())->getID() == IID_BUGSPRAYER ||
				getWorld()->getActor(getX() + 1, getY())->getID() == IID_DESTROYABLE_BRICK || getWorld()->getActor(getX() + 1, getY())->getID() == IID_PERMA_BRICK))
			{
				srand((unsigned int)time(NULL));
				setCurrentDirection(rand() % 4 + 1);
				resetActionTicks();
				return;
			}

			resetActionTicks();
			break;

		case 3:
			if (getWorld()->getActor(getX(), getY() - 1) == nullptr || (getWorld()->getActor(getX(), getY() - 1)->getID() != IID_BUGSPRAYER &&
				getWorld()->getActor(getX(), getY() - 1)->getID() != IID_DESTROYABLE_BRICK && getWorld()->getActor(getX(), getY() - 1)->getID() != IID_PERMA_BRICK))
				moveTo(getX(), getY() - 1);
			if (getWorld()->getActor(getX(), getY() - 1) != nullptr && (getWorld()->getActor(getX(), getY() - 1)->getID() == IID_BUGSPRAYER ||
				getWorld()->getActor(getX(), getY() - 1)->getID() == IID_DESTROYABLE_BRICK || getWorld()->getActor(getX(), getY() - 1)->getID() == IID_PERMA_BRICK))
			{
				srand((unsigned int)time(NULL));
				setCurrentDirection(rand() % 4 + 1);
				resetActionTicks();
				return;
			}

			resetActionTicks();
			break;
		case 4:
			if (getWorld()->getActor(getX(), getY() + 1) == nullptr || (getWorld()->getActor(getX(), getY() + 1)->getID() != IID_BUGSPRAYER &&
				getWorld()->getActor(getX(), getY() + 1)->getID() != IID_DESTROYABLE_BRICK && getWorld()->getActor(getX(), getY() + 1)->getID() != IID_PERMA_BRICK))
				moveTo(getX(), getY() + 1);
			if (getWorld()->getActor(getX(), getY() + 1) != nullptr && (getWorld()->getActor(getX(), getY() + 1)->getID() == IID_BUGSPRAYER ||
				getWorld()->getActor(getX(), getY() + 1)->getID() == IID_DESTROYABLE_BRICK || getWorld()->getActor(getX(), getY() + 1)->getID() == IID_PERMA_BRICK))
			{
				srand((unsigned int)time(NULL));
				setCurrentDirection(rand() % 4 + 1);
				resetActionTicks();
				return;
			}

			resetActionTicks();
			break;
		}
	}

	if (getX() == p1X && getY() == p1Y)//Check if after the zumi moved(or not), if it has killed the player
	{
		getWorld()->getPlayer()->setDead();
		getWorld()->playSound(SOUND_PLAYER_DIE);
		return;
	}
}

ComplexZumi::ComplexZumi(StudentWorld* world, int startX, int startY) : Zumi(world, IID_COMPLEX_ZUMI, world->getLevel().getOptionValue(optionTicksPerComplexZumiMove), startX, startY)
{
}
void ComplexZumi::doSomething() {

	if (!isAlive())
		return;

	int p1X, p1Y;
	getWorld()->getPlayerLocation(p1X, p1Y);

	incrementActionTicks();

	if (getCurrentActionTicks() == getMaxActionTick())//if it has incremented to the last tick of the retrieved optiontick
	{
		switch (getDirection())
		{
		case 1:
			//If the block that the simple zumi is moving to is not a Bug Sprayer or Perma Brick or a Temporary brick, move
			//If it is, it must do nothing, pick a new direction, and reset it's current Action Ticks

			if (getWorld()->getActor(getX() - 1, getY()) == nullptr || (getWorld()->getActor(getX() - 1, getY())->getID() != IID_BUGSPRAYER &&
				getWorld()->getActor(getX() - 1, getY())->getID() != IID_DESTROYABLE_BRICK && getWorld()->getActor(getX() - 1, getY())->getID() != IID_PERMA_BRICK))
				moveTo(getX() - 1, getY());
			if (getWorld()->getActor(getX() - 1, getY()) != nullptr && (getWorld()->getActor(getX() - 1, getY())->getID() == IID_BUGSPRAYER ||
				getWorld()->getActor(getX() - 1, getY())->getID() == IID_DESTROYABLE_BRICK || getWorld()->getActor(getX() - 1, getY())->getID() == IID_PERMA_BRICK))
			{
				srand((unsigned int)time(NULL));
				setCurrentDirection(rand() % 4 + 1);
				resetActionTicks();
				return;
			}

			resetActionTicks();
			break;
		case 2:
			if (getWorld()->getActor(getX() + 1, getY()) == nullptr || (getWorld()->getActor(getX() + 1, getY())->getID() != IID_BUGSPRAYER &&
				getWorld()->getActor(getX() + 1, getY())->getID() != IID_DESTROYABLE_BRICK && getWorld()->getActor(getX() + 1, getY())->getID() != IID_PERMA_BRICK))
				moveTo(getX() + 1, getY());
			if (getWorld()->getActor(getX() + 1, getY()) != nullptr && (getWorld()->getActor(getX() + 1, getY())->getID() == IID_BUGSPRAYER ||
				getWorld()->getActor(getX() + 1, getY())->getID() == IID_DESTROYABLE_BRICK || getWorld()->getActor(getX() + 1, getY())->getID() == IID_PERMA_BRICK))
			{
				srand((unsigned int)time(NULL));
				setCurrentDirection(rand() % 4 + 1);
				resetActionTicks();
				return;
			}

			resetActionTicks();
			break;

		case 3:
			if (getWorld()->getActor(getX(), getY() - 1) == nullptr || (getWorld()->getActor(getX(), getY() - 1)->getID() != IID_BUGSPRAYER &&
				getWorld()->getActor(getX(), getY() - 1)->getID() != IID_DESTROYABLE_BRICK && getWorld()->getActor(getX(), getY() - 1)->getID() != IID_PERMA_BRICK))
				moveTo(getX(), getY() - 1);
			if (getWorld()->getActor(getX(), getY() - 1) != nullptr && (getWorld()->getActor(getX(), getY() - 1)->getID() == IID_BUGSPRAYER ||
				getWorld()->getActor(getX(), getY() - 1)->getID() == IID_DESTROYABLE_BRICK || getWorld()->getActor(getX(), getY() - 1)->getID() == IID_PERMA_BRICK))
			{
				srand((unsigned int)time(NULL));
				setCurrentDirection(rand() % 4 + 1);
				resetActionTicks();
				return;
			}

			resetActionTicks();
			break;
		case 4:
			if (getWorld()->getActor(getX(), getY() + 1) == nullptr || (getWorld()->getActor(getX(), getY() + 1)->getID() != IID_BUGSPRAYER &&
				getWorld()->getActor(getX(), getY() + 1)->getID() != IID_DESTROYABLE_BRICK && getWorld()->getActor(getX(), getY() + 1)->getID() != IID_PERMA_BRICK))
				moveTo(getX(), getY() + 1);
			if (getWorld()->getActor(getX(), getY() + 1) != nullptr && (getWorld()->getActor(getX(), getY() + 1)->getID() == IID_BUGSPRAYER ||
				getWorld()->getActor(getX(), getY() + 1)->getID() == IID_DESTROYABLE_BRICK || getWorld()->getActor(getX(), getY() + 1)->getID() == IID_PERMA_BRICK))
			{
				srand((unsigned int)time(NULL));
				setCurrentDirection(rand() % 4 + 1);
				resetActionTicks();
				return;
			}

			resetActionTicks();
			break;
		}
	}

	if (getX() == p1X && getY() == p1Y)//Check if after the zumi moved(or not), if it has killed the player
	{
		getWorld()->getPlayer()->setDead();
		getWorld()->playSound(SOUND_PLAYER_DIE);
		return;
	}
}