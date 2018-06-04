#ifndef WOLF_H_DEFINED
#define WOLF_H_DEFINED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>

#include "Map.h"
#include "Tile.h"
#include "Animal.h"
using namespace std;

class Wolf : public Animal {
public:
	Wolf(Tile* parentTile);

	//Parameters:
	//vector< vector<Tile*> >* nextIterboard: Pointer to 2d vector of Tiles. Cow changes nextIterboard to affect future actions (e.g. moving)
  //Return Values:
  //Precondition: nextIterboard is defined.
  //Postcondition:
	//Sets a random Tile in 3x3 array around center to point to itself, if possible.
	//On interaction with grass, overrides grass.
	//Sets this parentTile to that new Tile.
	//void update(vector< vector<Tile*> >* nextIterboard) override;

	//Parameters:
	// x, y represent the coordinates to render at. w, h represent the width and height of the images.
	// r represents the SDL_Renderer used to render the image in the proper location.
	//Return Values:
	//Precondition: all parameters are defined.
	//Postcondition: Image rendered at x, y with width w and height h.

	void render(int x, int y, int w, int h, SDL_Renderer* r) override;

	void checkMove() override;
	void checkAction() override;
	void checkDeath() override;
	void checkReproduce() override;

};
#endif /* WOLF_H_DEFINED */