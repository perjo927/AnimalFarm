#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>

// Klassen Sprite ger möjlighet att rita ut 2D-bilder
// Är basklass och kan inte generera objekt
class Sprite
{
public:
  Sprite(int x_pos, int y_pos);
  Sprite();
  virtual ~Sprite();

  void loadGfx();
  int getW();
  int getH();
  int getX();
  int getY();
  // ger tillgång  egenskaper för bilden
  SDL_Rect* getBody(); 
  SDL_Surface* gfx_id();

  // måste implementeras av subklasser
  virtual void move() = 0; 

protected:
  SDL_Rect body; // Bildens "kropp"
  int gfx_current_id; // Animationsräknare
  std::vector<SDL_Surface*> gfx;
  std::vector<std::string> files; // Filnamn till gfx
  int w, h; // Positioner
  int x, y; // på skärmen

private:

};

#endif 
