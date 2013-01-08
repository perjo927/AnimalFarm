#include "Game.h"

#include "Game_Engine.h"
#include "Static_Sprite.h"
#include "Physics.h"
#include "Map.h"
#include "AI.h"

#include <SDL_ttf.h>

#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <cstdlib>
#include <ctime>

using namespace std;

// Definitioner för ökad läsbarhet vid spelhändelser
#define LEFT 0
#define RIGHT 1
#define UP 1
#define DOWN -1
#define SHEEP 0
#define PIG 1
#define QUIT 1
#define PLAY 0


Game::Game() :
  game_over(false), game_reset(false), quit(false),
  my_turn(true), turn_ctr(0), enemy_turn_ctr(0),
  frame_ctr(0), seconds(60), FPS(40),
  in_menu(true), in_game(false), menu_ctr(1),
  explosions(0), enemy_kills(0), user_kills(0)
{
}

Game::~Game()
{
  keystate = nullptr;
} 

void Game::handleMenu(Time& t)
{  
  if (ev.type == SDL_KEYDOWN)
  {
    switch(ev.key.keysym.sym)
    {
      case SDLK_ESCAPE:
      {
        if(!in_menu)
        {
          // Vi vill pausa spelet om vi öppnar pausmenyn
          t.interrupt(seconds);
        }
        in_menu = true;
        break;
      }
      case SDLK_UP:
      {
        if (in_menu)
        {
          ++menu_ctr %= 2;     
        }
        break;
      }
      case SDLK_DOWN:
      {
        if (in_menu)
        {
          ++menu_ctr %= 2;
        }
        break;
      }
      case SDLK_RETURN:
      {
        if (menu_ctr == PLAY  && in_menu)
        {
          in_menu = !in_menu;
          in_game = true;
          // Sätt igång klockan igenom om vi återvänder till spelet
          t.commence();
          
          if (game_over)
          {
            game_reset = true;
          }
        }
        else if (menu_ctr == QUIT && in_menu)
        {
          quit = QUIT;
        }
        break;
      }
      default:
      {
        break;
      }
    }
  }
}

void Game::handleMovement(Weaponry& grenade, Animal& user)
{
  if (ev.type == SDL_KEYDOWN)
  {
    switch(ev.key.keysym.sym)
    {      
      case SDLK_LEFT:
      {
        // Spelaren ska inte kunna röra under avfyrning
        if (!grenade.isActivated())
        {        
          user.setDir(LEFT);
          user.move();
        }
        break;
      }
      case SDLK_RIGHT:
      {
        if (!grenade.isActivated())
        {
          user.setDir(RIGHT);
          user.move(); 
        }
        break;        
      }    
      default:
      { break; }
    }
  }
  // När vi släpper mellanslagstangenten ska skottet avfyras
  else if (ev.type == SDL_KEYUP && grenade.isActivated())
  {       
    grenade.activated(false);
    grenade.getsThrown(true, user);
    grenade.setSpeed(SHEEP);       
  }
}

void Game::handleTrigger(Weaponry& grenade, Sprite& user)
{
  // Kollar kontinuerlig inmatning
  keystate = SDL_GetKeyState(NULL);
  SDL_PumpEvents();

  if (keystate[SDLK_UP])
  {
    grenade.setAngle(UP);
  } 
  else if (keystate[SDLK_DOWN])
  {
    grenade.setAngle(DOWN);
  }   
  else if (keystate[SDLK_SPACE] && !grenade.isThrown()) 
  {
    grenade.increasePower(1);
    grenade.activated(true);
  }
}

int Game::run()
{
  // Seeda slumpgenerator
  srand ( time(NULL) );

  // Skapa objekt som behövs i spelet
  AI ai;
  Physics physics;
  Game_Engine g;
  Time time;
  Time frames_per_second;
  string time_left = "00:  ";
  ostringstream convert; // för konvertering av heltal till strängar
  string status; // spelarhälsa
  string weapon_settings; // visar vapeninställningar
  int delay; // reglerar FPS
  int DIR; // markerar riktningar

  Map farm;
  farm.load("Maps/map1.map");
  
  // Skapa spelar- och fiendekaraktärer
  Animal* my_user(nullptr);
  Animal* my_partner(nullptr);
  Animal* enemy_user(nullptr);
  Animal* enemy_partner(nullptr);
  // Sätt positioner, och spelartyp
  Animal user1(20, 385, SHEEP);  
  user1.loadGfx();
  user1.setXSpeed(7);
  user1.setYSpeed(4);
  // Randomisera position
  user1.setDir(RIGHT);
  int steps = rand()% 10;
  for (int i=0; i <= steps; ++i)
  {
    user1.move();
  }
  
  Animal user2(260, 515, SHEEP); 
  user2.loadGfx();
  user2.setXSpeed(7);
  user2.setYSpeed(4);
  user2.setDir(LEFT);
  steps = rand()% 10;
  for (int i=0; i <= steps; ++i)
  {
    user2.move();
  }
  
  Animal* users[2] = {&user1, &user2};
  my_user = users[0];
  my_partner= users[1];

  // fiender
  Animal enemy1(620, 445, PIG); // 
  enemy1.loadGfx();
  enemy1.setXSpeed(7);
  enemy1.setYSpeed(-4);
  enemy1.setDir(RIGHT);
  steps = rand()% 7;
  for (int i=0; i <= steps; ++i)
  {
    enemy1.move();
  }


  Animal enemy2(490, 515, PIG); 
  enemy2.loadGfx();
  enemy2.setXSpeed(7);
  enemy2.setYSpeed(-4);
  enemy2.setDir(RIGHT);
  steps = rand()% 10;
  for (int i=0; i <= steps; ++i)
  {
    enemy2.move();
  }

  Animal* enemies[2] = {&enemy1, &enemy2};
  enemy_user = enemies[0];
  enemy_partner= enemies[1];  
  
  // Ladda arsenal
  Weaponry grenade(1,1);
  grenade.loadGfx();
  // Objekt för att rita ut skador på spelplanen
  vector<Static_Sprite*> damage; 
  vector<string> damage_gfx;
  damage_gfx.push_back("GFX/damage.png");
  // Objekt för att rita ut explosioner
  Static_Sprite explosion(0, 0);
  vector<string> explosion_gfx; 
  explosion_gfx.push_back("GFX/explode1.png");
  explosion_gfx.push_back("GFX/explode2.png");
  explosion_gfx.push_back("GFX/explode3.png");
  explosion_gfx.push_back("GFX/explode4.png");  
  explosion.setGfx(explosion_gfx);
  explosion.loadGfx();

  // Dra igång spelmotor
  if (!g.initialize())
  {
    quit = true;
  }

  // Sätt igång klockan
  time.commence();
  
  /// *** HUVUDLOOP FÖR SPELET *** ///
  // Körs till spelaren själv väljer att avsluta
  while (!quit)
  {
    // Bakgrundsgrafik //
    g.background();
    
    /// *************** /// 
    /// **** SPELA **** ///
    /// *************** ///

    /// ** HUVUDMENY || PAUSMENY ** ///
    if (in_menu)
    {
      
      if(game_over)
      {
        if (enemy_kills == 2)
          g.printFont(300, 200, 50, 35, 35, "GAME OVER!");
        else if (user_kills == 2)
          g.printFont(300, 200, 50, 35, 35, "VICTORY!");
      }
      
      g.printFont(300, 260, 50, 35, 35, "Animal Farm");
      if (menu_ctr == PLAY)
      {
        if (in_game)
          g.printFont(350, 340, 20, 30, 25, "RESUME");
        else
          g.printFont(350, 340, 20, 30, 25, "PLAY");
        g.printFont(350, 380, 20, 30, 15, "QUIT");
      }
      else if (menu_ctr == QUIT)
      {
        if (in_game)
          g.printFont(350, 340, 20, 30, 15, "RESUME");
        else
          g.printFont(350, 340, 20, 20, 15, "PLAY");
        g.printFont(350, 380, 20, 20, 25, "QUIT");
      }
    }

    // Nollställ värden om användaren vill börja om
    if (game_reset)
    {
      game_reset = !game_reset;
      game_over = !game_over;
      my_turn = true;
      in_game = true;
      in_menu = !in_menu;
      explosions = 0;
      user_kills = 0;
      enemy_kills = 0;

      my_user = users[0];
      my_partner = users[1];

      time.reset();
      time.commence();

      // Återuppliva spelare och fiender
      for (int i = 0; i < 2; ++i)
      {
        users[i]->revive();
        users[i]->setPos(20, 385);
      }
      user2.setDir(RIGHT);
      steps = (rand()% 10)+10;
      for (int i=0; i <= steps; ++i)
      {
        user2.move();
      }
      
      for (int i = 0; i < 2; ++i)
      {
        enemies[i]->revive();
        enemies[i]->setPos(620, 445);    
      }
      enemy2.setDir(LEFT);
      steps = (rand()% 10)+10;
      for (int i=0; i <= steps; ++i)
      {
        enemy2.move();
      }

      // Ta bort skador
      if (damage.size() > 0)
      {
        for (auto d = damage.begin(); d != damage.end(); ++d) 
        {
          Static_Sprite* temp = *d;
          damage.erase(d);
          delete temp;
          temp = nullptr;         
        }
        damage.clear();
      }      
    }
    
    // ** INNE I SPELET ** //
    else if (!in_menu )
    {
      // Frames-klocka
      frames_per_second.commence();
      // Turn-klocka
      seconds = time.getTime() / 1000 ;
      seconds += time.suspended();
      // SKriv ut klockan
      convert.str(""); // återställ ström
      convert << 60 - seconds; 
      // mata in antal kvarvarande sekunder
      time_left.replace(3, 2, convert.str());
      // skriv ut på skärm
      g.printFont(380, 5, 35, 20, 16, time_left);

      // BYT TUR //
      // - Om tiden löper ut - Om granat träffar - Om granat missar
      if (seconds == 60 || grenade.succeeds() || grenade.expires()) 
      {
        time.reset();
        time.commence();
        
        // VEMS TUR?
        my_turn = !my_turn;
        ++enemy_turn_ctr %= 2;
      
        if (my_turn)
        {
          // Byt vem vi kontrollerar
          // Om vi lever
          if (!my_user->isAlive())
          {
            my_user = my_partner;
          }
          else if (!my_partner->isAlive())
          {
            // passa
          }
          else
          {
            my_partner= users[turn_ctr];      
            my_user = users[++turn_ctr %= 2];
          }
        }
        else if (!my_turn)
        {
          // Byt vem datorn kontrollerar
          if (!enemy_user->isAlive())
          {
            enemy_user = enemy_partner;
          }
          else if (!enemy_partner->isAlive())
          {
            // passa
          }
          else
          {
            enemy_partner= enemies[enemy_turn_ctr];      
            enemy_user = enemies[ ++enemy_turn_ctr %= 2];
          }          
        }

        // Återställ granat
        grenade.impact(false);
        grenade.reset();
      }
    }
    
    // HANTERA  TANGENTBORDSINMATNING
    while (SDL_PollEvent(&ev))
    {
      if (my_turn && !in_menu)
      {
        handleMovement(grenade, *my_user);
      }
      handleMenu(time);
    }

    //// HANTERING AV SPELHÄNDELSER ////
    if (!in_menu)
    {
      if (my_turn)
      {      
        if (seconds < 2)
        {
          g.printFont(g.getW() / 2 - 50,
                      g.getH()/2, 100, 20, 20, "YOUR TURN");
        }

        // VAPENHANTERING
        handleTrigger(grenade, *my_user);
        // Måla sikte
        g.printFont(my_user->getX() + 50 +
                    (grenade.crossHair(SHEEP).first),
                    my_user->getY() + 15 -
                    (grenade.crossHair(SHEEP).second),
                    25, 25, 20, "+"); 
      

        // KOLLISIONSHANTERING
        // FÖr vapen
        if(grenade.isThrown())
        {
          for (int i = 0; i < 2; ++i)
          {
            if (physics.objCollision(grenade, *enemies[i])) // 
            {
              explosion.locate(grenade);            
              grenade.impact(true);
              grenade.reset();
              grenade.getsThrown(false, *my_user);
              enemies[i]->decreaseHealth(34);
              enemies[i]->setDir(RIGHT);   
              enemies[i]->move();
              ++explosions;
              break;
            }         
          }
          if (physics.objCollision(grenade, *my_partner))  
          {
            explosion.locate(grenade);            
            grenade.impact(true);
            grenade.reset();
            grenade.getsThrown(false, *my_user);
            my_partner->decreaseHealth(34);
            my_partner->setDir(RIGHT);  
            my_partner->move();
            ++explosions;            
          }
          else if (physics.mapCollision(grenade, farm))
          {
            // Orsaka ny skada
            Static_Sprite* d = new Static_Sprite(0,0);
            d->setGfx(damage_gfx);
            d->loadGfx();            
            d->locate(grenade);
            damage.push_back(d);

            explosion.locate(grenade);            
            grenade.impact(true);
            grenade.reset();
            grenade.getsThrown(false, *my_user);
            ++explosions;            
          }
          else if (physics.outOfBounds(grenade, farm))
          {
            grenade.reset();
            grenade.getsThrown(false, *my_user);            
          }          
        }

        // Kollisionshantering för spelarkrockar
        // Åerställ kollisionsflagga
        my_user->allow();
        
        if (physics.objCollision(*users[0], *users[1])) // 
        {
          // Returnerar true eller false => 0 eller 1 => LEFT eller RIGHT
          DIR = (my_user->getX() > my_partner->getX());
          my_user->reject(DIR); 
        }
        if (physics.mapCollision(*my_user, farm))
        {
          if (my_user->getDir() == LEFT)
          {
            my_user->reject(RIGHT);
          }
          else
          {
            my_user->reject(LEFT);            
          }
        }        
      }
      // DATORNS TUR //
      else if (!my_turn )
      {
        // Låt datorn "tänka"
        if (seconds < 2)
        {
          g.printFont(g.getW()/2-75, g.getH()/2, 100, 20, 20, "COMPUTER'S TURN");
        }
        else
        {
          if(!grenade.isThrown())
          {
            // Datorn bestämmer hur den vill flytta och skjuta
            ai.estimateAndMove(*enemy_user);
            ai.estimateAndShoot(grenade, *enemy_user);

            // Måla sikte
            g.printFont(enemy_user->getX() + 20 +
                        (grenade.crossHair(PIG).first),
                        enemy_user->getY() + 30 -
                        (grenade.crossHair(PIG).second),
                        25, 25, 20, "+");                    
          }
          // Kollisionshantering, vapen
          else if(grenade.isThrown())
          {
            for (int i = 0; i < 2; ++i)
            {
              if (physics.objCollision(grenade, *users[i])) 
              {
                explosion.locate(grenade);            
                grenade.impact(true);
                grenade.reset();
                grenade.getsThrown(false, *enemy_user);
                users[i]->decreaseHealth(34);
                users[i]->setDir(LEFT);  
                users[i]->move();         
                ++explosions;
                break;
              }         
            }
            if (physics.objCollision(grenade, *enemy_partner))  
            {
              explosion.locate(grenade);            
              grenade.impact(true);
              grenade.reset();
              grenade.getsThrown(false, *enemy_user);
              enemy_partner->decreaseHealth(34);
              enemy_partner->setDir(LEFT);  
              enemy_partner->move();
              ++explosions;            
            }
            else if (physics.mapCollision(grenade, farm))
            {
              Static_Sprite* d = new Static_Sprite(0,0);
              d->setGfx(damage_gfx);
              d->loadGfx();            
              d->locate(grenade);
              damage.push_back(d);
              
              explosion.locate(grenade);            
              grenade.impact(true);
              grenade.reset();
              grenade.getsThrown(false, *enemy_user);
              ++explosions;            
            }
            else if (physics.outOfBounds(grenade, farm))
            {
              grenade.reset();
              grenade.getsThrown(false, *enemy_user);            
            }          
          }
          // Kollision, djur
          enemy_user->allow();

          if (physics.objCollision(*enemies[0], *enemies[1])) 
          {
            DIR = (enemy_user->getX() > enemy_partner->getX());
            my_user->reject(DIR);
          }
          if (physics.mapCollision(*enemy_user, farm))
          {
            if (my_user->getDir() == LEFT)
            {
              enemy_user->reject(RIGHT);
            }
            else
            {
              enemy_user->reject(LEFT);            
            }
          }        
          
        }
      }
    
      // MÅLA SPELARNA
      // och deras status
      // om de lever
      for(int i = 0; i < 2; ++i)
      {
        if (users[i]->isAlive())
        {
          convert.str(""); 
          convert << (i+1);
          status = convert.str();
          status += "   ";
          convert.str(""); 
      
          convert << (users[i]->getHealth());
          status += convert.str();
          status += "%";
      
          int x = users[i]->getX()+15;
          int y = users[i]->getY()-15;

          g.paint(*users[i]);
          g.printFont(x, y, 50, 20, 14, status);
        }
      }
      for(int i = 0; i < 2; ++i)
      {
        if (enemies[i]->isAlive())
        {        
          convert.str("");
          convert << (i+1);
          status = convert.str();
          status += "   ";
          convert.str(""); 
      
          convert << (enemies[i]->getHealth());
          status += convert.str();
          status += "%";
      
          int x = enemies[i]->getX()+15;
          int y = enemies[i]->getY()-15;

          g.paint(*enemies[i]);
          g.printFont(x, y, 50, 20, 14, status);
        }
      }      
      // MÅLA GRANAT
      if (grenade.isThrown())
      {
        grenade.move(); 
        g.paint(grenade);
      }
      else if (grenade.succeeds())
      {
        explosion.move();
        g.paint(explosion);
      }

      if (explosions > 0 && damage.size() > 0)
      {
        for (auto d = damage.begin(); d != damage.end(); ++d) 
        {
          g.paint(**d);
        }
      }

      // Måla vapeninställningar 
      convert.str(""); 
      weapon_settings = "PLAYER: ";
      convert << (turn_ctr+1); //spelarnummer
      weapon_settings +=  convert.str();
      weapon_settings += "   Angle: ";
      convert.str(""); 
      convert << grenade.getAngle();
      weapon_settings += convert.str();
      weapon_settings += "   Power: ";
      convert.str(""); 
      convert << grenade.getPower();
      weapon_settings += convert.str();
      // ****************************
      g.printFont(5, 5, 100, 20, 12, weapon_settings);

      
    }
    
    // VÄND & VÄNTA //
    g.flip();
    
    // Begränsa FPS
    if( frames_per_second.getTime() < 1000 / FPS )
    {
      delay = (1000 / FPS) - frames_per_second.getTime();
      g.delay(delay);
    }

    /// GAME_OVER? ///
    enemy_kills = 0;
    user_kills = 0;
    // Kolla hur många överlevande vi har
    for (int i = 0; i < 2; ++i)
    {
      if (!users[i]->isAlive())
      {
        if (++enemy_kills == 2)
          game_over = true;
      }
      if (!enemies[i]->isAlive())
      {
        if (++user_kills == 2)
          game_over = true;
      }
    }
    if (game_over)
    {
      in_menu = true;
      in_game = false;
    }
    
  } // !QUIT

  // ÅTERSTÄLL PEKARE OCH MINNE
  my_user = nullptr;
  my_partner = nullptr;
  users[0] = nullptr;
  users[1] = nullptr;
  enemies[0] = nullptr;
  enemies[1] = nullptr;

  if (damage.size() > 0)
  {
    for (auto d = damage.begin(); d != damage.end(); ++d) 
    {
      delete *d;
      *d = nullptr;
    }
  }
  damage.clear();

  // AVSLUT
  return 0;
}




