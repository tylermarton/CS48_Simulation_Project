#ifndef ANIMAL_H_DEFINED
#define ANIMAL_H_DEFINED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>
#include <cstdlib>

#include "Map.h"
#include "Tile.h"
#include "Entity.h"
using namespace std;

class Animal : public Entity {
public:
	Animal(Tile* parentTile);
	virtual ~Animal() override;


	//checkMove - moves the animal to an adjacent, open square.  It will move towards food if it's nearby, otherwise it will wander randomly.
	//pre TRUE
	//post The animal is on a square that is in the map bounds and isn't overlapping with another annimal.
	virtual void checkMove();

	virtual void checkAction();


	virtual void checkDeath();

	virtual void checkReproduce();

	int health;
	int hunger;
	int age;
	int ageMax;

	static int getPopulationCount();
private:
	static int populationCount;
};







#endif /* ANIMAL_H_DEFINED */
