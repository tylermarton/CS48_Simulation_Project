#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>

#include "EventHandler.h"
//#include "ScreenManager.h"
#include "Component.h"
#include "Screen.h"

using namespace std;

class ScreenManager;
//class EventHandler;

Screen::Screen(EventHandler* eventHandler, SDL_Renderer* r, int w, int h) : width(w), height(h), renderer(r), eventHandler(eventHandler) {
	}

	//~Screen() {
	//  delete eventHandler;
	//  delete sm;
	//delete renderer;
	//}

	 void Screen::mousePressedUp() {
		//cout << "somewhere clicked" << end;
		//cout << "Clicked02" << endl;
		for (Component* c : components) {
			if (c->isOver(eventHandler->xMouse, eventHandler->yMouse)) {
				c->clicked();
			}
		}
	}

	 void Screen::mousePressedDown() {
	}

	 void Screen::mouseDown() {
	}

	 void Screen::update() {
	}

	 void Screen::render() {
		//cout <<  "Problem" << endl;
	}

