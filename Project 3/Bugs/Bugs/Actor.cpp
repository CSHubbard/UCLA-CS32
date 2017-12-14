#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include <vector>
#include <stdlib.h> 

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor Class implementations
Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth)
	:GraphObject(imageID, startX, startY, startDir, depth), m_World(world)
{}

bool Actor::isDead() const
{	return false;}

bool Actor::blocksMovement() const
{	return false;}

 void Actor::getBitten(int amt)
 {/*Do Nothing*/}

void Actor::getPoisoned()
{/*Do Nothing*/}

void Actor::getStunned()
{/*Do Nothing*/}

bool Actor::isEdible() const
{	return false;}

bool Actor::isPheromone(int colony) const
{	return false;}

bool Actor::isEnemy(int colony) const
{	return false;}

bool Actor::isDangerous(int colony) const
{	return false;}

bool Actor::isMyAntHill(int colony) const
{	return false;}

StudentWorld* Actor::getWorld() const
{
	return m_World;
}

//Pebble Implementations
Pebble::Pebble(StudentWorld* sw, int startX, int startY)
	:Actor(sw, startX, startY, right, IID_ROCK, 1)
{}

void Pebble::doSomething()
{/*Do Nothing*/}

bool Pebble::blocksMovement() const
{	return true;}

//EnergyHolder Implementations
EnergyHolder::EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth)
	:Actor(sw, startX, startY, startDir, imageID, depth), m_Energy(energy), im_Dead(false), m_HeldFood(0)
{}

bool EnergyHolder::isDead() const
{
	if (getEnergy() <= 0)
		return true;
	return false;
}

void EnergyHolder::killMe()
{
	im_Dead = true;
	if (becomesFoodUponDeath())
		addFood(100);
}

int EnergyHolder::getEnergy() const
{
	return m_Energy;
}

void EnergyHolder::updateEnergy(int amt)
{
	m_Energy += amt;
}

void EnergyHolder::addFood(int amt)
{
	Actor* foodXY;
	foodXY = getWorld()->getEdibleAt(getX(), getY());
	if (foodXY == nullptr)
	{
		Actor* temp = new Food(getWorld(), getX(), getY(), amt);
		getWorld()->addActor(temp);
	}
	if (dynamic_cast<Food*>(foodXY) == nullptr)
		return;
	dynamic_cast<Food&>(*foodXY).updateEnergy(amt);
}

int EnergyHolder::pickupFood(int amt)
{
	Actor* foodXY;
	foodXY = getWorld()->getEdibleAt(getX(), getY());
	if (foodXY == nullptr)
		return 0;
	else
	{
		//takes only food avalible if food avalible is less than amount
		if (dynamic_cast<Food&>(*foodXY).getEnergy() < amt)
			amt = dynamic_cast<Food&>(*foodXY).getEnergy();
		dynamic_cast<Food&>(*foodXY).updateEnergy(-amt);
		updateHeldFood(amt);
		return getHeldFood();
	}
}

int EnergyHolder::pickupAndEatFood(int amt)
{
	updateEnergy(pickupFood(amt));
	if (getHeldFood() == 0)
		return 0;
	int foodEaten = getHeldFood();
	updateHeldFood(-getHeldFood());
	return foodEaten;
}

bool EnergyHolder::becomesFoodUponDeath() const
{
	return false;
}

void EnergyHolder::updateHeldFood(int amt)
{
	m_HeldFood += amt;
	if (m_HeldFood < 0)
		m_HeldFood = 0;
}

int EnergyHolder::getHeldFood() const
{
	return m_HeldFood;
}

//Food Implementations
Food::Food(StudentWorld* sw, int startX, int startY, int energy)
	: EnergyHolder(sw, startX, startY, right, energy, IID_FOOD, 2)
{}

void Food::doSomething()
{/*Do Nothing*/}

bool Food::isEdible() const
{	return true;}

//AntHill Implementations
AntHill::AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program)
	:EnergyHolder(sw, startX, startY, right, 8999, IID_ANT_HILL, 2), m_Colony(colony), m_Program(program)
{}

void AntHill::doSomething()
{
	updateEnergy(-1);
	if (isDead())
	{
		killMe();
		return;
	}
	if (pickupAndEatFood(10000) > 0)
		return;
	tryCreateAnt();
}

bool AntHill::isMyAntHill(int colony) const
{
	if (m_Colony == colony)
		return true;
	return false;
}

//private AntHill implementations
void AntHill::tryCreateAnt()
{
	if (getEnergy() >= 2000)
	{
		Actor* temp = new Ant(getWorld(), getX(), getY(), m_Colony, m_Program, m_Colony);
		getWorld()->addActor(temp);
		updateEnergy(-1500);
		getWorld()->increaseScore(m_Colony);
	}
}

//Pheromone Implementations
Pheromone::Pheromone(StudentWorld* sw, int startX, int startY, int colony)
	:EnergyHolder(sw, startX, startY, right, 256, colony + 11, 2), m_Colony(colony)
{}

void Pheromone::doSomething()
{
	updateEnergy(-1);
	if (isDead())
	{
		killMe();
		return;
	}
}
bool Pheromone::isPheromone(int colony) const
{
	if (m_Colony == colony)
		return true;
	return false;
}

void Pheromone::increaseStrength()
{
	int tryPut = 256;
	if (getEnergy() + tryPut > 768)
		tryPut = 768 - getEnergy();
	updateEnergy(tryPut);
}

//TriggerableActor Implementations
TriggerableActor::TriggerableActor(StudentWorld* sw, int x, int y, int imageID)
	:Actor(sw, x, y, right, imageID, 2)
{}

bool TriggerableActor::isDangerous(int colony) const
{	return true;}

//WaterPool Implementations
WaterPool::WaterPool(StudentWorld* sw, int x, int y)
	:TriggerableActor(sw, x, y, IID_WATER_POOL)
{}

void WaterPool::doSomething()
{
	getWorld()->stunAllStunnableAt(getX(), getY());
}

//Poison Implementations
Poison::Poison(StudentWorld* sw, int x, int y)
	:TriggerableActor(sw, x, y, IID_POISON)
{}

void Poison::doSomething()
{
	getWorld()->poisonAllPoisonableAt(getX(), getY());
}

//Insect Class Implementations
Insect::Insect(StudentWorld* world, int startX, int startY, int energy, int imageID)
	:EnergyHolder(world, startX, startY, randDirection(), energy, imageID, 1), sleepTicks(0), wasStunned(false)
{}

void Insect::doSomething()
{
	updateEnergy(-1);
}

void Insect::getBitten(int amt)
{
	updateEnergy(amt);
}

void Insect::getPoisoned()
{
	updateEnergy(-150);
}

void Insect::getStunned()
{
	if (wasStunned == false)
	{
		sleepTicks += 2;
		wasStunned = true;
	}
}
bool Insect::isEnemy(int colony) const
{	return true;}

bool Insect::becomesFoodUponDeath() const
{	return true;}

void Insect::getXYInFrontOfMe(int& x, int& y)
{
	switch (getDirection())
	{
	case up:
	{
		x = getX();
		y = getY() + 1;
		break;
	}
	case right:
	{
		x = getX() + 1;
		y = getY();
		break;
	}
	case down:
	{
		x = getX();
		y = getY() - 1;
		break;
	}
	case left:
	{
		x = getX() - 1;
		y = getY();
		break;
	}
	default:
		break;
	}
}

bool Insect::moveForwardIfPossible()
{
	int tryX = 0, tryY = 0;
	getXYInFrontOfMe(tryX, tryY);
	if (getWorld()->canMoveTo(tryX, tryY))
	{
		moveTo(tryX, tryY);
		resetStunned();
		return true;
	}
	return false;
}

void Insect::updateSleepTicks(int amt)
{
	sleepTicks += amt;
}

bool Insect::isAsleep() const
{
	if (sleepTicks > 0)
		return true;
	return false;
}

GraphObject::Direction Insect::randDirection()
{
	int temp = randInt(1, 4);
	if (temp == 1)
		return up;
	if (temp == 2)
		return right;
	if (temp == 3)
		return down;
	if (temp == 4)
		return left;
	return none;
}

bool Insect::randAttack(int colony, int biteDamage)
{
	if (getWorld()->isEnemyAt(this, getX(), getY(), colony))
		return getWorld()->biteEnemyAt(this, colony, biteDamage);
	return false;
}

//private Insect implementations
void Insect::resetStunned()
{
	wasStunned = false;
}

//Ant implementations
Ant::Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID)
	:Insect(sw, startX, startY, 1500, colony), m_Colony(colony), wasBitten(false), wasBlocked(false), m_AI(program), m_randNum(0), m_ic(0)
{}

void Ant::doSomething()
{
	Insect::doSomething();
	if (isDead())
	{
		killMe();
		return;
	}
	if (isAsleep())
	{
		updateSleepTicks(-1);
		return;
	}
	if (!runAI())
	{
		killMe();
		return;
	}

}
void Ant::getBitten(int amt)
{
	updateEnergy(amt);
	wasBitten = true;
}

bool Ant::isEnemy(int colony) const
{
	if (m_Colony == colony)
		return false;
	return true;
}

bool Ant::moveForwardIfPossible()
{
	if (Insect::moveForwardIfPossible())
	{
		wasBitten = false;
		wasBlocked = false;
		return true;
	}
	wasBlocked = true;
	return false;
}

//private Ant implementations
bool Ant::runAI()
{
	Compiler::Command cmd;

	//counts number of command executed
	int command = 0;

	do {
		// get the command from element ic of the vector
		if (!m_AI->getCommand(m_ic, cmd))
			return false; // error - no such instruction!
		switch (cmd.opcode)
		{
		case Compiler::moveForward:
			// cause the ant to move forward by
			// updating its x,y coordinates
			moveForwardIfPossible();
			++m_ic; // advance to next instruction
			return true;
		case Compiler::eatFood:
			if (getHeldFood() > 0)
			{
				int wantEat = 100;
				if (getHeldFood() < 100)
					wantEat = getHeldFood();
				updateEnergy(wantEat);
				updateHeldFood(-wantEat);
			}
			++m_ic;
			return true;
		case Compiler::dropFood:
			addFood(getHeldFood());
			++m_ic;
			return true;
		case Compiler::bite:
			randAttack(m_Colony, 15);
			++m_ic;
			return true;
		case Compiler::pickupFood:
			//int tryGrab = 400;
			if (getHeldFood() + 400 > 1800)
				pickupFood(1800 - getHeldFood());
			pickupFood(400);
			++m_ic;
			return true;
		case Compiler::emitPheromone:
			emitPheromone();
			++m_ic;
			return true;
		case Compiler::faceRandomDirection:
			setDirection(randDirection());
			++m_ic;
			return true;
		case Compiler::rotateClockwise:
			switch (getDirection())
			{
			case GraphObject::up:
				setDirection(GraphObject::right);
				return true;
			case GraphObject::right:
				setDirection(GraphObject::down);
				return true;
			case GraphObject::down:
				setDirection(GraphObject::left);
				return true;
			case GraphObject::left:
				setDirection(GraphObject::up);
				return true;
			default:
				return true;
			}
			++m_ic;
			return true;
		case Compiler::rotateCounterClockwise:
			switch (getDirection())
			{
			case GraphObject::up:
				setDirection(GraphObject::left);
				return true;
			case GraphObject::right:
				setDirection(GraphObject::up);
				return true;
			case GraphObject::down:
				setDirection(GraphObject::right);
				return true;
			case GraphObject::left:
				setDirection(GraphObject::down);
				return true;
			default:
				return false;
			}
			++m_ic;
			return true;
		case Compiler::generateRandomNumber:
			//i dont know how atoi or .c_str() works but they are needed here
			m_randNum = randInt(0, atoi(cmd.operand1.c_str()));
			++m_ic; // advance to next instruction
			command++;
			break;
		case Compiler::goto_command:
			// just set ic the specified position
			// in operand1
			m_ic = atoi(cmd.operand1.c_str());
			command++;
			break;
		case  Compiler::if_command:
			// if the condition of the if command is is true, then go to the target position
			// in the vector; otherwise fall through to the next position
			switch (atoi(cmd.operand1.c_str()))
			{
			case Compiler::last_random_number_was_zero:
				if (m_randNum == 0)
					m_ic = atoi(cmd.operand2.c_str());
				else
					++m_ic;
				command++;
				break;
			case Compiler::i_am_carrying_food:
				if (getHeldFood() > 0)
					m_ic = atoi(cmd.operand2.c_str());
				else
					++m_ic;
				command++;
				break;
			case Compiler::i_am_hungry:
				if (getEnergy() < 25)
					m_ic = atoi(cmd.operand2.c_str());
				else
					++m_ic;
				command++;
				break;
			case Compiler::i_am_standing_with_an_enemy:
				if (getWorld()->isEnemyAt(this, getX(), getY(), m_Colony))
					m_ic = atoi(cmd.operand2.c_str());
				else
					++m_ic;
				command++;
				break;
			case Compiler::i_am_standing_on_food:
				if (getWorld()->getEdibleAt(getX(), getY()) != nullptr)
					m_ic = atoi(cmd.operand2.c_str());
				else
					++m_ic;
				command++;
				break;
			case Compiler::i_am_standing_on_my_anthill:
				if (getWorld()->isAntHillAt(getX(), getY(), m_Colony))
					m_ic = atoi(cmd.operand2.c_str());
				else
					++m_ic;
				command++;
				break;
			case Compiler::i_smell_pheromone_in_front_of_me:
			{
				int tempX, tempY;
				getXYInFrontOfMe(tempX, tempY);
				if (getWorld()->getPheromoneAt(tempX, tempY, m_Colony) != nullptr)
					m_ic = atoi(cmd.operand2.c_str());
				else
					++m_ic;
				command++;
				break;
			}
			case Compiler::i_smell_danger_in_front_of_me:
			{
				int tempX, tempY;
				getXYInFrontOfMe(tempX, tempY);
				if (getWorld()->isDangerAt(this, tempX, tempY, m_Colony))
					m_ic = atoi(cmd.operand2.c_str());
				else
					++m_ic;
				command++;
				break;
			}
			case Compiler::i_was_bit:
				if (wasBitten)
					m_ic = atoi(cmd.operand2.c_str());
				else
					++m_ic;
				command++;
				break;
			case Compiler::i_was_blocked_from_moving:
				if (wasBlocked)
					m_ic = atoi(cmd.operand2.c_str());
				else
					++m_ic;
				command++;
				break;
			default:
				break;
			}
		default:
			break;
		}
	}
	while (command <= 10);
	return true;
}

void Ant::emitPheromone()
{
	Actor* phmXY;
	phmXY = getWorld()->getPheromoneAt(getX(), getY(), m_Colony);
	if (phmXY == nullptr)
	{
		Actor* temp = new Pheromone(getWorld(), getX(), getY(), m_Colony);
		getWorld()->addActor(temp);
		return;
	}
	if (dynamic_cast<Pheromone*>(phmXY) == nullptr)
		return;
	dynamic_cast<Pheromone&>(*phmXY).increaseStrength();

}



//Grashopper Implementations
Grasshopper::Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID)
	: Insect(sw, startX, startY, energy, imageID)
{
	randDist();
}

int Grasshopper::getDist() const
{
	return m_Dist;
}

void Grasshopper::updateDist(int amt)
{
	m_Dist += amt;
}

void Grasshopper::newPath()
{
	updateDist(randInt(2, 10));
	setDirection(randDirection());
}

bool Grasshopper::tryEat()
{
	if (pickupAndEatFood(200) > 0)
	{
		int temp = randInt(1, 2);
		switch (temp)
		{
		case 1:
		{
			updateSleepTicks(2);
			return true;
		}
		case 2:
		default:
			break;
		}
	}
	return false;
}

void Grasshopper::tryWalk()
{
	if (getDist() == 0)
		newPath();
	if (getDist() > 0)
	{
		if (moveForwardIfPossible())
			updateDist(-1);
		else
			updateDist(-getDist());
	}
}

//private Grasshopper implementations
void Grasshopper::randDist()
{
	m_Dist = randInt(2, 10);
}

//Babygasshopper Implementations
BabyGrasshopper::BabyGrasshopper(StudentWorld* sw, int startX, int startY)
	:Grasshopper(sw, startX, startY, 500, IID_BABY_GRASSHOPPER)
{}

void BabyGrasshopper::doSomething()
{
	Insect::doSomething();

	if (isDead())
	{
 		killMe();
		return;
	}

	if (isAsleep())
	{
		updateSleepTicks(-1);
		return;
	}

	if (getEnergy() >= 1600)
	{
		updateEnergy(-getEnergy());
		Actor* temp = new AdultGrasshopper(getWorld(), getX(), getY());
		getWorld()->addActor(temp);
		killMe();
		return;
	}
	
	if (tryEat())
	{
		return;
	}

	tryWalk();
	updateSleepTicks(2);
}

//Adult Grasshopper Implementations
AdultGrasshopper::AdultGrasshopper(StudentWorld* sw, int startX, int startY)
	:Grasshopper(sw, startX, startY, 1600, IID_ADULT_GRASSHOPPER)
{}

void AdultGrasshopper::doSomething()
{
	Insect::doSomething();

	if (isDead())
	{
		killMe();
		return;
	}

	if (isAsleep())
	{
		updateSleepTicks(-1);
		return;
	}
	if (tryBite())
	{
		updateSleepTicks(2);
		return;
	}
	if (tryJump())
	{
		updateSleepTicks(2);
		return;
	}
	if (tryEat())
		return;
	tryWalk();
	updateSleepTicks(2);
	
}

void AdultGrasshopper::getBitten(int amt)
{
	updateEnergy(amt);
	if (!isDead())
	{
		switch (randInt(1, 2))
		{
		case 1:
			randAttack(-1, 50);
			break;
		case 2:
		default:
			break;
		}
	}

}

void AdultGrasshopper::getPoisoned()
{/*do nothing*/}
void AdultGrasshopper::getStunned()
{/*do nothing*/
}

//private AdultGrasshopper implememntations

bool AdultGrasshopper::tryBite()
{
	switch (randInt(1, 3))
	{
	case 1:
		return randAttack(-1, 50);
	case 2:
	case 3:
	default:
		return false;
	}
}

bool AdultGrasshopper::tryJump()
{
	//this terrible complex loop checks all spaces within 5 squares
	//up down left or right from our origin, and if the sqaure is within
	//our field stores it as a possiblejumpto poisiton.
	if (randInt(1, 10) == 1)
	{

		//this creates a vector of 121 intger vectors. the integer vectors will then
		//store all the valid jumping spots for our grasshopper
		std::vector<std::vector<int>> vec(121);

		int tempX = getX();
		int tempY = getY();


		int	i = 0;

		for (int k = -5; k <= 5; k++)
		{
			if ((tempX + k >= 0) && (tempX + k < 64))
			{
				for (int j = -5; j <= 5; j++)
				{
					if ((tempY + j >= 0) && (tempY + j < 64))
						if (getWorld()->canMoveTo(tempX + k, tempY + j))
						{
							vec[i].push_back(tempX + k);
							vec[i].push_back(tempY + j);
							i++;
						}
				}
			}
		}
		int randJmp = randInt(0, i-1);
		moveTo(vec[randJmp][0], vec[randJmp][1]);
		return true;
	}
	return false;
}