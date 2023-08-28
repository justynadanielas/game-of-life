#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QTimer>
#include <cstdlib>
#include <ctime>
#include <QMouseEvent>
#include <QColor>
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


Cell::Cell(int x, int y, bool alive) : Point(x, y), alive(alive) {
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
Board::Board(int size) {
    rows = size;
    cols = size;
    rowsWithFrame = rows + 2;
    colsWithFrame = cols + 2;

    // Create a 2D array of Cell objects
    std::cout<<"dupa"<<std::endl;
    cells = new Cell**[rowsWithFrame];
    for (int i = 0; i < rows; i++) {
        cells[i] = new Cell*[colsWithFrame];
        for (int j = 0; j < cols; j++) {
            cells[i][j] = new Cell(i, j, false);
        }
    }
    std::cout<<"dupa2"<<std::endl;
}

// Method to set the state of a cell at position (x, y)
void Board::setCellAlive(int x, int y, bool alive) {
    // Check if the given coordinates are within bounds
    if (x >= 0 && x < rows && y >= 0 && y < cols) {
        cells[x][y]->setAlive(alive);
    } else {
        std::cerr << "Error: Coordinates out of bounds." << std::endl;
    }
}

bool Board::isCellAlive(int x, int y) {
    return cells[x][y]->isAlive();
}

void Board::putRandomValues() {
    srand(time(0));
    // chcę, żeby tablica miała ramkę z zer, więc zaczynam indeksowanie od 1 i kończę na cols+1 (lub rows+1)
    for (int i = 1; i <= cols; i++) {
        for (int j = 1; j <= rows; j++) {
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
    for (int i = 1; i <= cols; i++) {
        for (int j = 1; j <= rows; j++) {
            cells[i][j]->setNumOfNeighbors(mooreNeighborCounter(i, j));
        }
    }
}

void Board::step() {
    for (int i = 1; i <= cols; i++) {
        for (int j = 1; j <= rows; j++) {
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
//    update(); ta funkcja nie dziala, bo jest z MainWindow
    // do naprawy
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    state = get2d(N); //N zadeklarowane w .h
    board = new Board(N);
    board->putRandomValues();
    // tu zamiast wypełnij będzie metoda klasy Board
//    wypelnij(state, N);
    timer = new QTimer;
    timer->setInterval(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(krok2())); //(odmierza interwały, w których wykonuje się krok)
    //timer->start(); //timer start uruchamia caly program (teraz jest w funkcji toggle start)
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(toggleStart()));//pierwszy argument to to co wywołuje funkcję;
                                                         //po przecinku to co się ma dziać z guzikiem;
                                                         //to co ma przyjmować ten sygnał (main window)
                                                         //to jaka funkcja ma się wywołać
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(czyszczenieTablicy()));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(randomowanieTablicy()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/** funkcja tworzy dwuwymiarową dynamiczną tablicę
*/

int** MainWindow::get2d(int N) {
    int** tab = new int* [N];
    for (int i = 0; i < N; i++) {
        tab[i] = new int[N];
    }
    return tab;
}

/** funkcja wypełnia dwuwymiarową tablicę losowo zerami i jedynkami
*/

void MainWindow::wypelnij(int** tab, int N) {
    srand(time(0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tab[i][j] = rand() % (2);
        }
    }
}

/** funkcja wypełnia dwuwymiarową tablicę zerami
 *  ta funkcja jest potrzebna, żeby w funkcji umiescWRamce zapełnić zerami ramkę
*/

void MainWindow::wypelnijZerami(int** tab, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tab[i][j] = 0;
        }
    }
}

/** funkcja tworzy tablicę o dwa większą od tablicy danej jako parametr funkcji
 *  potem zapełnia ją zerami
 *  potem w środek tej tablicy wkłada tablicę daną jako paramter funkcji
*/

int** MainWindow::umiescWRamce(int** tab, int N) {
    int** ramka = get2d(N + 2);
    wypelnijZerami(ramka, N + 2);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            ramka[i + 1][j + 1] = tab[i][j];
        }
    }
    return ramka;
}

/** funkcja oblicza, ile dana komórka ma żywych sąsiadów w sąsiedztwie Neumanna
 *  sąsiedztwo Neumanna to cztery komórki, które graniczą z centralną komórką krawędziami
*/

int MainWindow::licznikSasiadowNeumann(int** tab, int i, int j) {
    int licznik = 0;
    if (tab[i - 1][j] == 1) {
        licznik++;
    }
    if (tab[i][j + 1] == 1) {
        licznik++;
    }
    if (tab[i + 1][j] == 1) {
        licznik++;
    }
    if (tab[i][j - 1] == 1) {
        licznik++;
    }
    return licznik;
}

/** funkcja oblicza, ile dana komórka ma żywych sąsiadów w sąsiedztwie Moore'a
 *  sąsiedztwo Moore'a to osiem komórek, które graniczą z centralną komórką krawędziami i wierzchołkami
 *  rozpatruję tylko te, które graniczą wierzchołkami, żeby potem dodać to do funkcji licznikSasiadowNeumann
*/

int MainWindow::licznikSasiadowMoore(int** tab, int i, int j) {
    int licznik = 0;
    if (tab[i - 1][j - 1] == 1) {
        licznik++;
    }
    if (tab[i - 1][j + 1] == 1) {
        licznik++;
    }
    if (tab[i + 1][j + 1] == 1) {
        licznik++;
    }
    if (tab[i + 1][j - 1] == 1) {
        licznik++;
    }
    return licznik;
}

/** funkcja dodaje tylko wartości obliczone w dwóch poprzednich funkcjach
*/

int MainWindow::licznikSasiadow(int** tab, int i, int j) {
    int licznik = 0;
    licznik = licznikSasiadowMoore(tab, i, j) + licznikSasiadowNeumann(tab, i, j);
    return licznik;
}

/** funkcja tworzy dwuwymiarową tablicę sąsiadów
 *  na początku zapełnia ją zerami, żeby domyślnie liczba sąsiadów była zero
 *  następnie w komórki tej tablicy wpisuje liczbę żywych sąsiadów komórki tablicy, która jest dana jako parametr
*/

int** MainWindow::tablicaWszystkichSasiadow(int** tab, int N) {
    int** tabSasiadow = get2d(N);
    wypelnijZerami(tabSasiadow, N);
    for (int i = 1; i < N + 1; i++) {
        for (int j = 1; j < N + 1; j++) {
            tabSasiadow[i - 1][j - 1] = licznikSasiadow(tab, i, j);
        }
    }
    return tabSasiadow;
}

/** funkcja jako parametr bierze tablicę z zerami i jedynkami
 *  wkłada ją w ramkę z zerami
 *  potem w podwójnej pętli sprawdza liczbę sąsiadów danej komórki i na tej podstawie zamienia ją na zero albo jedynkę
*/

void MainWindow::krok(int** tab, int N) {
    int** tab1 = umiescWRamce(tab, N);
    int** tabSasiadow = tablicaWszystkichSasiadow(tab1, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (tabSasiadow[i][j] == 3 && tab[i][j] == 0) {
                tab[i][j] = 1;
            }
            else if ((tabSasiadow[i][j] == 3 || tabSasiadow[i][j] == 2) && tab[i][j] == 1) {
                tab[i][j] = 1;
            }
            else {
                tab[i][j] = 0;
            }
        }
    }
    update();
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

void MainWindow::run(int** tab, int N) {
    for (int i = 0; i < 10; i++) {
        krok(tab, N);
    }
}

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

void MainWindow::paintEvent(QPaintEvent* event){
    QPainter painter(this); //painter to nazwa zmiennej; QPainter to klasa
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            if(state[i][j]==1){
                //rysowanieKwadracikow(i, j);
                painter.setBrush(QBrush(Qt::black, Qt::BrushStyle::SolidPattern));
                painter.drawRect(QRect(20+20*i, 20+20*j, 20, 20));
            }else{
                //QPainter painter(this);
                painter.setBrush(QBrush(Qt::white, Qt::BrushStyle::SolidPattern));
                painter.drawRect(QRect(20+20*i, 20+20*j, 20, 20)); //x position //y position //lenght //width
            }
        }
    }
}

/** funkcja do guzika start
 *  uruchamia się po naciśnięciu guzika start/stop
 *  jeśli program jest w trakcie działania, naciśnięcie guzika zatrzymuje program i zamienia tekst na start
 *  jeśli program akurat nie działa, ustawia interwał na 100 i odpala program, a tekst zamienia na stop
*/

void MainWindow::toggleStart(){
    if(timer->isActive()){
        timer->stop();
        ui->pushButton->setText("Start");
    }else{
        timer->setInterval(100); //ustawiam taki sam interwał jak w metodzie MainWindow(czyli co 100 ma się wykonać krok2)
        timer->start();
        ui->pushButton->setText("Stop");
    }
}

/** funkcja sprawia, że kwadracik zmienia kolor przy kliknięciu
*/

void MainWindow::toggleCell(int ktory_kwadracik_x, int ktory_kwadracik_y){
    if(state[ktory_kwadracik_x][ktory_kwadracik_y]==1){
        state[ktory_kwadracik_x][ktory_kwadracik_y] = 0;
    }else{
        state[ktory_kwadracik_x][ktory_kwadracik_y] = 1;
    }
}

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
    int ktory_kwadracik_x = (x/20)-1;
    int ktory_kwadracik_y = (y/20)-1;
    toggleCell(ktory_kwadracik_x, ktory_kwadracik_y);
    std::cout<<"x:"<<ktory_kwadracik_x <<" y:"<<ktory_kwadracik_y<<std::endl;
    update();
}

/** funkcja czyszcząca tablicę, żeby działał guzik "clear"
*/

void MainWindow::czyszczenieTablicy(){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            state[i][j] = 0;
        }
    }
    update();
}

/** funkcja zapełniająca tablicę losowo zerami i jedynkami, żeby działał guzik "random"
*/

void MainWindow::randomowanieTablicy() {
    srand(time(0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            state[i][j] = rand() % (2);
        }
    }
    update();
}

