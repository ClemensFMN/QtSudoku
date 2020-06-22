#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // we setup 81 QLineEdits
    for(int i=0;i<9;i++) {
        for(int j=0;j<9;j++) {
            field[i][j] = new QLineEdit();
            field[i][j]->setText(QString::number(grid[i][j]));
            field[i][j]->setFixedWidth(50);
            QValidator *validator = new QIntValidator(0, 9, this);
            field[i][j]->setValidator(validator);
            field[i][j]->show();
            controlsLayout->addWidget(field[i][j], i, j);
            if(gameBoard.getCell(i,j) != 0) { // if grid contains a fixed value, we
                gameBoard.allowedVals[i][j] << gameBoard.getCell(i,j); // set the allowedVals accordingly
                field[i][j]->setReadOnly(true); // set the lineedit as readonly, and
                field[i][j]->setStyleSheet("QLineEdit { background: rgb(100, 100, 100); }"); // set the color to gray
            }
            // signal editingFinished of each textfield is connected to enterVal; note the use of a lambda here, so that we can provide the row/col to the called function
            connect(field[i][j], &QLineEdit::editingFinished, this, [=]() {this->enterVal(i,j);});
        }
    }
    //we are done with setup and calculate the allowed values for all cells
    updateAllAllowedVals();
    // the allowed values for each cell are displayed as tooltip
    for(int i=0;i<9;i++) {
        for(int j=0;j<9;j++) {
            auto txt = gameBoard.getTTText(i,j); //QVec2String(allowedVals[i][j]);
            field[i][j]->setToolTip(txt);
        }
    }

    QHBoxLayout* all = new QHBoxLayout(this);
    QVBoxLayout *btns = new QVBoxLayout(this);
    QPushButton *resetBtn = new QPushButton("Reset");
    QPushButton *helpBtn = new QPushButton("&Help");

    connect(resetBtn, &QPushButton::clicked, this, [=]() {reset();});
    connect(helpBtn, &QPushButton::pressed, this, [=]() {showHelp();});

    btns->addWidget(resetBtn);
    btns->addWidget(helpBtn);
    all->addLayout(btns);
    all->addLayout(controlsLayout);

    centralWidget->setLayout(all);
    this->setCentralWidget(centralWidget);
}


// called after a new value is entered in a cell
void MainWindow::enterVal(int row, int col) {
    auto newVal = field[row][col]->text().toInt();
    if(newVal != 0) { // 1 - 9 entered
        if(gameBoard.allowedVals[row][col].contains(newVal)) { // which is valid
            gameBoard.setCell(row, col, newVal); // -> update board
        }
        else { // which is invalid
            field[row][col]->setText("0"); // ignore input, but give limited feedback to user (originally, we changed the background (see below), nice would be some kind of callout (TODO)
        }
    //if(newVal != 0) { // 1 - 9 entered
        // we check whether we have entered an allowed value
    //    if(!gameBoard.allowedVals[row][col].contains(newVal)) {
    //        field[row][col]->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); }");
    //    }
    //    else {
    //        field[row][col]->setStyleSheet("QLineEdit { background: rgb(0, 100, 0); }");
    //    }
    }
    else { // 0 entered
        field[row][col]->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); }"); // -> revert back to white (original) background
        // update the grid
        gameBoard.setCell(row, col, newVal);
    }
    // and update allowed values of all cells
    updateAllAllowedVals();
}

// Update the allowed values of all cells
void MainWindow::updateAllAllowedVals() {
    for(int row=0; row < 9; row++) {
        for(int col=0; col < 9; col++) {
            auto ngbs = gameBoard.getNeighbours(row,col); // get the neighbours of the current cell
            QSet<int> otherVals;
            // collect all values from the neighbours
            for(auto ngb : ngbs) {
                auto val = gameBoard.getCell(ngb.first, ngb.second);
                if(val != 0) {
                    otherVals.insert(val);
                }
            }
            QSet<int> allVals;
            allVals << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
            // subtract all
            auto _allowedvals = allVals - otherVals;
            gameBoard.allowedVals[row][col] = _allowedvals.values().toVector();
            auto txt = gameBoard.getTTText(row, col);
            field[row][col]->setToolTip(txt);
        }
    }
}


void MainWindow::showHelp() {
    qDebug() << "Hello";
    /* TODO press "help" button to
        i) briefly light up cells with only one allowed value
        ii) show cells with no allowed values
        */
    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            if((gameBoard.allowedVals[i][j].length() == 1) && (gameBoard.getCell(i,j)==0)) {
                field[i][j]->setStyleSheet("QLineEdit { background: rgb(255, 255, 0); }");
            }
        }
    }
}

void MainWindow::reset() {
    // reset the underlying model and display - not so beautiful :-(
    gameBoard.resetGrid();
    for(int i=0;i<9;i++) {
        for(int j=0;j<9;j++) {
            field[i][j]->setText(QString::number(grid[i][j]));
            if(gameBoard.getCell(i,j) != 0) { // if grid contains a fixed value, we
                gameBoard.allowedVals[i][j] << gameBoard.getCell(i,j); // set the allowedVals accordingly
                field[i][j]->setStyleSheet("QLineEdit { background: rgb(100, 100, 100); }"); // set the color to gray
            }
            else { // 0
                field[i][j]->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); }"); // -> revert back to white (original) background
            }
        }
    }
    updateAllAllowedVals();
}


void MainWindow::setColors() {
    // currently not used
    for(int i=0;i<9;i++) {
        for(int j=0;j<9;j++) {
            if(gameBoard.isCellFixed(i,j)) {
                field[i][j]->setStyleSheet("QLineEdit { background: rgb(100, 100, 100); }"); // set the color to gray
            }
            else { // 0
                field[i][j]->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); }"); // -> revert back to white (original) background
            }
        }
    }

}
