#include "Time.h"

#include <SDL.h>


Time::Time() :
  ticks_since_start(0), paused_time(0), interrupted(false)
{}

void Time::commence()
{
  ticks_since_start = SDL_GetTicks();
}

void Time::interrupt(int s)
{
  paused_time =  s;
}

int Time::suspended()
{
  return paused_time;
}

int Time::getTime()
{
  return SDL_GetTicks() - ticks_since_start;
}

void Time::reset()
{
  paused_time = 0;
}


