/** Klasa CellPainter
 *  Służy do zapewnienia polimorfizmu
 *  Daje możliwość przełączania się pomiędzy trybami "gay" i "straight" aplikacji
 *  Pola: painter, cellSide, screenWidth
*/


/** Konstruktor CellPainter
 *  Parametry to wskaźnik do paintera, długość boku komórki, szerokość ekranu
*/

CellPainter::CellPainter(QPainter &painter, int cellSide, int screenWidth) {
    this->painter = &painter;
    this->cellSide = cellSide;
    this->screenWidth = screenWidth;
}

/** Wirtualna metoda do rysowania żywej komórki
 *  Parametry to indeks wiersza i kolumny
*/
void CellPainter::drawAliveCell(int i, int j){

}

/** Wirtualna metoda do rysowania martwej komórki
 *  Parametry to indeks wiersza i kolumny
*/
void CellPainter::drawDeadCell(int i, int j){

}

/** Klasa BlackAndWhiteSquares
 *  Dziedziczy po klasie CellPainter
 *  Nadpisuje metody drawAliveCell i drawDeadCell
*/

BlackAndWhiteSquares::BlackAndWhiteSquares(QPainter &painter, int cellSide, int screenWidth) : CellPainter(painter, cellSide, screenWidth) {}

void BlackAndWhiteSquares::drawAliveCell(int i, int j){
    painter->setBrush(QBrush(Qt::black, Qt::BrushStyle::SolidPattern));
    painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
}

void BlackAndWhiteSquares::drawDeadCell(int i, int j){
    painter->setBrush(QBrush(Qt::white, Qt::BrushStyle::SolidPattern));
    painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
}

/** Klasa GaySquares
 *  Dziedziczy po klasie CellPainter
 *  Nadpisuje metody drawAliveCell i drawDeadCell
*/

GaySquares::GaySquares(QPainter &painter, int cellSide, int screenWidth) : CellPainter(painter, cellSide, screenWidth) {}


/** Metoda GaySquares::drawAliveCell
 *  Jest trochę bardziej złożona, więc zasługuje na opis
 *  Nadpisuje metody drawAliveCell i drawDeadCell
 *  Dzieli liczbę kolumn na 6 części, aby każdej przypisać jeden z kolorów tęczy
*/
void GaySquares::drawAliveCell(int i, int j){
    int numOfCols = screenWidth/cellSide;
    if(j>=1 && j<numOfCols/6){
        painter->setBrush(QBrush(Qt::red, Qt::BrushStyle::SolidPattern));
        painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
    }else if(j<2*(numOfCols/6)){
        painter->setBrush(QBrush(QColor::fromRgb(255, 165, 0), Qt::BrushStyle::SolidPattern));
        painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
    }else if(j<3*(numOfCols/6)){
        painter->setBrush(QBrush(Qt::yellow, Qt::BrushStyle::SolidPattern));
        painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
    }else if(j<4*(numOfCols/6)){
        painter->setBrush(QBrush(Qt::green, Qt::BrushStyle::SolidPattern));
        painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
    }else if(j<5*(numOfCols/6)){
        painter->setBrush(QBrush(Qt::blue, Qt::BrushStyle::SolidPattern));
        painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
    }else{
        painter->setBrush(QBrush(QColor::fromRgb(160, 8, 211), Qt::BrushStyle::SolidPattern));
        painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
    }

}

void GaySquares::drawDeadCell(int i, int j){
    painter->setBrush(QBrush(Qt::white, Qt::BrushStyle::SolidPattern));
    painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
}
