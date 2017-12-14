#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Field.h"
#include <string>
#include <vector>

class Actor;
class Compiler;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	virtual ~StudentWorld();

	virtual int init();
	virtual int move();
	virtual void cleanUp();

	// Can an insect move to x,y?
	bool canMoveTo(int x, int y) const;

	// Add an actor to the world 
	void addActor(Actor* a);

	// If an item that can be picked up to be eaten is at x,y, return a
	// pointer to it; otherwise, return a null pointer.  (Edible items are
	// only ever going be food.)
	Actor* getEdibleAt(int x, int y) const;

	// If a pheromone of the indicated colony is at x,y, return a pointer
	// to it; otherwise, return a null pointer.
	Actor* getPheromoneAt(int x, int y, int colony) const;

	// Is an enemy of an ant of the indicated colony at x,y?
	bool isEnemyAt(Actor* me, int x, int y, int colony) const;

	// Is something dangerous to an ant of the indicated colony at x,y?
	bool isDangerAt(Actor* me, int x, int y, int colony) const;

	// Is the anthill of the indicated colony at x,y?
	bool isAntHillAt(int x, int y, int colony) const;

	// Bite an enemy of an ant of the indicated colony at me's location
	// (other than me; insects don't bite themselves).  Return true if an
	// enemy was bitten.
	bool biteEnemyAt(Actor* me, int colony, int biteDamage);

	// Poison all poisonable actors at x,y.
	bool poisonAllPoisonableAt(int x, int y);

	// Stun all stunnable actors at x,y.
	bool stunAllStunnableAt(int x, int y);

	// Record another ant birth for the indicated colony.
	void increaseScore(int colony);

private:
	//a vector of 64 x values holds a vector of 64 y values at each x,
	//and each y vector holds a vector of Actor pointers at that x,y loaction
	std::vector<std::vector<std::vector<Actor*>>> m_field;

	//holds our compiled ants in order.
	std::vector<Compiler*> m_CompiledAnts;

	//keeps track of the winning ant
	int winningColony;

	//holds the score of each ant colony in order of thier compilation
	int scores[4] = {};

	int simTicks; //number of ticks of the simulation

	//private functions

	//determines if any/a new ant is winning
	void checkWinner();

	//compile ants
	bool compileAnts();

	//returns simTicks
	int getCurrentTicks() const;

	//increases simTicks
	void incTicks();

	//Loads the field file
	bool loadField();

	//asks each actor to do something
	void doStuff();

	//moves each actor to its correct position in my StudentWorld
	//field data structure
	void checkPosition(); 

	//deletes dead objects
	void handleDead(); 

	//checsk to see if the game is over
	int checkGameStatus(int ticks);

	//sets gametext at the top of the screen
	void setDisplayText();

};

#endif // STUDENTWORLD_H_