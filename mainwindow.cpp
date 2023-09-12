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


/** Klasa Point
 *  Przechowuje współrzędne x i y punktu
*/

/** Konstruktor Point
 *  Parametry: współrzędne x i y (oba typu int)
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
*/
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
 *  (ramka umożliwia obliczenie liczby sąsiadów dla komórek na obrzeżach planszy)
 *  "cells" to dwuwymiarowa dynamiczna tablica komórek
*/
Board::Board(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    rowsWithFrame = rows + 2;
    colsWithFrame = cols + 2;

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
 *  Metoda zwraca liczbę żywych komórek sąsiadujących z komórką o podanym indeksie
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
 *  Metoda zwraca liczbę żywych komórek sąsiadujących z komórką o podanym indeksie

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
 *  Metoda zwraca liczbę żywych komórek sąsiadujących z komórką o podanym indeksie
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


/** Konstruktor MainWindow
 *  Dziedziczy po QMainWindow
 *  Metoda najpierw ustawia wielkość planszy na podstawie wielkości ekranu;
 *  zapełnia planszę losowymi wartościami;
 *  ustawia paintera na BlackAndWhiteSquares
 *  ustawia timer na 100 ms
 *  timer wywołuje metodę windowStep
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
    connect(timer, SIGNAL(timeout()), this, SLOT(windowStep())); //(odmierza interwały, w których wykonuje się krok)
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


/** Metoda, w której wykonuje się "step"
 *  następnie odświeża się okno aplikacji (metoda "update")
*/

void MainWindow::windowStep() {
    board->step();
    update();
}

/** Metoda służy do rysowania w aplikacji kwadratów reprezentujących komórki
 *  QPainter painter jest deklarowany w headerze
 *  tak samo cellPainter
 *  cellPainter jest zainicjowany jako wskaźnik do obiektu klasy BlackAndWhiteSquares w konstruktorze MainWindow
 *  Później dzięki metodzie keyPressEvent można go zmieniać, by wskazywał na GaySquares
*/

void MainWindow::paintEvent(QPaintEvent* event){
    painter.begin(this);
    for(int i=1; i<=rows; i++){
        for(int j=1; j<=cols; j++){
            if(board->isCellAlive(i, j)){
                cellPainter->drawAliveCell(i, j);
            }else{
                cellPainter->drawDeadCell(i, j);
            }
        }
    }
    painter.end();
}

/** Metoda służy do tego, by kliknięcie zmieniało kolor komórki
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
    int squareNumCol = (x/cellSide)+1;
    int squareNumRow = (y/cellSide)+1;
    board->toggleCell(squareNumRow, squareNumCol);
    update();
}

/** Metoda służy do obsługiwania aplikacji klawiszami spacji, G i S
*  spacja zamyka aplikację
*  klawisz G przełącza aplikację na gayMode
*  klawisz S przełącza aplikację na blackAndWhiteSquares
*/

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        // Close the application when the space key is pressed
        QApplication::quit();
    }
    if (event->key() == Qt::Key_G)
    {
        delete cellPainter;
        cellPainter = new GaySquares(painter, cellSide, screenWidth);
    }
    if (event->key() == Qt::Key_S)
    {
        delete cellPainter;
        cellPainter = new BlackAndWhiteSquares(painter, cellSide, screenWidth);
    }
}
