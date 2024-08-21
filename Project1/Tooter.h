//
//  Tooter.hpp
//  CS32_Project1
//
//  Created by Aidan Shahmaram on 1/8/24.
//
#ifndef Tooter_h
#define Tooter_h





class City;

class Tooter
{
  public:
        // Constructor
    Tooter(City* cp, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;

        // Mutators
    void move();

  private:
    City* m_city;
    int   m_row;
    int   m_col;
};

#endif /* Tooter_h */
