#include "AI.h"

#include <cstdlib>
#include <ctime>

using namespace std;

#define PIG 1
#define UP 1
#define DOWN -1

AI::AI() :
  desired_power(0), desired_angle(0), actual_angle(0),
  desired_direction(0), steps(0), desired_steps(0),
  grenade_choices_made(false), move_choices_made(false)
{
  // Seeda
  srand(time(nullptr));
}


void AI::estimateAndShoot(Weaponry& grenade, Animal& ai)
{
  // Ställ in siktet (en gång)
  if (!grenade_choices_made)
  {
    desired_angle = (rand() % 180) + 1;
    desired_power = (rand() % 60) + 40;    
    grenade_choices_made = true;
  }

  // Kolla var siktet befinner sig
  actual_angle = grenade.getAngle();

  // Fortsätt ställa om siktet tills det är rätt
  if ( actual_angle < desired_angle )
  {
    grenade.setAngle(UP);
  }      
  else if ( actual_angle  > desired_angle   )
  {
    grenade.setAngle(DOWN);
  }
  // Avfyra 
  if (actual_angle == desired_angle)
  {
    if ( grenade.getPower() < desired_power )
    {
      grenade.increasePower(1);
    }   
    else if ( grenade.getPower() == desired_power )
    {
      grenade.getsThrown(true, ai);
      grenade.setSpeed(PIG);
      grenade_choices_made = !grenade_choices_made;
      move_choices_made = !move_choices_made;
    }      
  }
}

void AI::estimateAndMove(Animal& ai)
{
  // Välj ut en lämplig position att röra sig till
  // (en gång per tur)
  if (!move_choices_made)
  {  
    desired_direction = rand() % 2 ; // left/right
    ai.setDir(desired_direction);
    desired_steps = rand() % 5;
    move_choices_made = true;
    steps = 0;
  }
  // Fortsätt tills vi kommit till målet
  if ( steps < desired_steps )
  {
    ++steps;
    ai.move();
  }      
    
}
