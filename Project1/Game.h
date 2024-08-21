//
//  Game.hpp
//  CS32_Project1
//
//  Created by Aidan Shahmaram on 1/8/24.
//

#ifndef Game_h
#define Game_h




class Player;

class City;

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nTooters);
    ~Game();

        // Mutators
    void play();

  private:
    City* m_city;
};


#endif /* Game_h */
