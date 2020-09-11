#ifndef sudoku_h
#define sudoku_h

class sudoku
{
public:
    sudoku(int newgrid[9][9]);
    bool solve();
    void printSudokuGrid();
    bool checkIfPossible(int row, int col, int number);
    int getSteps();

    int sudokuGrid[9][9];

private:
    int calculateSectorStart(int rc);
    int backTrackSteps = 0;
};

#endif