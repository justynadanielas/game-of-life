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


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//    int** get2d(int N);
//    void wypelnij(int** tab, int N);
//    void wypelnijZerami(int** tab, int N);
//    int** umiescWRamce(int** tab, int N);
//    int licznikSasiadowNeumann(int** tab, int i, int j);
//    int licznikSasiadowMoore(int** tab, int i, int j);
//    int licznikSasiadow(int** tab, int i, int j);
//    int** tablicaWszystkichSasiadow(int** tab, int N);
//    void run(int** tab, int N);
    void kasuj(int**& tab, int N);
    void kasuj1d(int*& tab);
    Board* board;

    //testowanie
//    void testujGet2d();
//    void testujUmiescWRamce();
//    void testujLicznikSasiadowNeumann();
//    void testujLicznikSasiadowMoore();
//    void testujTablicaWszystkichSasiadow();
//    void testujKrok();
//    void testujWszystko();

    //rysowanie prostokątów
    virtual void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void toggleCell(int ktory_kwadracik_x, int ktory_kwadracik_y);
    //void rysowanieKwadracikow(int i, int j);

public slots:
//    void krok(int** tab, int N);
    void krok2();
//    void toggleStart();
//    void czyszczenieTablicy();
//    void randomowanieTablicy();

private:
    Ui::MainWindow *ui;
    int** state;
//    int N = 30; //NxN to rozmiar tablicy komórek
    int cellSide;
    int rows;
    int cols;
    QTimer* timer;
};



#endif // MAINWINDOW_H
