#ifndef BOARD_H
#define BOARD_H

#include <QPair>
#include <QSet>
#include <QVector>

class Board {
public:
    Board();
    int getCell(int row, int col);
    void setCell(int row, int col, int val);
    QVector<int> allowedVals[9][9];
    QSet<QPair<int, int> > getNeighbours(int, int);
    QString getTTText(int row, int col);
    void resetGrid();
    bool isCellFixed(int, int);

private:
    // this is supposed to be "easy"; i.e. solvable via constraint propagation only...
    int start[9][9] = {{0,0,3,0,2,0,6,0,0},
                      {9,0,0,3,0,5,0,0,1},
                      {0,0,1,8,0,6,4,0,0},
                      {0,0,8,1,0,2,9,0,0},
                      {7,0,0,0,0,0,0,0,8},
                      {0,0,6,7,0,8,2,0,0},
                      {0,0,2,6,0,9,5,0,0},
                      {8,0,0,2,0,3,0,0,9},
                      {0,0,5,0,1,0,3,0,0}};
    int grid[9][9];

};

#endif // BOARD_H
