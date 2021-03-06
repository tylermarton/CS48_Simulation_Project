#include <vector>

#include "Tile.h"
#include "Map.h"
#include "Game.h"
#include <stdlib.h>
#include "EntityManager.h"

Map::Map(int width, int height) {
	buildMap(width, height);
}

Map::Map() {
	buildMap(0, 0);
}

void Map::buildMap(int width, int height) {

	this->width = width;
	this->height = height;
	animals.clear(); //for 	//move  //Eat / Action //Death //Reproduce  update gamestate
	plants.clear(); //for Reproduce
	toDestroyAnimals.clear();
	toDestroyPlants.clear();

	mapGrid = new vector< vector<Tile*> >(width, vector<Tile*>(height));
	//bufferboard = new vector< vector<Tile*> >(width, vector<Tile*>(height));
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			(*mapGrid)[i][j] = new Tile(i, j, this);
			//(*bufferboard)[i][j] = new Tile(i, j, this);
		}
	}
}


Tile* Map::getTile(int posX, int posY) {

	//return a tile after checking that request in bounds.
	if ((posX >= 0) && (posX < this->width) && (posY >= 0) && (posY < this->height)) {
		return (*mapGrid)[posX][posY];
	}
	else {
		return nullptr;
	}
}

Tile* Map::getRandomTile() {
	int posX = rand() % this->width;
	int posY = rand() % this->height;
	return getTile(posX, posY);
}

Entity* Map::getEntityOfTypeFromTile(int entityType, int layerNum, Tile* tile) {

	if (tile) {
		Entity* tmpEntity = tile->getEntityOfType(entityType, layerNum);
		if (tmpEntity)
			return tmpEntity;
	}

	return nullptr;
}




// Consider writing another function function that returns a vector or list of all entities in range.



Entity* Map::getClosestEntityInRange(int entityType, int layerNum, Tile* centerTile, int maxRadius) {

	int centerX = centerTile->getPosX();
	int centerY = centerTile->getPosY();
	Entity* tmpEntity = nullptr;

	// Check central tile
	tmpEntity = this->getEntityOfTypeFromTile(entityType, layerNum, centerTile);
	if (tmpEntity)
		return tmpEntity;

	// Loop through checking progressively larger squares
	int minX, maxX, minY, maxY;
	for (int currentRadius = 1; currentRadius <= maxRadius; currentRadius++) {
		minX = centerX - currentRadius;
		maxX = centerX + currentRadius;
		minY = centerY - currentRadius;
		maxY = centerY + currentRadius;

		// Search top row
		for (int posX = minX; posX <= maxX; posX++) {
			tmpEntity = this->getEntityOfTypeFromTile(entityType, layerNum, this->getTile(posX, maxY));
			if (tmpEntity)
				return tmpEntity;
		}

		// Search bottom row
		for (int posX = minX; posX <= maxX; posX++) {
			tmpEntity = this->getEntityOfTypeFromTile(entityType, layerNum, this->getTile(posX, minY));
			if (tmpEntity)
				return tmpEntity;
		}

		// Search left column
		for (int posY = minY + 1; posY <= maxY - 1; posY++) {  // + and - 1 to skip searching corners twice.
			tmpEntity = this->getEntityOfTypeFromTile(entityType, layerNum, this->getTile(minX, posY));
			if (tmpEntity)
				return tmpEntity;
		}

		// Search right column
		for (int posY = minY + 1; posY <= maxY - 1; posY++) {  // + and - 1 to skip searching corners twice.
			tmpEntity = this->getEntityOfTypeFromTile(entityType, layerNum, this->getTile(maxX, posY));
			if (tmpEntity)
				return tmpEntity;
		}
	}

	return nullptr;
}


void Map::updateEntities() {

	//Generate a cow at random location on the map if there are less than 3.
	if (Cow::getPopulationCount() < 3) {
		Tile* tmpTile = this->getRandomTile();
		EntityManager::createEntity(EntityID::cow, tmpTile);
	}

	//Generate a grass at random location on the map if there are less than 3 or given a small random chance.
	if (Grass::getPopulationCount() < 3 || (rand() % 30 == 0)) {
		Tile* tmpTile = this->getRandomTile();
		EntityManager::createEntity(EntityID::grass, tmpTile);
	}
	//Generate a cow at random location on the map if there are less than 2.
	if (Wolf::getPopulationCount() < 2) {
		Tile* tmpTile = this->getRandomTile();
		EntityManager::createEntity(EntityID::wolf, tmpTile);
	}

	//if (Wolf::getPopulationCount() < 3) {
	//	Tile* tmpTile = this->getRandomTile();
	//	EntityManager::createEntity(EntityID::wolf, tmpTile);
	//}
	


	for (Animal* a : animals) {
		a->checkMove();
	}
	for (Animal* a : animals) {
		a->checkAction();
	}
	for (Animal* a : animals) {
		a->checkReproduce();
	}
	for (Plant* p : plants) {
		p->checkReproduce();
	}
	for (Animal* a : animals) {
		a->checkDeath();
	}
	for (Plant* p : plants) {
		p->checkDeath();
	}
	for (Animal* a : toDestroyAnimals) {
		animals.erase(a);
		delete a;
	}
	for (Plant* p : toDestroyPlants) {
		plants.erase(p);
		delete p;
	}
	toDestroyAnimals.clear();
	toDestroyPlants.clear();

	/*
		if (counter == 0) {
			for (int i = 0; i < this->width; ++i) {
				for (int j = 0; j < this->height; ++j) {
					(*this->mapGrid)[i][j]->update(this->bufferboard);
				}
			}
			vector< vector<Tile*> >* temp = this->mapGrid;
			this->mapGrid = this->bufferboard;
			this->bufferboard = temp;
			counter = Game::COUNTER;
		}
		--counter;
		//cout << counter << endl;
	*/
}
