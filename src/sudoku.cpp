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
    switch (rc)
    {
    case 0:
    case 1:
    case 2:
        return 0;
        break;
    case 3:
    case 4:
    case 5:
        return 3;
        break;
    case 6:
    case 7:
    case 8:
        return 6;
        break;    
    default:
        break;
    }
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

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (this->sudokuGrid[rowSector+row][colSector+col] == number)
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
            backTrackSteps++;
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

int sudoku::getSteps()
{
    int steps = this->backTrackSteps;
    this->backTrackSteps = 0;
    return steps;
}