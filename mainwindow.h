#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QGridLayout>
#include <QDebug>
#include <QIntValidator>
#include <QPushButton>
#include "board.h"

/*
 * Other ideas:
 * - Make an undo function
 * - load / store of sudokus
 * - store snapshots
*/

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    Board gameBoard;

    int grid[9][9] = {{0,0,3,0,2,0,6,0,0},
                      {9,0,0,3,0,5,0,0,1},
                      {0,0,1,8,0,6,4,0,0},
                      {0,0,8,1,0,2,9,0,0},
                      {7,0,0,0,0,0,0,0,8},
                      {0,0,6,7,0,8,2,0,0},
                      {0,0,2,6,0,9,5,0,0},
                      {8,0,0,2,0,3,0,0,9},
                      {0,0,5,0,1,0,3,0,0}};

    QVector<int> allowedVals[9][9];

    QWidget *centralWidget = new QWidget;
    QLineEdit *field[9][9]; //setup a field of 9x9 textfields
    QGridLayout *controlsLayout = new QGridLayout; // contained in a grid layout

    QSet<QPair<int, int> > getNeighbours(int row, int col);
    QString QVec2String(QVector<int> vec);

    void setColors();
public slots:
    void updateAllAllowedVals();
    void showHelp();
    void enterVal(int row, int col);
    void reset();
private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
