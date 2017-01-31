#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

StudentWorld::StudentWorld()//INITIALIZES DATA MEMBERS NEEDED THROUGHOUT THE GAME
{
	num_level = 0;
}

StudentWorld::~StudentWorld()//FREES ANY REMAINING DYNAMICALLY ALLOCATED DATA
{
	delete p1;

	while (!v1.empty())
	{
		delete v1.back();
		v1.pop_back();
	}
}

int StudentWorld::init()
{
	//INITIALIZE DATA AND LOAD LEVEL 00 

	ostringstream oss;
	oss.fill('0');
	oss << setw(2) << num_level;
	string str_num_level = oss.str();

	string curLevel = "level" + str_num_level + ".dat";
	
	Level::LoadResult result = lev.loadLevel(curLevel);//Load current Level

	if (result == Level::load_fail_file_not_found)
	{
		cout << "NO FIRST LEVEL FOUND\n";
		return GWSTATUS_NO_FIRST_LEVEL;
	}
	else if (result == Level::load_fail_bad_format)
	{
		cout << "Your level was improperly formatted\n";
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_success)
	{
		cout << "Successfully loaded level\n";

		num_bonus = lev.getOptionValue(optionLevelBonus);

		//DEPLOYED NUMBER OF SPRAYS ON SCREEN IS 0
		num_deployed = 0;

		//Set Number of Zumis to 0
		num_zumi = 0;

		//Set levelFinished to False
		levelFinished = false;

		for (int x = 0; x < 15; x++)
		{
			for (int y = 0; y < 15; y++)
			{
				Level::MazeEntry item = lev.getContentsOf(x, y);

				if (item == Level::player) //ALLOCATE AND INSERT A PLAYER INTO THE GAME WORLD
				{
					p1 = new Player(this, x, y);
				}
				else if (item == Level::complex_zumi)
				{
					//Initiate complex zumi object
					ComplexZumi * cz1 = new ComplexZumi(this, x, y);
					num_zumi++;
					v1.push_back(cz1);
				}
				else if (item == Level::simple_zumi)
				{
					//Initiate simple zumi object
					SimpleZumi * sz1 = new SimpleZumi(this, x, y);
					num_zumi++;
					v1.push_back(sz1);
				}
				else if (item == Level::exit)
				{
					Exit * e1 = new Exit(this, x, y);
					v1.push_back(e1);
				}
				else if (item == Level::perma_brick) //ALLOCATE AND INSERT PERMA_BRICKS
				{
					Brick * b1 = new Brick(this, IID_PERMA_BRICK, x, y);
					v1.push_back(b1);
				}
				else if (item == Level::destroyable_brick) //ALLOCATE AND INSERT DESTROYABLE_BRICKS
				{
					Brick * b2 = new Brick(this, IID_DESTROYABLE_BRICK, x, y);
					v1.push_back(b2);
				}
			}
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	//Updates the Game Status Line
	setDisplayText();

	p1->doSomething();//will handle if dead or alive

	for (unsigned int i = 0; i < v1.size(); i++)
	{
		v1[i]->doSomething();
	}

	removeDeadActors();

	//Decrement Bonus per tick
	reduceLevelBonusByOne();

	if (!p1->isAlive())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	if (getLevelFinished())
	{
		num_level++;
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete p1;

	while (!v1.empty())
	{
		delete v1.back();
		v1.pop_back();
	}
}

void StudentWorld::setDisplayText()
{
	int score = getScore();
	int level = num_level;
	int livesLeft = getLives();
	
	ostringstream oss_score;
	oss_score.fill('0');
	oss_score << setw(8) << score;
	string s_score = oss_score.str();

	ostringstream oss_level;
	oss_level.fill('0');
	oss_level << setw(2) << num_level;
	string s_level = oss_level.str();

	ostringstream oss_bonus;
	oss_bonus.fill(' ');
	oss_bonus << setw(6) << num_bonus;
	string s_bonus = oss_bonus.str();

	ostringstream oss_lives;
	oss_lives.fill('0');
	oss_lives << setw(3) << livesLeft;
	string s_lives = oss_lives.str();

	string s = "Score: " + s_score + "  Level: " + s_level + "  Lives: " + s_lives + "  Bonus: " + s_bonus;

	setGameStatText(s);
}

void StudentWorld::removeDeadActors()
{
	vector<Actor*>::iterator it;
	it = v1.begin();

	while (it != v1.end())
	{
		if (!(*it)->isAlive())
		{
			delete (*it);
			it = v1.erase(it);
		}
		else {
			++it;
		}
	}
}

void StudentWorld::reduceLevelBonusByOne()
{
	if (num_bonus > 0)
		num_bonus--;
}

Actor* StudentWorld::getActor(int x, int y) const
{
	for (unsigned int i = 0; i < v1.size() ; i++)
	{
		if (v1[i]->getX() == x && v1[i]->getY() == y)
			return v1[i];
	}

	return nullptr;
}

void StudentWorld::addActor(Actor *a)
{
	v1.push_back(a);
}

void StudentWorld::getPlayerLocation(int& x, int& y) const
{
	x = p1->getX();
	y = p1->getY();
}

void StudentWorld::killPlayer()
{
	p1->setDead();
}