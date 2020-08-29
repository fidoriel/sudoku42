#!/usr/bin/python3

sudoku = [[0,0,8,0,0,4,9,3,5],
          [0,0,0,0,0,5,2,6,0],
          [0,0,0,0,2,3,0,0,8],
          [0,0,0,0,0,0,1,0,7],
          [6,1,0,0,7,0,0,9,0],
          [0,0,0,4,5,0,0,0,0],
          [5,2,0,0,0,0,0,0,3],
          [0,0,3,0,1,0,0,0,0],
          [0,0,0,0,0,0,6,0,1]]

def numberPossible(yP, xP, nr):
    global sudoku

    for i in range(0, 9):
        if sudoku[yP][i] == nr:
            return False

    for i in range(0, 9):
        if sudoku[i][xP] == nr:
            return False
    
    x0 = (xP//3)*3
    y0 = (yP//3)*3

    for i in range(0, 3):
        for j in range(0, 3):
            if sudoku[y0+i][x0+j] == nr:
                return False
    return True

def printSudoku():
    global sudoku
    print()
    for i in range(0, 3):
        print(" ", sudoku[i][0], sudoku[i][1], sudoku[i][2], " | ", sudoku[i][3], sudoku[i][4], sudoku[i][5], " | ", sudoku[i][6], sudoku[i][7], sudoku[i][8], " ")
    print("---------+---------+--------")
    for i in range(3, 6):
        print(" ", sudoku[i][0], sudoku[i][1], sudoku[i][2], " | ", sudoku[i][3], sudoku[i][4], sudoku[i][5], " | ", sudoku[i][6], sudoku[i][7], sudoku[i][8], " ")
    print("---------+---------+--------")
    for i in range(6, 9):
        print(" ", sudoku[i][0], sudoku[i][1], sudoku[i][2], " | ", sudoku[i][3], sudoku[i][4], sudoku[i][5], " | ", sudoku[i][6], sudoku[i][7], sudoku[i][8], " ")
    print()

def solve():
    global sudoku
    for y in range(9):
        for x in range(9):
            if sudoku[y][x] == 0:
                for nr in range(1, 10):
                    if numberPossible(y, x, nr):
                        sudoku[y][x] = nr
                        if solve():
                            return True
                        sudoku[y][x] = 0
                return False
    return True


solve()
printSudoku()