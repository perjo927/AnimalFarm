#include "Physics.h"

#include <SDL.h>
#include <SDL_image.h>

#define OBJECT 1

using namespace std;

Physics::Physics()
{}

// Kollision mellan Sprites
bool Physics::objCollision(Sprite& obj1, Sprite& obj2)
{
  SDL_Rect* a = obj1.getBody();
  SDL_Rect* b = obj2.getBody();

  if ((a->x + a->w > b->x && b->x > a->x) || (b->x + b->w > a->x && a->x > b->x))
  {
    if ((a->y + a->h > b->y && b->y > a->y) || (b->y + b->h > a->y && a->y > b->y))
    {
      a = nullptr; b = nullptr;
      return true;
    }
  }
  a = nullptr; b = nullptr;
  return false;
}

// Kollision mellan Sprites och spelplanen
bool Physics::mapCollision(Sprite& obj, Map const& farm)
{
  int x = (obj.getX())+1; // undvik division med 0
  int y = (obj.getY())+1;

  // dividera med 20 för att motsvara map:ens dimensioner
  if (farm.getPos(x/20, y/20) == OBJECT)
  {
    return true;
  }
  return false;
}

// Vapen som avfyras och hamnar utanför spelplanen utan att kollidera
bool Physics::outOfBounds(Sprite& obj, Map const& farm)
{
  if (obj.getX() > farm.getW()*20 || obj.getX() < 0)
  {
    return true;
  }
  if (obj.getY() > farm.getH()*20 || obj.getY() < 0)
  {
    return true;
  }
  
  return false;
}





