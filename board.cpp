#include "board.h"

Board::Board() {
    resetGrid();
}

void Board::resetGrid() {
    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            grid[i][j] = start[i][j];
        }
    }
}

bool Board::isCellFixed(int row, int col) {
    return start[row][col] != 0;
}

int Board::getCell(int row, int col) {
    return grid[row][col];
}

void Board::setCell(int row, int col, int val) {
    grid[row][col] = val;
}

QSet<QPair<int, int> > Board::getNeighbours(int row, int col) {
    //helper function to get a list of row/col pairs holding all neighbouring cells of a cell located @ row/col
    QSet<QPair<int,int>> res;
    for(int i=0; i<9; i++) { //run along the row
        if(i != col)
            res.insert(QPair<int,int>(row,i));
    }
    for(int i=0; i<9; i++) { // run along the column
        if(i != row)
            res.insert(QPair<int,int>(i,col));
    }

    int xsub = row / 3;
    int ysub = col / 3;

    for(int i=3*xsub; i<3*(xsub+1); i++) { //run inside the subgrid
        for(int j=3*ysub; j<3*(ysub+1); j++) {
            if((i != row) && (j != col)) {
                res.insert(QPair<int,int>(i,j));
            }
        }
    }
    return res;
}

QString Board::getTTText(int row, int col) {
    QString res;
    if(isCellFixed(row, col))
        return QString("");
    else {
        for(auto itm:allowedVals[row][col]) {
            res.append(QString::number(itm) + " ");
        }
        if(allowedVals[row][col].isEmpty()) // empty string -> no value allowed in the field
            res.append("No value allowed");
        return res;
    }
}
