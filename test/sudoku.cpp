#include "sudoku.h"
#include <iostream>

sudoku::sudoku(int newgrid[9][9])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            this->sudokuGrid[j][i] = newgrid[j][i];
        }
    }
}

void sudoku::printSudokuGrid()
{
    std::cout << std::endl;
    for (int y = 0; y < 3; y++)
    {
        std::cout << " " << this->sudokuGrid[y][0] << " " << this->sudokuGrid[y][1] << " " << this->sudokuGrid[y][2] << " | " << this->sudokuGrid[y][3] << " " << this->sudokuGrid[y][4] << " " << this->sudokuGrid[y][5] << " | " << this->sudokuGrid[y][6] << " " << this->sudokuGrid[y][7] << " " << this->sudokuGrid[y][8] << " " << std::endl;
    }
    std::cout << " ------+-------+------ " << std::endl;
    for (int y = 3; y < 6; y++)
    {
        std::cout << " " << this->sudokuGrid[y][0] << " " << this->sudokuGrid[y][1] << " " << this->sudokuGrid[y][2] << " | " << this->sudokuGrid[y][3] << " " << this->sudokuGrid[y][4] << " " << this->sudokuGrid[y][5] << " | " << this->sudokuGrid[y][6] << " " << this->sudokuGrid[y][7] << " " << this->sudokuGrid[y][8] << " " << std::endl;
    }
    std::cout << " ------+-------+------ " << std::endl;
    for (int y = 6; y < 9; y++)
    {
        std::cout << " " << this->sudokuGrid[y][0] << " " << this->sudokuGrid[y][1] << " " << this->sudokuGrid[y][2] << " | " << this->sudokuGrid[y][3] << " " << this->sudokuGrid[y][4] << " " << this->sudokuGrid[y][5] << " | " << this->sudokuGrid[y][6] << " " << this->sudokuGrid[y][7] << " " << this->sudokuGrid[y][8] << " " << std::endl;
    }
    std::cout << std::endl;
}

int sudoku::calculateSectorStart(int rc)
{
    if(rc >= 0 && rc < 3)
    {
        return 0;
    }
    if(rc >= 3 && rc < 6)
    {
        return 3;
    }
    if(rc >= 6 && rc < 9)
    {
        return 6;
    }
    return 0;
}

bool sudoku::checkIfPossible(int row, int col, int number)
{
    for (int i = 0; i < 9; i++)
    {
        if (this->sudokuGrid[row][i] == number) 
        {
            return false;
        }  
    }
    for (int i = 0; i < 9; i++)
    {
        if (this->sudokuGrid[i][col] == number)
        {
            return false;
        }
    }

    int rowSector = this->calculateSectorStart(row);
    int colSector = this->calculateSectorStart(col);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (this->sudokuGrid[rowSector+i][colSector+i] == number)
            {
                return false;
            }
            
        }
        
    }
    return true;
}

bool sudoku::solve()
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (this->sudokuGrid[row][col] == 0)
            {
                for (int number = 1; number < 10; number++)
                {
                    if (this->checkIfPossible(row, col, number))
                    {
                        this->sudokuGrid[row][col] = number;
                        if(this->solve())
                        {
                            return true;
                        }
                        this->sudokuGrid[row][col] = 0;
                    }
                }
                return false;              
            }
        }
    }
    return true;
}

/*
int templateG[9][9] = {{5,0,1,0,4,0,7,0,6},
                       {0,7,2,6,1,0,4,0,9},
                       {4,6,0,2,0,7,0,8,0},
                       {0,4,0,0,9,1,0,7,0},
                       {1,8,7,4,0,2,3,0,5},
                       {0,0,5,0,8,0,0,1,4},
                       {6,1,0,3,0,8,9,0,0},
                       {0,0,8,9,2,0,0,4,3},
                       {2,9,0,0,5,0,8,0,7}};

sudoku su(templateG);

int main(){
    su.solve();
    su.printSudokuGrid();
}

*/