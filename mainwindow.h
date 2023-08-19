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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int** get2d(int N);
    void wypelnij(int** tab, int N);
    void wypelnijZerami(int** tab, int N);
    int** umiescWRamce(int** tab, int N);
    int licznikSasiadowNeumann(int** tab, int i, int j);
    int licznikSasiadowMoore(int** tab, int i, int j);
    int licznikSasiadow(int** tab, int i, int j);
    int** tablicaWszystkichSasiadow(int** tab, int N);
    void run(int** tab, int N);
    void kasuj(int**& tab, int N);
    void kasuj1d(int*& tab);

    //testowanie
    void testujGet2d();
    void testujUmiescWRamce();
    void testujLicznikSasiadowNeumann();
    void testujLicznikSasiadowMoore();
    void testujTablicaWszystkichSasiadow();
    void testujKrok();
    void testujWszystko();

    //rysowanie prostokątów
    virtual void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void toggleCell(int ktory_kwadracik_x, int ktory_kwadracik_y);
    //void rysowanieKwadracikow(int i, int j);

public slots:
    void krok(int** tab, int N);
    void krok2();
    void toggleStart();
    void czyszczenieTablicy();
    void randomowanieTablicy();

private:
    Ui::MainWindow *ui;
    int** state;
    int N = 24; //NxN to rozmiar tablicy komórek
    QTimer* timer;
};

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

public:
    Cell(int x, int y, bool alive);
    bool isAlive();
    void setAlive(bool alive);
};

//class Board {
//private:
//    static const int rows = 10;
//    static const int cols = 10;
//    Cell cells[rows][cols];

//public:
//    Board();
//    void setCellAlive(int x, int y, bool alive);
//    void display();
//};



#endif // MAINWINDOW_H
