#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>
#include <cstdlib>

#include "Button_Start.h"
#include "Button.h"
#include "Entity.h"
#include "Grass.h"
#include "Cow.h"
#include "Map.h"


using namespace std;
class ScreenManager;

Button_Start::Button_Start(int x, int y, int w, int h, SDL_Renderer* r, int& currentScreen, Map& map) : Button(x, y, w, h, r), currentScreen(currentScreen), map(map) {
	unclicked_img = IMG_LoadTexture(renderer, "StartButtonRed.png");
	clicked_img = IMG_LoadTexture(renderer, "StartButtonBlue.png");
	current_img = unclicked_img;
}

//~Button_Start() {
//  delete currentScreen;
//}

void Button_Start::clicked() {
	map = Map(20, 20);

	Tile* tile = (*map.mapGrid)[5][5];
	//Square must know Entity
	tile->entity = new Grass(tile);
	tile = (*map.mapGrid)[3][5];
	tile->entity = new Cow(tile);

	//delete map;
	//delete c;

	currentScreen = 1;
}

void Button_Start::pressed() {
	current_img = clicked_img;
}

void Button_Start::released() {
	current_img = unclicked_img;
}

void Button_Start::render() {
	SDL_Rect rect;
	rect.x = xpos;
	rect.y = ypos;
	rect.w = width;
	rect.h = height;
	//SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0x00));
	//SDL_SetRenderDrawColor( renderer, 100, 50, 0, 255 );

	//SDL_Texture *img = NULL;
	//int w, h; // texture width & height
	//img = IMG_LoadTexture(renderer, "StartButtonRed.png");
	//SDL_QueryTexture(current_img, NULL, NULL, &w, &h); // get the width and height of the texture
	SDL_RenderCopy(renderer, current_img, NULL, &rect);
	//SDL_RenderFillRect(renderer, &rect);
}
