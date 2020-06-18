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
            if(grid[i][j] != 0) { // if grid contains a fixed value, we
                allowedVals[i][j] << grid[i][j]; // set the allowedVals accordingly,
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
            auto txt = QVec2String(allowedVals[i][j]);
            field[i][j]->setToolTip(txt);
        }
    }
    centralWidget->setLayout(controlsLayout);
    this->setCentralWidget(centralWidget);
}

//helper function to get a list of row/col pairs holding all neighbouring cells of a cell located @ row/col
QSet<QPair<int,int>> MainWindow::getNeighbours(int row, int col) {
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

// convert a vec to a string
QString MainWindow::QVec2String(QVector<int> vec) {
    QString res;
    for(auto itm:vec) {
        res.append(QString::number(itm) + " ");
    }
    if(vec.isEmpty()) // empty string -> no vlaue allowed in the field
        res.append("No value allowed");
    return res;
}

// called after a new value is entered in a cell
void MainWindow::enterVal(int row, int col) {
    auto newVal = field[row][col]->text().toInt();
    // we check whether we have entered an allowed value
    if(!allowedVals[row][col].contains(newVal)) {
        field[row][col]->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); }");
    }
    else {
        field[row][col]->setStyleSheet("QLineEdit { background: rgb(0, 100, 0); }");
    }
    // update the grid
    grid[row][col] = newVal;
    // and update allowed values of all cells
    updateAllAllowedVals();
}

// Update the allowed values of all cells
void MainWindow::updateAllAllowedVals() {
    for(int row=0; row < 9; row++) {
        for(int col=0; col < 9; col++) {
            auto ngbs = getNeighbours(row,col); // get the neighbours of the current cell
            QSet<int> otherVals;
            // collect all values from the neighbours
            for(auto ngb : ngbs) {
                auto val = grid[ngb.first][ngb.second];
                if(val != 0) {
                    otherVals.insert(val);
                }
            }
            QSet<int> allVals;
            allVals << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
            // subtract all
            auto _allowedvals = allVals - otherVals;
            allowedVals[row][col] = _allowedvals.values().toVector();
            auto txt = QVec2String(allowedVals[row][col]);
            field[row][col]->setToolTip(txt);
        }
    }
}




void MainWindow::mySlot()
{
    qDebug() << "Hello";
}
