#include "Weaponry.h"

#include <cmath>
#include <iostream>

#define UP 1
#define DOWN -1
#define SHEEP 0
#define PIG 1

using namespace std;

Weaponry::Weaponry(int x_pos, int y_pos) :
  Movable_Sprite(x_pos, y_pos),
  angle(0), power(0), activate(false), is_thrown(false),
  hit(false), miss(false)
{
  files.push_back("GFX/green.png");
  files.push_back("GFX/green2.png");
}

// Riktningen på skotten är olika beroende på
// vilket läger som avfyrar dem
void Weaponry::setSpeed(int weapon_user)
{
  if (weapon_user == SHEEP)
  {
    // Formel för estetiskt fin kurva
    xspeed = (power / 3) + 1;
    yspeed = -(angle / 12) + 1;
  }
  else if (weapon_user == PIG)
  {
    // Samma formel fast omvänd
    xspeed = (power / 3) + 1;
    yspeed = -(angle / 12) + 1;
    xspeed = -xspeed;
  }
}

void Weaponry::setAngle(int DIR)
{
  if (angle >= 0 && angle <= 180)
  {
    angle += DIR;
  }
  if (angle == 180)
  {
    angle = 1;
  }
  if (angle == -1)
  {
    angle = 179;
  }
}

void Weaponry::increasePower(int inc)
{
  if (power < 100)
  {
    power += inc;
  }
  if (power == 100)
  {
    power = 0;
  }
}

void Weaponry::activated(bool b)
{
  activate = b;
}

bool Weaponry::isActivated()
{
  return activate;
}

void Weaponry::reset()
{
  angle = 0;
  power = 0;
  x = -10;
  y = -10;
}    
  
void Weaponry::move() 
{
  body.x += xspeed;
  body.y += yspeed;

  // Gravitation
  ++yspeed;
  
  ++gfx_current_id %=2;
}

pair<int, int> Weaponry::crossHair(int user)
{
  float x, y;

  // fastställer hur rörelsen av siktet ska
  // animeras trovärdigt
  x =  cos((3.14*(angle+270)/(180))) * 50;
  y =  sin((3.14*(angle+270)/(180))) * 50;

  // Omvänt för andra sidan
  if (user == PIG)
  {
    x = -x;
  }          
    return make_pair(int(x),int(y));
}

int Weaponry::getAngle()
{
  return angle;
}

int Weaponry::getPower()
{
  return power;
}
void Weaponry::getsThrown(bool b, Sprite& user)
{
  body.x = user.getX() + 30;
  body.y = user.getY() + 15;
  is_thrown = b;
}

bool Weaponry::isThrown()
{
  return is_thrown;
}

void Weaponry::impact(bool b)
{
  hit = b;   
  is_thrown = false;  
}

void Weaponry::misses(bool b)
{
  miss = b;   
  is_thrown = false;  
}

bool Weaponry::succeeds()
{
  return hit;
}

bool Weaponry::expires()
{
  return miss;
}
