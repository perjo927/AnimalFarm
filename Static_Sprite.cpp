#include "Static_Sprite.h"

using namespace std;

Static_Sprite::Static_Sprite(int x_pos, int y_pos) :
  Sprite(x_pos, y_pos)
{}

// move innebär animation i det här fallet
void Static_Sprite::move()
{
  if (gfx_current_id < 3) // files.end .. iterator
  {
    gfx_current_id++; 
  }
  else
  {
    gfx_current_id = 0;
  }
}

// Lokaliserar ett givet Sprite-objekt och tar dess position
// T.ex. för att lokalisera var en explosion ska äga rum
void Static_Sprite::locate(Sprite& obj)
{
  body.x = obj.getX();
  body.y = obj.getY();
}

// Ger möjlighet att välja hur Spriten ska se ut
void Static_Sprite::setGfx(std::vector<std::string> new_files)
{
  files = new_files;
}
