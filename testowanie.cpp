#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>



void MainWindow::testujGet2d() {
    int a[5][5] = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1}
    };
    int** b = get2d(5);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            b[i][j] = 1;
        }
    }
    int licznik = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (a[i][j] == b[i][j]) {
                licznik++;
            }
        }
    }
    if (licznik == 5 * 5) {
        cout << "Funkcja get2d dziala dobrze!" << endl;
    }
    else {
        cout << "Funkcja get2d dziala zle :c" << endl;
    }
}

void MainWindow::testujUmiescWRamce() {
    int** a = get2d(5);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            a[i][j] = 1;
        }
    }
    int** ramka = umiescWRamce(a, 5);
    int b[7][7] = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };
    int licznik = 0;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (ramka[i][j] == b[i][j]) {
                licznik++;
            }
        }
    }
    if (licznik == 7 * 7) {
        cout << "Funkcja umiescWRamce dziala dobrze!" << endl;
    }
    else {
        cout << "Funkcja umiescWRamce dziala zle :c" << endl;
    }

}

void MainWindow::testujLicznikSasiadowNeumann() {
    int** a = get2d(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a[i][j] = 1;
        }
    }
    int licznik = licznikSasiadowNeumann(a, 1, 1);
    if (licznik == 4) {
        cout << "Funkcja licznikSasiadowNeumann dziala dobrze!" << endl;
    }
    else {
        cout << "Funkcja licznikSasiadowNeumann dziala zle :c" << endl;
    }
}

void MainWindow::testujLicznikSasiadowMoore() {
    int** a = get2d(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a[i][j] = 1;
        }
    }
    int licznik = licznikSasiadowMoore(a, 1, 1);
    if (licznik == 4) {
        cout << "Funkcja licznikSasiadowMoore dziala dobrze!" << endl;
    }
    else {
        cout << "Funkcja licznikSasiadowMoore dziala zle :c" << endl;
    }
}


void MainWindow::testujTablicaWszystkichSasiadow() {
    int** a = get2d(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a[i][j] = 0;
        }
    }
    int** tabSasiadow = tablicaWszystkichSasiadow(a, 3);
    int** b = get2d(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            b[i][j] = 0;
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabSasiadow[i][j] == b[i][j]) {
                cout << "Funkcja tablicaWszystkichSasiadow dziala dobrze!" << endl;
            }
            else {
                cout << "Funkcja tablicaWszystkichSasiadow dziala zle :c" << endl;
            }
        }
    }
}

void MainWindow::testujKrok() {
    int** a = get2d(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a[i][j] = 1;
        }
    }
    krok(a, 3);
    int** b = get2d(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            b[i][j] = 0;
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (a[i][j] == b[i][j]) {
                cout << "Funkcja krok dziala dobrze!" << endl;
            }
            else {
                cout << "Funkcja krok dziala zle :c" << endl;
            }
        }
    }
}

void MainWindow::testujWszystko() {
    void testujGet2d();
    void testujUmiescWRamce();
    void testujLicznikSasiadowNeumann();
    void testujLicznikSasiadowMoore();
    void testujTablicaWszystkichSasiadow();
    void testujKrok();
}
