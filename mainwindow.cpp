#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QTimer>
#include <cstdlib>
#include <ctime>
#include <QMouseEvent>
#include <QColor>
#include <QScreen>
#include "cellPainter.cpp"
//#include "Point.h"


/** Klasa Point
 *  Przechowuje współrzędne x i y punktu
*/

/** Konstruktor Point
 *  Ma parametry współrzędne x i y
*/
Point::Point(int x, int y) : x(x), y(y) {}

/** Getter dla x
*/
int Point::getX() {
    return x;
}

/** Setter dla x
*/
void Point::setX(int x) {
    this->x = x;
}

/** Getter dla y
*/
int Point::getY() {
    return y;
}

/** Setter dla y
*/
void Point::setY(int y) {
    this->y = y;
}

/** Klasa Cell
 *  Dziedziczy po klasie Point
 *  Definiuje komórkę, podstawowy element gry w życie
*/

/** Konstruktor Cell
 *  Parametry: indeks wiersza, indeks kolumny, informacja o stanie komórki (żywa czy martwa)
 *  indeksowi wiersza odpowiada współrzędna y obiektu typu Point
 *  indeksowi kolumny odpowiada współrzędna x obiektu typu Point
 *  Pola: liczba sąsiadów, przechowujące liczbę żywych sąsiadów danej komórki
*/
// czy dobrze rozumiem rozróżnienie na pola/atrybuty i parametry?
Cell::Cell(int row_num, int col_num, bool alive) : Point(col_num, row_num), alive(alive) {
    numOfNeighbors = 0;
}

/** Getter dla stanu komórki
*/
bool Cell::isAlive() {
    return alive;
}

/** Setter dla stanu komórki
*/
void Cell::setAlive(bool alive) {
    this->alive = alive;
}

/** Getter dla liczby sąsiadów komórki
*/
int Cell::getNumOfNeighbors() {
    return numOfNeighbors;
}

/** Setter dla liczby sąsiadów komórki
*/
void Cell::setNumOfNeighbors(int numOfNeighbors) {
    this->numOfNeighbors = numOfNeighbors;
}

/** Klasa Board
 *  Definiuje planszę, na której toczy się gra w życie
*/

/** Konstruktor Board
 *  Parametry: liczba wierszy, liczba kolumn
 *  Pola: wiersze z ramką; kolumny z ramką;
 *  (ramka umożliwia obliczenie liczby sąsiadów dla komórek na obrzeżach planszy)
 *  "cells" to dwuwymiarowa dynamiczna tablica komórek
*/
Board::Board(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    rowsWithFrame = rows + 2;
    colsWithFrame = cols + 2;

    // ?
    // najpierw powstaje jednowymiarowa tablica wskaźników
    cells = new Cell**[rowsWithFrame];
    for (int i = 0; i < rowsWithFrame; i++) {
        // na kolejnych indeksach tej listy są umieszczane listy wskaźników do komórek
        cells[i] = new Cell*[colsWithFrame];
        for (int j = 0; j < colsWithFrame; j++) {
            // w miejsca na liście wskaźników do komórek wskakują komórki
            cells[i][j] = new Cell(i, j, false);
        }
    }
}

/** Setter dla stanu komórki o danych indeksach na planszy
*/
void Board::setCellAlive(int i, int j, bool alive) {
    // uwaga, indeksowanie zaczyna się od jeden i kończy na rows/cols ze względu na ramkę!
    if (i >= 1 && i <= rows && j >= 1 && j <= cols) {
        // ? o co chodzi z ostrzeżeniem w linijce poniżej?
        cells[i][j]->setAlive(alive);
    } else {
        std::cerr << "Error: Coordinates out of bounds, x = " << i << ", y = " << j << std::endl;
    }
}

/** Getter dla stanu komórki o danych indeksach na planszy
*/
bool Board::isCellAlive(int i, int j) {
    return cells[i][j]->isAlive();
}

/** Metoda zapełniająca planszę losowo żywymi i martwymi komórkami
*/
void Board::putRandomValues() {
    srand(time(0));
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= cols; j++) {
            setCellAlive(i, j, rand() % 2);
        }
    }
}

/** Metoda oblicza, ile dana komórka ma żywych sąsiadów w sąsiedztwie Neumanna
 *  sąsiedztwo Neumanna to cztery komórki, które graniczą z centralną komórką krawędziami
*/
int Board::neumannNeighborCounter(int i, int j) {
    int counter = 0;
    if (cells[i - 1][j]->isAlive()) {
        counter++;
    }
    if (cells[i][j + 1]->isAlive()) {
        counter++;
    }
    if (cells[i + 1][j]->isAlive()) {
        counter++;
    }
    if (cells[i][j - 1]->isAlive()) {
        counter++;
    }
    return counter;
}

/** Metoda oblicza, ile dana komórka ma żywych sąsiadów wśród komórek,
 *  które graniczą z nią wierzchołkami
*/

int Board::diagonalNeighborCounter(int i, int j) {
    int counter = 0;
    if (cells[i - 1][j - 1]->isAlive()) {
        counter++;
    }
    if (cells[i - 1][j + 1]->isAlive()) {
        counter++;
    }
    if (cells[i + 1][j + 1]->isAlive()) {
        counter++;
    }
    if (cells[i + 1][j - 1]->isAlive()) {
        counter++;
    }
    return counter;
}

/** Metoda oblicza, ile dana komórka ma żywych sąsiadów w sąsiedztwie Moore'a
 *  sąsiedztwo Moore'a to osiem komórek, które graniczą z centralną komórką krawędziami i wierzchołkami
*/

int Board::mooreNeighborCounter(int i, int j) {
    return neumannNeighborCounter(i, j) + diagonalNeighborCounter(i, j);
}

/** Metoda przechodzi po tablicy komórek
 *  i dla każdej komórki ustawia jej liczbę sąsiadów
*/

void Board::countNeighborsForEachCell() {
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= cols; j++) {
            cells[i][j]->setNumOfNeighbors(mooreNeighborCounter(i, j));
        }
    }
}

/** Metoda najpierw wywołuje countNeighborsForEachCell
 *  potem na podstawie warunków zdefiniowanych przez Conwaya
 *  ustawia, które komórki będą żywe w następnej turze
*/
void Board::step() {
    this->countNeighborsForEachCell();
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= cols; j++) {
            if (cells[i][j]->getNumOfNeighbors() == 3 && !cells[i][j]->isAlive()) {
                cells[i][j]->setAlive(true);
            }
            else if ((cells[i][j]->getNumOfNeighbors() == 3 || cells[i][j]->getNumOfNeighbors() == 2) && cells[i][j]->isAlive()) {
                cells[i][j]->setAlive(true);
            }
            else {
                cells[i][j]->setAlive(false);
            }
        }
    }
}

/** Metoda do przełączania stanu komórek w czasie trwania gry
*/
void Board::toggleCell(int i, int j){
    if(cells[i][j]->isAlive()){
        cells[i][j]->setAlive(0);
    }else{
        cells[i][j]->setAlive(1);
    }
}


/** Klasa MainWindow
 *  Definiuje co dzieje się w okienku
*/

// ?
/** Konstruktor MainWindow
 *  Dziedziczy po QMainWindow
 *  Parametr to QWidget??
 *  To coś po przecinku to nie wiem
 *  Pola:
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // ustawianie wielkości tablicy na podstawie wielkości ekranu
    QScreen *screen = QGuiApplication::primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();
    // przypisywanie wartości do zmiennej zainicjowanej w headerze
    cellSide = 20;
    // to się przyda! do wykorzystania w klasie cellPainter?
    rows = screenHeight/cellSide;
    cols = screenWidth/cellSide;
    // tworzenie planszy i zapełnianie jej losowo
    board = new Board(rows, cols);
    board->putRandomValues();
    // tworzenie paintera
    cellPainter = new BlackAndWhiteSquares(painter, cellSide, screenWidth);
    // tworzenie timera i ustawianie częstości odświeżania planszy
    timer = new QTimer;
    timer->setInterval(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(krok2())); //(odmierza interwały, w których wykonuje się krok)
    //    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(toggleStart()));//pierwszy argument to to co wywołuje funkcję;
                                                         //po przecinku to co się ma dziać z guzikiem;
                                                         //to co ma przyjmować ten sygnał (main window)
                                                         //to jaka funkcja ma się wywołać
    // timer->start rozpoczyna program
    timer->start();
}

/** Dekonstruktor MainWindow
*/
MainWindow::~MainWindow()
{
    delete ui;
}


/** funkcja bardzo podobna do funkcji krok
 *  pozbawiona parametrów, żeby lepiej działały connecty
 *  state to tablica dwuwymiarowa zadeklarowana w zmiennych globalnych
 *  zapełniona losowo zerami i jedynkami za pomocą funkcji wypełnij
*/

void MainWindow::krok2() {
    board->step();
    update(); //służy do tego, żeby okno rysowało się dwa razy
}

/** funkcja, która służyła do tego, żeby nowy stan tablicy obliczał się ograniczoną liczbę razy
*/


/** funkcja, która kasuje dwuwymiarową tablicę dynamiczną
*/

void MainWindow::kasuj(int**& tab, int N) {
    for (int i = 0; i < N; i++) {
        delete[] tab[i];
    }
    delete[]tab;
    tab = NULL;
}

/** funkcja, która kasuje jednowymiarową tablicę dynamiczną
*/

void MainWindow::kasuj1d(int*& tab) {
    delete[]tab;
    tab = NULL;
}

/** nadpisana funkcja z QWidget
 *  jeśli komórka jest 1, rysuje czarny kwadracik
 *  jeśli komórka jest 0, rysuje biały kwadracik
*/

void MainWindow::paintEvent(QPaintEvent* event){
    painter.begin(this); //painter to nazwa zmiennej; QPainter to klasa
    for(int i=1; i<=rows; i++){
        for(int j=1; j<=cols; j++){
            if(board->isCellAlive(i, j)){
//                painter.setBrush(QBrush(Qt::black, Qt::BrushStyle::SolidPattern));
//                painter.drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
                cellPainter->drawAliveCell(i, j);
//                gayPainter->drawAliveCell(i, j);
            }else{
                //QPainter painter(this);
//                painter.setBrush(QBrush(Qt::white, Qt::BrushStyle::SolidPattern));
//                painter.drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
                cellPainter->drawDeadCell(i, j);
            }
        }
    }
    painter.end();
}

/** funkcja do guzika start
 *  uruchamia się po naciśnięciu guzika start/stop
 *  jeśli program jest w trakcie działania, naciśnięcie guzika zatrzymuje program i zamienia tekst na start
 *  jeśli program akurat nie działa, ustawia interwał na 100 i odpala program, a tekst zamienia na stop
*/

/** funkcja sprawia, że kwadracik zmienia kolor przy kliknięciu
*/


/** nadpisana funkcja z QWidget
*  w zmiennych x i y przechowuje współrzędne kliknięcia
*  potem współrzędne kliknięcia przetłumacza na indeks komórki
*  za pomocą funkcji toggleCell zmienia kolor komórki
*/

void MainWindow::mousePressEvent(QMouseEvent* event){
    int x = event->x();
    int y = event->y();
    //jak ustalić, na którym kwadraciku jest kursor:
    //pozycję kursora należy podzielić na szerokość kwadracika, a potem dodać 1
    //20 to długość boku kwadratu
    int ktory_kwadracik_col = (x/cellSide)+1;
    int ktory_kwadracik_row = (y/cellSide)+1;
    board->toggleCell(ktory_kwadracik_row, ktory_kwadracik_col);
//    std::cout<<"x:"<<ktory_kwadracik_x <<" y:"<<ktory_kwadracik_y<<std::endl;
    update();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        // Close the application when the space key is pressed
        QApplication::quit();
    }
}

/** funkcja czyszcząca tablicę, żeby działał guzik "clear"
*/


/** funkcja zapełniająca tablicę losowo zerami i jedynkami, żeby działał guzik "random"
*/



