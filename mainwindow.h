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
 * \date 2021.06.09
 * \version 1.0
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
    // być może powinno też jako parametr przyjmować rozmiar tablicy?
    void putRandomValues();
    void step();
    void toggleCell(int i, int j);

//    void display();
};

class CellPainter {
protected:
    QPainter* painter;
    int cellSide;

public:
    CellPainter(QPainter &painter, int cellSide);
    virtual void drawAliveCell(int i, int j);
    virtual void drawDeadCell(int i, int j);
};

class BlackAndWhiteSquares : public CellPainter {
public:
    BlackAndWhiteSquares(QPainter &painter, int cellSide);
    void drawAliveCell(int i, int j) override;
    void drawDeadCell(int i, int j) override;
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void kasuj(int**& tab, int N);
    void kasuj1d(int*& tab);
    Board* board;

    //rysowanie prostokątów
    virtual void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void toggleCell(int ktory_kwadracik_x, int ktory_kwadracik_y);
    void gayMode(int i, int j, QPainter &painter);

public slots:
//    void krok(int** tab, int N);
    void krok2();
//    void toggleStart();
//    void czyszczenieTablicy();
//    void randomowanieTablicy();

private:
    Ui::MainWindow *ui;
    int** state; // do wykasowania
//    int N = 30; //NxN to rozmiar tablicy komórek
    int cellSide;
    int rows;
    int cols;
    int screenWidth;
    int screenHeight;
    QTimer* timer;
    QPainter painter; //chyba deklaracja razem z inicjacją?? wyjątkowo
    CellPainter* blackAndWhiteSquaresPainter;
    void keyPressEvent(QKeyEvent *event);
};



#endif // MAINWINDOW_H
