#include "StudentWorld.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "Compiler.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir), m_field(64, std::vector<std::vector<Actor*>>(64)), winningColony(-1), simTicks(0)
{}

StudentWorld::~StudentWorld()
{
	for (size_t i = 0; i < m_CompiledAnts.size(); i++)
	{
		delete m_CompiledAnts[i];

	}
}

int StudentWorld::init()
{
	if (!compileAnts())
		return GWSTATUS_LEVEL_ERROR;
	if (!loadField())
		return GWSTATUS_LEVEL_ERROR;
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	incTicks();
	doStuff();
	checkPosition();
	handleDead();
	setDisplayText();
	return checkGameStatus(getCurrentTicks());
}

void StudentWorld::cleanUp()
{
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			for (int i = 0; i < m_field[x][y].size(); i++)
			{
				delete m_field[x][y][i];
			}
		}
	}
}

//Public Function Implememntations
bool StudentWorld::canMoveTo(int x, int y) const
{
	for (int i = 0; i < m_field[x][y].size(); i++)
	{
		if (m_field[x][y][i]->blocksMovement())
			return false;
	}
	return true;
}

void StudentWorld::addActor(Actor* a)
{
	m_field[a->getX()][a->getY()].push_back(a);
}

Actor* StudentWorld::getEdibleAt(int x, int y) const
{
	for (int i = 0; i < m_field[x][y].size(); i++)
	{
		if (m_field[x][y][i]->isEdible())
			return m_field[x][y][i];
	}
	return nullptr;
}

Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const
{
	for (int i = 0; i < m_field[x][y].size(); i++)
	{
		if (m_field[x][y][i]->isPheromone(colony))
			return m_field[x][y][i];
	}
	return nullptr;
}

bool StudentWorld::isEnemyAt(Actor* me, int x, int y, int colony) const
{
	for (int i = 0; i <  m_field[x][y].size(); i++)
	{
		if (m_field[x][y][i] != me)
			if (m_field[x][y][i]->isEnemy(colony))
				if (! m_field[x][y][i]->isDead())
					return true;
	}
	return false;
}

bool  StudentWorld::isDangerAt(Actor* me, int x, int y, int colony) const
{
	if (isEnemyAt(me, x, y, colony))
		return true;
	for (int i = 0; i < m_field[x][y].size(); i++)
	{
		if (m_field[x][y][i]->isDangerous(colony))
			return true;
	}
	return false;
}

bool  StudentWorld::isAntHillAt(int x, int y, int colony) const
{
	for (int i = 0; i < m_field[x][y].size(); i++)
	{
			if (m_field[x][y][i]->isMyAntHill(colony))
					return true;
	}
	return false;
}

//puts all biteble actors at "me"'s location in a vector 
//then randomly accesses one of the elemnts of that vector to decide who to bite.
bool StudentWorld::biteEnemyAt(Actor* me, int colony, int biteDamage)
{
	int tempX = me->getX(); //these are used becasue I didnt want to call
	int tempY = me->getY(); //me->getX() or me->getY() 10 or so times
		vector<Actor*> vec;
		for (int i = 0; i < m_field[tempX][tempY].size(); i++)
		{
			if (m_field[tempX][tempY][i] != me)
				if (m_field[tempX][tempY][i]->isEnemy(colony))
					vec.push_back(m_field[tempX][tempY][i]);
		}
		vec[randInt(0, vec.size()-1)]->getBitten(-biteDamage);
		return true;
}

bool StudentWorld::poisonAllPoisonableAt(int x, int y)
{
	for (int i = 0; i < m_field[x][y].size(); i++)
	{
		m_field[x][y][i]->getPoisoned();
	}
	return true;
}

bool StudentWorld::stunAllStunnableAt(int x, int y)
{
	for (int i = 0; i < m_field[x][y].size(); i++)
	{
		m_field[x][y][i]->getStunned();	
	}
	return true;
}

void StudentWorld::increaseScore(int colony)
{
	scores[colony]++;
}

//Private Function Implementations
void StudentWorld::checkWinner()
{
	for (int i = 0; i < 4; i++)
	{
		if (scores[i] > 5 && scores[i] > scores[winningColony])
			winningColony = i;
	}
}

void StudentWorld::incTicks()
{	simTicks++;}

int StudentWorld::getCurrentTicks() const
{	return simTicks;}

bool StudentWorld::compileAnts()
{
		// get the names of all of the ant program source files
		// we provide the getFilenamesOfAntPrograms() function for
		// you in our GameWorld class.
		std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
		std::string error;
		for (int i = 0; i < fileNames.size(); i++)
		{
			Compiler* compilerForEntrant = new Compiler;

			// compile the source files If the compile function returns
			// false, there was a syntax error during compilation!
			if (!compilerForEntrant->compile(fileNames[i], error))
			{
				// entrant 0’s source code had a syntax error!
				// send this error to our framework to warn the user.
				// do it JUST like this!
				setError(fileNames[i] + " " + error);
				// return an error to tell our simulation framework
				// that something went wrong, and it’ll inform the user
				return false;
			}
			m_CompiledAnts.push_back(compilerForEntrant);
		}
		return true;
	}

bool StudentWorld::loadField() //used in initailize it method
{
	Field f;
	string fieldFile = getFieldFilename();

	string error;
	if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
	{
		setError(fieldFile + " " + error);
		return false; // something bad happened!
	}
	// otherwise the load was successful and you can access the
	// contents of the field – here’s an example

	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			Field::FieldItem item = f.getContentsOf(x, y);
			
			//switch case statement creates new objects for every processed
			//item in the field
			switch (item)
			{
			case (Field::FieldItem::rock):
			{
				Actor* temp = new Pebble(this, x, y);
				m_field[x][y].push_back(temp);
				break;
			}
			case (Field::FieldItem::food):
			{
				Actor* temp = new Food(this, x, y, 6000);
				m_field[x][y].push_back(temp);
				break;
			}
			case (Field::FieldItem::anthill0):
			{
				if (0 < m_CompiledAnts.size())
				{
					Actor* temp = new AntHill(this, x, y, IID_ANT_TYPE0, m_CompiledAnts[IID_ANT_TYPE0]);
					m_field[x][y].push_back(temp);
				}
				break;
			}
			case (Field::FieldItem::anthill1):
			{
				if (1 < m_CompiledAnts.size())
				{
					Actor* temp = new AntHill(this, x, y, IID_ANT_TYPE1, m_CompiledAnts[IID_ANT_TYPE1]);
					m_field[x][y].push_back(temp);
				}
				break;
			}
			case (Field::FieldItem::anthill2):
			{
				if (2 < m_CompiledAnts.size())
				{
					Actor* temp = new AntHill(this, x, y, IID_ANT_TYPE2, m_CompiledAnts[IID_ANT_TYPE2]);
					m_field[x][y].push_back(temp);
				}
				break;
			}
			case (Field::FieldItem::anthill3):
			{
				if (3 < m_CompiledAnts.size())
				{
					Actor* temp = new AntHill(this, x, y, IID_ANT_TYPE3, m_CompiledAnts[IID_ANT_TYPE3]);
					m_field[x][y].push_back(temp);
				}
				break;
			}
			case (Field::FieldItem::water):
			{
				Actor* temp = new WaterPool(this, x, y);
				m_field[x][y].push_back(temp);
				break;
			}
			case (Field::FieldItem::poison):
			{
				Actor* temp = new Poison(this, x, y);
				m_field[x][y].push_back(temp);
				break;
			}
			case (Field::FieldItem::grasshopper):
			{
				Actor* temp = new BabyGrasshopper(this, x, y);
				m_field[x][y].push_back(temp);
				break;
			}
			default:
				break;
			}
		}
	}
	return true;
//return Field::FieldItem::empty;
}

void StudentWorld::doStuff()
{
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			for (int i = 0; i < m_field[x][y].size(); i++)
			{
				m_field[x][y][i]->doSomething();
			}
		}
	}
}

void StudentWorld::checkPosition()
{
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			for (int i = 0; i < m_field[x][y].size(); i++)
			{
				int checkX, checkY;
				checkX = m_field[x][y][i]->getX();
				checkY = m_field[x][y][i]->getY();

				//checks if each Actor in the graphics field has matching
				//x & y locations in the StudentWorld field data structure
				//if not moves it to the correct position in StudentWorlds field
				if ((checkX != x) || (checkY != y))
				{
					m_field[checkX][checkY].push_back(m_field[x][y][i]);
					m_field[x][y].erase(m_field[x][y].begin() + i);
				}
			}
		}
	}
}

void StudentWorld::handleDead()
{
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			for (int i = 0; i < m_field[x][y].size(); i++)
			{
				if (m_field[x][y][i]->isDead())
				{
					Actor* temp = m_field[x][y][i];
					m_field[x][y].erase(m_field[x][y].begin() + i);
					delete temp;
				}
			}
		}
	}
}

int StudentWorld::checkGameStatus(int ticks)
{
	checkWinner();
	if (ticks == 2000)
	{
		vector<string> c_Names = getFilenamesOfAntPrograms();
		if (winningColony != -1)
		{
			setWinner(c_Names[winningColony]);
			return GWSTATUS_PLAYER_WON;
		}
		else
			return GWSTATUS_NO_WINNER;
	}
	// the simulation is not yet over, continue!

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::setDisplayText()
{
	int ticks = getCurrentTicks();

	vector<string> c_Names = getFilenamesOfAntPrograms();

	int winningAntNumber = winningColony;
	

	// Create a string from your statistics, of the form:
	 //Ticks: 1134 - AmyAnt: 32 BillyAnt: 33 SuzieAnt*: 77 IgorAnt: 05

	std::ostringstream oss;
	oss << "Ticks:" << std::setw(5) << 2000-ticks <<"  -  ";
	for (int i = 0; i < c_Names.size(); i++)
	{
		oss << c_Names[i];
		if (i == winningColony)
			oss << "*";
		oss.fill('0');
		oss << ": " << std::setw(2) << scores[i];
		oss << "  ";
	}
	std::string s = oss.str();

	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}