
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>
//#include <vector>
#include <cstdlib>

//#include "Map.h"
//#include "Tile.h"
//#include "Entity.h"
#include "Game.h"
#include "Cow.h"
#include "Plant.h"
#include "EntityManager.h"

using namespace std;

int Cow::populationCount = 0;

//PUBLIC FUNCTIONS


SDL_Texture* Cow::static_img = NULL;
SDL_Texture* Cow::static_img_sick = NULL;

Cow::Cow(Tile* parentTile) : Animal(parentTile) {
	id = 3;
	health = 100;
	hunger = 100;
	age = 0;
	Cow::populationCount++;
	//cout << "Cow population: " << Cow::populationCount << endl;
}

Cow::~Cow() {
	Cow::populationCount--;
	//cout << "Cow population: " << Cow::populationCount << endl;
}


void Cow::setTextureImg() {
		//cout << "cow.png" << endl;
	Cow::static_img = IMG_LoadTexture(Game::renderer, "cow.png");
}

void Cow::setTextureImgSick() {
		//cout << "cow_sick.png" << endl;
		Cow::static_img_sick = IMG_LoadTexture(Game::renderer, "cow_sick.png");
}


// checkMove moves the Cow towards adjacent food or in a random direction if no food is directly nearby
void Cow::checkMove() {
	int changeX = 0;
	int changeY = 0;

	int eaterX = parentTile->getPosX();
	int eaterY = parentTile->getPosY();

	// Find the closet adjacent grass square within 1 range.
	Entity* tmpFood = parentTile->map->getClosestEntityInRange(EntityID::grass, 1, parentTile, 1);

	// If food is found within range, calculate the relative change in position necessary to move there
	if (tmpFood) {

		int foodX = tmpFood->getParentTile()->getPosX();
		int foodY = tmpFood->getParentTile()->getPosY();

		if (eaterX < foodX)
			changeX = 1;
		if (eaterX > foodX)
			changeX = -1;

		if (eaterY < foodY)
			changeY = 1;
		if (eaterY > foodY)
			changeY = -1;
	}
	else { // If food is not found nearby, randomly select a direction to move
		changeX = rand() % 3 - 1;
		changeY = rand() % 3 - 1;
	}

	// Calculate the new square position based on the Cow's current square and calculated change in position
	int newX = eaterX + changeX;
	int newY = eaterY + changeY;


	// Move the Cow to the new tile position if that tile is open
	if (parentTile->map->getTile(newX,newY)) {
		vector< vector<Tile*> >* grid = parentTile->map->mapGrid;
		Tile* s = (*grid)[newX][newY];//getting tentative location in iter
		//Square must know Entity
		if (!s->layer2) {
			s->layer2 = this;
			parentTile->layer2 = NULL;
			parentTile = s; // set parent to new tile
		}
	}
}

void Cow::checkAction() {
	++age;
	hunger -= 9;
	Entity* e = parentTile->layer1;
	if (hunger < 75 && e && e->id == 4) {
		((Plant*) parentTile->layer1)->health -= 50;
		hunger = min(100, hunger + 75);
		health = min(100, health + 25);
	}
}

void Cow::checkDeath() {
	if (hunger <= 0) {
		health += hunger;
	}
	if (health <= 0 || age > ageMax) {
		parentTile->layer2 = NULL;
		//cout << "TestDestroy: " << health << " " << age << endl;
		parentTile->map->toDestroyAnimals.insert(this);
		//delete this;
	}
	//if (s->layer1.id = 1)
}

void Cow::checkReproduce() {
	if (age > 10 && hunger > 90 && rand() % 7 == 0) {
		int changeX = rand() % 3 - 1;
		int changeY = rand() % 3 - 1;

		int newX = parentTile->getPosX() + changeX;
		int newY = parentTile->getPosY() + changeY;

		if (parentTile->map->getTile(newX, newY)) {
			vector< vector<Tile*> >* grid = parentTile->map->mapGrid;
			Tile* s = (*grid)[newX][newY];
			//Square must know Entity
			EntityManager::createEntity(EntityID::cow, s);
		}
	}
}

void Cow::render(int x, int y, int w, int h, SDL_Renderer* r) {
	//cout << "GrassRender" << endl;
	//SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
	int le = min(w, w/2 + age);
	int he = min(h, h/2 + age);

	SDL_Rect rect = {x + (w - le)/2, y + (h - he)/2, le, he };
	//SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0x00));

	//SDL_SetRenderDrawColor(r, 100, 50, 0, 255);
	//SDL_SetRenderDrawColor(r, health, max(0, hunger), min(age, 255), 255);
	//SDL_RenderFillRect(r, &rect);
	//SDL_RenderPresent( renderer);
	//SDL_Rect rect = { x, y, w, h };

	if (health <= 95 || age > ageMax - 25) {
		SDL_RenderCopy(r, Cow::static_img_sick, NULL, &rect);
	}
	else {
		SDL_RenderCopy(r, Cow::static_img, NULL, &rect);
	}

}

int Cow::getPopulationCount() {
	return Cow::populationCount;
}


void Cow::modifyHealth(int healthChange) {
	health += healthChange;
	return;
}
