#ifndef _WEAPONRY_H_
#define _WEAPONRY_H_

#include "Sprite.h"
#include "Movable_Sprite.h"
#include "Animal.h"

#include <utility>

// Klassen Weaponry har hand om funktionaliteten för granaten
class Weaponry : public Movable_Sprite
{
public:
  Weaponry(int x_pos, int y_pos);

  void setSpeed(int weapon_user);
  void setAngle(int DIR);
  void increasePower(int inc);
  void activated(bool b);
  bool isActivated();
  void reset();
  
  void move();
  std::pair<int, int> crossHair(int user); 
  int getAngle();
  int getPower();
  void getsThrown(bool b, Sprite& user); 
  bool isThrown();
  void impact(bool b);
  void misses(bool b);
  bool succeeds();
  bool expires();
  
private:
  float angle; 
  unsigned int power;
  bool activate;
  bool is_thrown;
  bool hit;
  bool miss;
};

#endif

