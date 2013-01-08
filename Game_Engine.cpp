#include "Game_Engine.h"

#include <iostream>

using namespace std;

Game_Engine::Game_Engine() :
  screen(nullptr),  backg(IMG_Load("GFX/bg.png")),
  font(nullptr), text_surface(nullptr),
  text_color({0, 0, 0, 0}),
  SCREEN_WIDTH(800), SCREEN_HEIGHT(600), BPP(32)
{
  backg_body.x = 0; backg_body.y = 0;
}

// Anropas och rensar upp när spelet nått sitt slut
Game_Engine::~Game_Engine()
{
  font = nullptr;
  SDL_FreeSurface(backg);
  SDL_FreeSurface(text_surface); 
  SDL_FreeSurface(screen);
  SDL_Quit();
}

int Game_Engine::getW() const
{
  return SCREEN_WIDTH;
}

int Game_Engine::getH() const
{
  return SCREEN_HEIGHT;
}

// Grafik
bool Game_Engine::initialize()
{
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
  {
    cerr << "Error initializing SDL" << endl;
    return false;
  }
 
  screen =
  SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BPP, SDL_SWSURFACE | SDL_DOUBLEBUF);

  if (screen == nullptr)
  {
    cerr << "Screen Error " << endl;
    return false;
  }

  // Titel
  SDL_WM_SetCaption(
  "Animal Farm: no animal shall kill any other animal without cause", nullptr);

  // Fonter
  if( TTF_Init() == -1 )
  {
    cerr << "TTF Error " << endl;    
    return false;
  } 
  return true;
}

void Game_Engine::background()
{
  SDL_BlitSurface(backg, nullptr, screen, &backg_body);
}

void Game_Engine::paint(Sprite& obj)
{
  SDL_BlitSurface(obj.gfx_id(), nullptr, screen, obj.getBody());
}

void Game_Engine::flip()
{
  SDL_Flip(screen); 
}

void Game_Engine::delay(int delay)
{
  SDL_Delay(delay);                         
}

void Game_Engine::printFont(int x, int y, int w, int h, int px, string s) 
{  
  font = TTF_OpenFont("Fonts/FreeSansBold.otf", px); 
  text_surface = TTF_RenderText_Blended(font, s.c_str(), text_color); 
  text_rect = { x, y, w, h };
  SDL_BlitSurface(text_surface, nullptr, screen, &text_rect);
  TTF_CloseFont(font);
}


