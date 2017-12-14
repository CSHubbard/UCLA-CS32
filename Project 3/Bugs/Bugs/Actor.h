#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Compiler;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth);

	// Action to perform each tick.
	virtual void doSomething() = 0;

	// Is this actor dead?
	virtual bool isDead() const;

	// Does this actor block movement?
	virtual bool blocksMovement() const;

	// Cause this actor to be be bitten, suffering an amount of damage.
	virtual void getBitten(int amt);

	// Cause this actor to be be poisoned.
	virtual void getPoisoned();

	// Cause this actor to be be stunned.
	virtual void getStunned();

	// Can this actor be picked up to be eaten?
	virtual bool isEdible() const;

	// Is this actor detected by an ant as a pheromone?
	virtual bool isPheromone(int colony) const;

	// Is this actor an enemy of an ant of the indicated colony?
	virtual bool isEnemy(int colony) const;

	// Is this actor detected as dangerous by an ant of the indicated colony?
	virtual bool isDangerous(int colony) const;

	// Is this actor the anthill of the indicated colony?
	virtual bool isMyAntHill(int colony) const;

	// Get this actor's world.
	StudentWorld* getWorld() const;

private:
	StudentWorld* m_World;
};

class Pebble : public Actor
{
public:
	Pebble(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
	virtual bool blocksMovement() const;
};

class EnergyHolder : public Actor
{
public:
	EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth);
	virtual bool isDead() const;

	//sets im_Dead to true, leaves food if nessecary
	void killMe();

	// Get this actor's amount of energy (for a Pheromone, same as strength).
	int getEnergy() const;

	// Adjust this actor's amount of energy upward or downward.
	void updateEnergy(int amt);

	// Add an amount of food to this actor's location, creates new 
	//food acto is none is pesent
	void addFood(int amt);

	// Have this actor pick up an amount of food.
	int pickupFood(int amt);

	// Have this actor pick up an amount of food and eat it.
	int pickupAndEatFood(int amt);

	// Does this actor become food when it dies?
	virtual bool becomesFoodUponDeath() const;

	// Adjust this actor's amount of held food upward or downward.
	void updateHeldFood(int amt);

	// Get this actor's amount of held food.
	int getHeldFood() const;

private:
	int m_Energy;
	bool im_Dead;
	int m_HeldFood;
};

class Food : public EnergyHolder
{
public:
	Food(StudentWorld* sw, int startX, int startY, int energy);
	virtual void doSomething();
	virtual bool isEdible() const;
};

class AntHill : public EnergyHolder
{
public:
	AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program);
	virtual void doSomething();
	virtual bool isMyAntHill(int colony) const;


private:
	void tryCreateAnt();
	int m_Colony;
	Compiler* m_Program;
};

class Pheromone : public EnergyHolder
{
public:
	Pheromone(StudentWorld* sw, int startX, int startY, int colony);
	virtual void doSomething();
	virtual bool isPheromone(int colony) const;

	// Increase the strength (i.e., energy) of this pheromone.
	void increaseStrength();
private:
	int m_Colony;
};

class TriggerableActor : public Actor
{
public:
	TriggerableActor(StudentWorld* sw, int x, int y, int imageID);
	virtual bool isDangerous(int colony) const;
};

class WaterPool : public TriggerableActor
{
public:
	WaterPool(StudentWorld* sw, int x, int y);
	virtual void doSomething();
};

class Poison : public TriggerableActor
{
public:
	Poison(StudentWorld* sw, int x, int y);
	virtual void doSomething();
};

class Insect : public EnergyHolder
{
public:
	Insect(StudentWorld* world, int startX, int startY, int energy, int imageID);
	virtual void doSomething(); 
	virtual void getBitten(int amt);
	virtual void getPoisoned();
	virtual void getStunned();
	virtual bool isEnemy(int colony) const;
	virtual bool becomesFoodUponDeath() const;

	// Set x,y to the coordinates of the spot one step in front of this insect.
	void getXYInFrontOfMe(int& x, int& y);

	// Move this insect one step forward if possible, and return true;
	// otherwise, return false without moving.
	virtual bool moveForwardIfPossible();

	// Update the number of ticks this insect will sleep by the indicated amount.
	void updateSleepTicks(int amt);

	//checks to see if the Insect is sleeping
	bool isAsleep() const;

	//returns a random direction; was the only way I found to make
	//my initalizer list work for reasons I do not fully understand
	Direction randDirection();

	//attacks a random inect on its square, returns true if successful
	virtual bool randAttack(int colony, int biteDamage);

private:
	int sleepTicks;
	bool wasStunned;

//private functions
	//resets wasStunned variable to false
	void resetStunned();
};

class Ant : public Insect
{
public:
	Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID);
	virtual void doSomething();
	virtual void getBitten(int amt);
	virtual bool isEnemy(int colony) const;
	virtual bool moveForwardIfPossible();

private:
	int m_Colony;
	bool wasBitten;
	bool wasBlocked;
	Compiler* m_AI;
	int m_randNum;
	int m_ic;

	//this is our AI interperter, when run it will execute commands 
	//from the respective ant's .bug file.
	bool runAI();
	void emitPheromone();



};

class Grasshopper : public Insect
{
public:
	Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID);
	//virtual void doSomething();
	int getDist() const;
	void updateDist(int);

	//sets the grasshopper on a new path giving it 
	//a random distance(2,10) and direction
	void newPath();

	//the Gasshopper trys to eat any food on the current square.
	//if it ate there is a 50% chance to sleep. Returns true of the grasshopper
	//decided to rest afer eating, false if it didnt eat or didnt sleep.
	bool tryEat();

	//trys to walk its curernt path, if it has finished its desired distance
	//or was blocked this function creates a new path
	void tryWalk();

private:
	int m_Dist;

	//private functions
	void randDist();
};

class BabyGrasshopper : public Grasshopper
{
public:
	BabyGrasshopper(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
};

class AdultGrasshopper : public Grasshopper
{
public:
	AdultGrasshopper(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
	virtual void getBitten(int amt);
	virtual void getPoisoned();
	virtual void getStunned();

private:
	bool tryBite();
	bool tryJump();
};

#endif // ACTOR_H_

