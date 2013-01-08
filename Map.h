#ifndef _MAP_H_
#define _MAP_H_

#include <string>

// Klassen Map läser in en teckenkodad karta
// och hjälper fysik-klassen med
// kollisionshantering
class Map
{	
public:
  Map();
  int getPos(int w, int h) const;
  int getW() const;
  int getH() const;
  bool load(std::string map_filename);
  // void draw();

private:
  static const int WIDTH = 40;
  static const int HEIGHT = 30;
  int farm_map[HEIGHT][WIDTH];  
};

#endif
