//
//  History.cpp
//  CS32_Project1
//
//  Created by Aidan Shahmaram on 1/9/24.
//

#include "History.h"
#include "City.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;


History::History(int nRows, int nCols){
    m_rows = nRows;
    m_cols = nCols;
    for(int i = 0; i < MAXROWS; i++){
        for(int j = 0; j < MAXCOLS; j++){
            grid[i][j] = 0;
        }
    }
}

bool History::record(int r, int c){
    if(r > m_rows || c > m_cols || r < 1 || c < 1)
        return false;
    grid[(r - 1)][(c - 1)]++;
    return true;
}

void History::display() const{
    clearScreen();
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++){
            switch(grid[r][c]){
                case 0:
                    cout << ".";
                    break;
                case 1:
                    cout << "A";
                    break;
                case 2:
                    cout << "B";
                    break;
                case 3:
                    cout << "C";
                    break;
                case 4:
                    cout << "D";
                    break;
                case 5:
                    cout << "E";
                    break;
                case 6:
                    cout << "F";
                    break;
                case 7:
                    cout << "G";
                    break;
                case 8:
                    cout << "H";
                    break;
                case 9:
                    cout << "I";
                    break;
                case 10:
                    cout << "J";
                    break;
                case 11:
                    cout << "K";
                    break;
                case 12:
                    cout << "L";
                    break;
                case 13:
                    cout << "M";
                    break;
                case 14:
                    cout << "N";
                    break;
                case 15:
                    cout << "O";
                    break;
                case 16:
                    cout << "P";
                    break;
                case 17:
                    cout << "Q";
                    break;
                case 18:
                    cout << "R";
                    break;
                case 19:
                    cout << "S";
                    break;
                case 20:
                    cout << "T";
                    break;
                case 21:
                    cout << "U";
                    break;
                case 22:
                    cout << "V";
                    break;
                case 23:
                    cout << "W";
                    break;
                case 24:
                    cout << "X";
                    break;
                case 25:
                    cout << "Y";
                    break;
                default:
                    cout << "Z";
                    break;
            }
        }
        cout << endl;
    }
    cout << endl;
    
}

