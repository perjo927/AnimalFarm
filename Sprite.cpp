#include "Sprite.h"

using namespace std;

Sprite::Sprite(int x_pos, int y_pos) :  gfx_current_id(0)
{
  body.x = x_pos; body.y = y_pos;
}

Sprite::Sprite() : gfx_current_id(0)
{
  body.x = 0; body.y = 0;
}

// Rensa minne
Sprite::~Sprite()
{
  for (auto g = gfx.begin(); g != gfx.end(); ++g) 
  {
    SDL_FreeSurface(*g); 
  }
}

void Sprite::loadGfx()
{
  for (auto f = files.begin(); f != files.end(); ++f)
  {
    gfx.push_back( IMG_Load(f->c_str()) );
  }

  body.w = gfx.at(0)->w;
  body.h = gfx[0]->h;
}

int Sprite::getW()
{
  return body.w;
}

int Sprite::getH()
{
  return body.h;
}

int Sprite::getX()
{
  return body.x;
}

int Sprite::getY()
{
  return body.y;
}

SDL_Rect* Sprite::getBody()
{
  return &body;
}

// Hjälper animering
SDL_Surface* Sprite::gfx_id()
{
  return gfx.at(gfx_current_id);
}



