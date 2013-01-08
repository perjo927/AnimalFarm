
#ifndef _ANIMAL_H_
#define _ANIMAL_H_

#include "Movable_Sprite.h"

// Klassen Animal ritar ut spelets karaktärer
// och ger dem vissa egenskaper
class Animal : public Movable_Sprite
{
public:
  Animal(int x_pos, int y_pos, int a_type);

  void setPos(int x, int y);
  
  void allow();
  void reject(int DIR); // kollisionshantering
  void setDir(int DIR);
  int getDir();
  void move();
  void decreaseHealth(int decrease);
  int getHealth();
  bool isAlive();
  void revive();

private:
  int health;
  bool rejection; // för kollisionshantering
  int push_back; // för kollisionshantering
  bool alive;
};

#endif
 
