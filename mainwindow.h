#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

using namespace std;

/**
 * \mainpage
 * \par Game of Life
 * Program prawdziwszy niż życie
 * \author Justyna Sałacińska
 * \date 2023.09.12
 * \version 2.0
 * \par Kontakt:
 * \a justyna.salacinska.stud@pw.edu.pl
*/

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class Point {
private:
    int x;
    int y;

public:
    Point(int x, int y);
    int getX();
    void setX(int x);
    int getY();
    void setY(int y);
};

class Cell : public Point {
private:
    bool alive;
    int numOfNeighbors;

public:
    Cell(int x, int y, bool alive);
    bool isAlive();
    void setAlive(bool alive);
    int getNumOfNeighbors();
    void setNumOfNeighbors(int numOfNeighbors);
};

class Board {
private:
    int rows;
    int cols;
    int rowsWithFrame;
    int colsWithFrame;
    Cell*** cells;
    int neumannNeighborCounter(int i, int j);
    int diagonalNeighborCounter(int i, int j);
    int mooreNeighborCounter(int i, int j);
    void countNeighborsForEachCell();

public:
    Board(int rows, int cols);
    void setCellAlive(int i, int j, bool alive);
    bool isCellAlive(int i, int j);
    void putRandomValues();
    void step();
    void toggleCell(int i, int j);
};

class CellPainter {
protected:
    QPainter* painter;
    int cellSide;
    int screenWidth;

public:
    CellPainter(QPainter &painter, int cellSide, int screenWidth);
    virtual void drawAliveCell(int i, int j);
    virtual void drawDeadCell(int i, int j);
};

class BlackAndWhiteSquares : public CellPainter {
public:
    BlackAndWhiteSquares(QPainter &painter, int cellSide, int screenWidth);
    void drawAliveCell(int i, int j) override;
    void drawDeadCell(int i, int j) override;
};

class GaySquares : public CellPainter {
public:
    GaySquares(QPainter &painter, int cellSide, int screenWidth);
    void drawAliveCell(int i, int j) override;
    void drawDeadCell(int i, int j) override;
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void windowStep();

private:
    Ui::MainWindow *ui;
    Board* board;
    int cellSide;
    int rows;
    int cols;
    int screenWidth;
    int screenHeight;
    QTimer* timer;
    QPainter painter;
    CellPainter* cellPainter;
    void keyPressEvent(QKeyEvent *event);
    virtual void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void toggleCell(int squareNumCol, int squareNumRow);
};



#endif // MAINWINDOW_H
