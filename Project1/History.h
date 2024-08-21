//
//  History.hpp
//  CS32_Project1
//
//  Created by Aidan Shahmaram on 1/9/24.
//

#ifndef History_hpp
#define History_hpp
#include "globals.h"



class City;

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
    
private:
    int m_rows;
    int m_cols;
    int grid[MAXROWS][MAXCOLS];
    City* m_city;
    };

#endif /* History_hpp */
