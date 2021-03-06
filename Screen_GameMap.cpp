
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "Screen.h"
#include "EventHandler.h"
#include "Map.h"
#include "Screen_GameMap.h"
#include "Game.h"
#include "Button_Speedup.h"
#include "Button_Speeddown.h"
#include "Wolf.h"
//#include "plotsdl\llist.h"
//#include "plotsdl\plot.h"


using namespace std;
//#include "ScreenManager.h"
class ScreenManager;

SDL_Texture* Screen_GameMap::static_img = NULL;

Screen_GameMap::Screen_GameMap(EventHandler* eventHandler, SDL_Renderer* r, int x, int y, int w, int h, int& currentScreen, Map& map) : Screen(eventHandler, r, w, h), map(map), xpos(x), ypos(y), width(w),
height(h), worldposX(0), worldposY(0) {
	scaleX = width / map.width;
	scaleY = height / map.height;
	button_zoomIn = new Button_ZoomIn(xpos + width / 20, ypos + height / 20, width / 10, height / 10, r, scaleX, scaleY);
	components.push_back(button_zoomIn);
	button_zoomOut = new Button_ZoomOut(xpos + width / 20, ypos + 7 * height / 40 , width / 10, height / 10, r, scaleX, scaleY);
	components.push_back(button_zoomOut);
	showplot = false;
	button_plot = new Button_Plot(xpos + width / 20, ypos + 12 *height / 40, width / 10, height / 10, r, showplot);
	components.push_back(button_plot);

	gameTicks = 0;


	caption_list = NULL;
	strcpy(textCows, "Cows");
	strcpy(textGrass, "Grass");
	strcpy(textWolves, "Wolves");


	caption_list=push_back_caption(caption_list,textCows,0,0xFF0000);
	caption_list=push_back_caption(caption_list,textGrass,1,0x00FF00);
	caption_list=push_back_caption(caption_list,textWolves,2,0x0000FF);

	//caption_list=push_back_caption(caption_list,"Cows",0,0xFF0000);
	//caption_list=push_back_caption(caption_list,"Grass",1,0x00FF00);
	//caption_list=push_back_caption(caption_list,"Wolves",2,0x0000FF);

	coordinate_list = NULL;
	coordinate_list=push_back_coord(coordinate_list, 0, 0,0);
	coordinate_list=push_back_coord(coordinate_list, 1, 0,0);
	coordinate_list=push_back_coord(coordinate_list, 2, 0,0);



	//string path = "opensans.ttf";
	//char *fontpath = new char [path.length()+1];
	//strcpy(fontpath, path.c_str());

	params.screen_xpos= xpos + 20;
	params.screen_ypos= ypos +  9*height/20;
	params.screen_width= 3 * width/4;
	params.screen_heigth= height/2;
	//params.font_text_path = fontpath;
	params.font_text_size=18;
	strcpy(textTime, "Time");
	strcpy(textNumber, "Number");
	params.caption_text_x= textTime;
	params.caption_text_y= textNumber;
	params.caption_list = caption_list;
	params.coordinate_list = coordinate_list;
	params.scale_x = 1;
	params.scale_y = 10;
	params.max_x = 10;
	params.max_y = 100;


	setvbuf (stdout, NULL, _IONBF, 0);

	surface_list = NULL;



	if(TTF_Init() == -1)
	{
		fprintf(stderr, "Error SDL TTF_Init error : %s\n", TTF_GetError());
	}
	if (SDL_Init(SDL_INIT_VIDEO)==-1){

		fprintf(stderr, "Error SDL init failure : %s\n", SDL_GetError());
	}


	plot.font                 = NULL;
	plot.screen               = NULL;
	plot.plot_surface         = NULL;
	plot.plot_mask_surface    = NULL;
	plot.caption_surface      = NULL;
	plot.caption_mask_surface = NULL;
	plot.captionX             = NULL;
	plot.captionY             = NULL;
	plot.textureX             = NULL;
	plot.textureY             = NULL;
	plot.renderer             = NULL;

	//font specs
	plot.font = TTF_OpenFont("OpenSans-Light.ttf", params.font_text_size);
	if(plot.font==NULL)
	{
		printf("Error font file read failure, check your font file\n");
		clean_plot(&plot,&params,&surface_list);

		//return EXIT_FAILURE;
	}

}

void Screen_GameMap::mousePressedUp() {
	for (Component* c : components) {
		c->released();
		if (c->isOver(eventHandler->xMouse, eventHandler->yMouse)) {
			c->clicked();
		}
	}
	for (int i = 0; i < map.width; ++i) {
		for (int j = 0; j < map.height; ++j) {

			//TO ADD: render only elements on display
			if ((i+1)*scaleX - worldposX >= 0 && (j+1)*scaleY - worldposY >= 0 && j*scaleY - worldposY <=  height && i*scaleX - worldposX <=  width) {
				Tile* t = (*map.mapGrid)[i][j];
				if (t->isOver(xpos, ypos, worldposX, worldposY, scaleX, scaleY, eventHandler->xMouse, eventHandler->yMouse))
					t->clicked();
			}

		}
	}
}

void Screen_GameMap::mousePressedDown() {
	mouseInitX = eventHandler->xMouse;
	mouseInitY = eventHandler->yMouse;
	worldInitX = worldposX;
	worldInitY = worldposY;
}

void Screen_GameMap::mouseDown() {
	worldposX = min(max(worldInitX - (eventHandler->xMouse - mouseInitX), -20), map.width * scaleX - width + 20);
	worldposY = min(max(worldInitY - (eventHandler->yMouse - mouseInitY), -20), map.height * scaleY - height + 20);

	for (Component* c : components) {
		if (c->isOver(eventHandler->xMouse, eventHandler->yMouse)) {
			c->pressed();
		}
	}
}

void Screen_GameMap::update() {
	map.updateEntities();
	if (gameTicks % 10 == 0) {
		coordinate_list=push_back_coord(coordinate_list, 0, static_cast<float>(gameTicks/10), static_cast<float>(Cow::getPopulationCount()));
		coordinate_list=push_back_coord(coordinate_list, 1, static_cast<float>(gameTicks/10), static_cast<float>(Grass::getPopulationCount()));
		coordinate_list=push_back_coord(coordinate_list, 2, static_cast<float>(gameTicks/10), static_cast<float>(Wolf::getPopulationCount()));
		params.max_x = static_cast<float>(max(12, gameTicks/10));
		params.max_y = static_cast<float>( max( max(120, max((int)map.animals.size(), (int)map.plants.size())), static_cast<int>(params.max_y)));
		params.scale_x = params.max_x / 12;
		params.scale_y = params.max_y / 12;
	}

	++gameTicks;
	//move
	//Eat / Action
	//Death
	//Reproduce
	//update gamestate
}


void Screen_GameMap::setTextureImg() {
	//cout << "map_water_background.jpg" << endl;
	Screen_GameMap::static_img = IMG_LoadTexture(Game::renderer, "map_water_background.jpg");
}

void Screen_GameMap::render() {
	SDL_Rect rect = { xpos, ypos, width, height };

	//SDL_SetRenderDrawColor(renderer, 0, 200, 255, 205);
	//SDL_RenderFillRect(renderer, &rect);
	SDL_RenderCopy(renderer, Screen_GameMap::static_img, NULL, &rect);

	for (int i = 0; i < map.width; ++i) {
		for (int j = 0; j < map.height; ++j) {

			//TO ADD: render only elements on display
			if ((i+1)*scaleX - worldposX >= 0 && (j+1)*scaleY - worldposY >= 0 && j*scaleY - worldposY <=  height && i*scaleX - worldposX <=  width) {
				(*map.mapGrid)[i][j]->render(xpos, ypos, worldposX, worldposY, scaleX, scaleY, renderer);
			}

		}
	}
	button_zoomIn->render();
	button_zoomOut->render();
	button_plot->render();
	if (showplot) {
			draw_plot(&plot, &params, &surface_list);
	}
}
