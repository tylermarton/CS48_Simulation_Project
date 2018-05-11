#ifndef COMPONENT_H_DEFINED
#define COMPONENT_H_DEFINED


class Component {
public:
  Component(int x, int y, int w, int h, SDL_Renderer* r): xpos(x), ypos(y), width(w), height(h), renderer(r) {
  }
  virtual bool isOver(int mousex, int mousey) {
    return (mousex >= xpos && mousex < xpos + width && mousey >= ypos && mousey < ypos + height);
  }
  virtual void clicked();
  virtual void render();

  int xpos;
  int ypos;
  int width;
  int height;
  SDL_Renderer* renderer;
};
#endif /* COMPONENT_H_DEFINED */
