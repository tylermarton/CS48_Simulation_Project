#ifndef BUTTON_ZOOMIN_H_DEFINED
#define BUTTON_ZOOMIN_H_DEFINED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <cstdlib>
#include "Button.h"

using namespace std;

class Button_ZoomIn : public Button {
public:
	Button_ZoomIn(int x, int y, int w, int h, SDL_Renderer* r, int & scaleX, int & scaleY);

	//Parameters:
	//Return Values:
	//Precondition: Mouse is over button border and is released.
	//Postcondition: Increase values of map scales.
	void clicked() override;

	//Parameters:
	//Return Values:
	//Precondition:
	//Postcondition: Renders images of zoom out button.
	//void render() override;

	int &scx;
	int &scy;
};

#endif /* BUTTON_ZOOMIN_H_DEFINED */
