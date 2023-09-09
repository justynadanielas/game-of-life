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

// Constructor implementation
Point::Point(int x, int y) : x(x), y(y) {}

// Getter for x
int Point::getX() {
    return x;
}

// Setter for x
void Point::setX(int x) {
    this->x = x;
}

// Getter for y
int Point::getY() {
    return y;
}

// Setter for y
void Point::setY(int y) {
    this->y = y;
}


Cell::Cell(int row_num, int col_num, bool alive) : Point(col_num, row_num), alive(alive) {
    numOfNeighbors = 0;
}

bool Cell::isAlive() {
    return alive;
}

void Cell::setAlive(bool alive) {
    this->alive = alive;
}

int Cell::getNumOfNeighbors() {
    return numOfNeighbors;
}

void Cell::setNumOfNeighbors(int numOfNeighbors) {
    this->numOfNeighbors = numOfNeighbors;
}

// Constructor for the Board class
Board::Board(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    rowsWithFrame = rows + 2;
    colsWithFrame = cols + 2;

    // Create a 2D array of Cell objects
//    std::cout<<"dupa"<<std::endl;
    cells = new Cell**[rowsWithFrame];
    for (int i = 0; i < rowsWithFrame; i++) {
        cells[i] = new Cell*[colsWithFrame];
        for (int j = 0; j < colsWithFrame; j++) {
            cells[i][j] = new Cell(i, j, false);
//            std::cout<<"created cell: x = " << i << " i y = " << j << std::endl;
        }
    }
//    std::cout<<"dupa2"<<std::endl;
}

// Method to set the state of a cell at position (x, y)
void Board::setCellAlive(int i, int j, bool alive) {
    // Check if the given coordinates are within bounds
    if (i >= 1 && i <= rows && j >= 1 && j <= cols) {
        cells[i][j]->setAlive(alive);
//        std::cout<< "Udalo sie ustawic dla x = " << x << " i y = " << y << std::endl;
    } else {
        std::cerr << "Error: Coordinates out of bounds, x = " << i << ", y = " << j << std::endl;
    }
}

bool Board::isCellAlive(int i, int j) {
    return cells[i][j]->isAlive();
}

void Board::putRandomValues() {
    srand(time(0));
    // chcę, żeby tablica miała ramkę z zer, więc zaczynam indeksowanie od 1 i kończę na cols+1 (lub rows+1)
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= cols; j++) {
            setCellAlive(i, j, rand() % 2);
        }
    }
}

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

int Board::mooreNeighborCounter(int i, int j) {
    return neumannNeighborCounter(i, j) + diagonalNeighborCounter(i, j);
}

void Board::countNeighborsForEachCell() {
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= cols; j++) {
            cells[i][j]->setNumOfNeighbors(mooreNeighborCounter(i, j));
        }
    }
}

void Board::step() {
    this->countNeighborsForEachCell();
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= cols; j++) {
//            std::cout<<cells[i][j]->getNumOfNeighbors()<<std::endl;
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

void Board::toggleCell(int i, int j){
    if(cells[i][j]->isAlive()){
        cells[i][j]->setAlive(0);
    }else{
        cells[i][j]->setAlive(1);
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // ustawianie wielkości tablicy na podstawie wielkości ekranu
    QScreen *screen = QGuiApplication::primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();
    cellSide = 20;
//    std::cout<<screenWidth<<" "<<screenHeight<<std::endl;
//    state = get2d(N); //N zadeklarowane w .h
    rows = screenHeight/cellSide;
    cols = screenWidth/cellSide;
    board = new Board(rows, cols);
//    std::cout<<"dupa3"<<std::endl;
    board->putRandomValues();
//    std::cout<<"dupa4"<<std::endl;
//    wypelnij(state, N);
    blackAndWhiteSquaresPainter = new BlackAndWhiteSquares(painter, cellSide); //ten painter nie jest nigdzie wyżej tworzony, bo jest brany z headera
    timer = new QTimer;
    timer->setInterval(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(krok2())); //(odmierza interwały, w których wykonuje się krok)
    //timer->start(); //timer start uruchamia caly program (teraz jest w funkcji toggle start)
//    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(toggleStart()));//pierwszy argument to to co wywołuje funkcję;
                                                         //po przecinku to co się ma dziać z guzikiem;
                                                         //to co ma przyjmować ten sygnał (main window)
                                                         //to jaka funkcja ma się wywołać
    timer->start();
//    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(czyszczenieTablicy()));
//    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(randomowanieTablicy()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/** funkcja tworzy dwuwymiarową dynamiczną tablicę
*/

/** funkcja wypełnia dwuwymiarową tablicę losowo zerami i jedynkami
*/

/** funkcja wypełnia dwuwymiarową tablicę zerami
 *  ta funkcja jest potrzebna, żeby w funkcji umiescWRamce zapełnić zerami ramkę
*/

/** funkcja tworzy tablicę o dwa większą od tablicy danej jako parametr funkcji
 *  potem zapełnia ją zerami
 *  potem w środek tej tablicy wkłada tablicę daną jako paramter funkcji
*/

/** funkcja oblicza, ile dana komórka ma żywych sąsiadów w sąsiedztwie Neumanna
 *  sąsiedztwo Neumanna to cztery komórki, które graniczą z centralną komórką krawędziami
*/

/** funkcja oblicza, ile dana komórka ma żywych sąsiadów w sąsiedztwie Moore'a
 *  sąsiedztwo Moore'a to osiem komórek, które graniczą z centralną komórką krawędziami i wierzchołkami
 *  rozpatruję tylko te, które graniczą wierzchołkami, żeby potem dodać to do funkcji licznikSasiadowNeumann
*/

/** funkcja dodaje tylko wartości obliczone w dwóch poprzednich funkcjach
*/


/** funkcja tworzy dwuwymiarową tablicę sąsiadów
 *  na początku zapełnia ją zerami, żeby domyślnie liczba sąsiadów była zero
 *  następnie w komórki tej tablicy wpisuje liczbę żywych sąsiadów komórki tablicy, która jest dana jako parametr
*/


/** funkcja jako parametr bierze tablicę z zerami i jedynkami
 *  wkłada ją w ramkę z zerami
 *  potem w podwójnej pętli sprawdza liczbę sąsiadów danej komórki i na tej podstawie zamienia ją na zero albo jedynkę
*/


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

// funkcja do gaymode

//void MainWindow::rysowanieKwadracikow(int i, int j){
//    QPainter p(this);
//    if(j>=0 && j<4){
//        p.setBrush(QBrush(Qt::red, Qt::BrushStyle::SolidPattern));
//        p.drawRect(QRect(20+20*i, 20+20*j, 20, 20));
//    }else if(j>=4 && j<8){
//        p.setBrush(QBrush(QColor::fromRgb(255, 165, 0), Qt::BrushStyle::SolidPattern));
//        p.drawRect(QRect(20+20*i, 20+20*j, 20, 20));
//    }else if(j>=8 && j<12){
//        p.setBrush(QBrush(Qt::yellow, Qt::BrushStyle::SolidPattern));
//        p.drawRect(QRect(20+20*i, 20+20*j, 20, 20));
//    }else if(j>=12 && j<16){
//        p.setBrush(QBrush(Qt::green, Qt::BrushStyle::SolidPattern));
//        p.drawRect(QRect(20+20*i, 20+20*j, 20, 20));
//    }else if(j>=16 && j<20){
//        p.setBrush(QBrush(Qt::blue, Qt::BrushStyle::SolidPattern));
//        p.drawRect(QRect(20+20*i, 20+20*j, 20, 20));
//    }else if(j>=20 && j<24){
//        p.setBrush(QBrush(QColor::fromRgb(160, 8, 211), Qt::BrushStyle::SolidPattern));
//        p.drawRect(QRect(20+20*i, 20+20*j, 20, 20));
//    }
//}

void MainWindow::gayMode(int i, int j, QPainter &painter){
//    QPainter painter(this);
    if(j>=0 && j<screenWidth/2/cellSide){
        painter.setBrush(QBrush(Qt::red, Qt::BrushStyle::SolidPattern));
        painter.drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
    }else{
        painter.setBrush(QBrush(Qt::blue, Qt::BrushStyle::SolidPattern));
        painter.drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
    }
}

void MainWindow::paintEvent(QPaintEvent* event){
    painter.begin(this); //painter to nazwa zmiennej; QPainter to klasa
    for(int i=1; i<=rows; i++){
        for(int j=1; j<=cols; j++){
            if(board->isCellAlive(i, j)){
//                painter.setBrush(QBrush(Qt::black, Qt::BrushStyle::SolidPattern));
//                painter.drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
                blackAndWhiteSquaresPainter->drawAliveCell(i, j);
            }else{
                //QPainter painter(this);
//                painter.setBrush(QBrush(Qt::white, Qt::BrushStyle::SolidPattern));
//                painter.drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
                blackAndWhiteSquaresPainter->drawDeadCell(i, j);
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



