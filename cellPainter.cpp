// konstruktor klasy CellPainter

CellPainter::CellPainter(QPainter &painter, int cellSide, int screenWidth) {
    this->painter = &painter;
    this->cellSide = cellSide;
    this->screenWidth = screenWidth;
}

void CellPainter::drawAliveCell(int i, int j){

}

void CellPainter::drawDeadCell(int i, int j){

}

BlackAndWhiteSquares::BlackAndWhiteSquares(QPainter &painter, int cellSide, int screenWidth) : CellPainter(painter, cellSide, screenWidth) {}

void BlackAndWhiteSquares::drawAliveCell(int i, int j){
    painter->setBrush(QBrush(Qt::black, Qt::BrushStyle::SolidPattern));
    painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
}

void BlackAndWhiteSquares::drawDeadCell(int i, int j){
    painter->setBrush(QBrush(Qt::white, Qt::BrushStyle::SolidPattern));
    painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
}

GaySquares::GaySquares(QPainter &painter, int cellSide, int screenWidth) : CellPainter(painter, cellSide, screenWidth) {}

void GaySquares::drawAliveCell(int i, int j){
//    if(j>=0 && j<screenWidth/2/cellSide){
//        painter->setBrush(QBrush(Qt::red, Qt::BrushStyle::SolidPattern));
//        painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
//    }else{
//        painter->setBrush(QBrush(Qt::blue, Qt::BrushStyle::SolidPattern));
//        painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
//    }
    // zrefaktorować numOfCols - czyli poprawić bez zmiany funkcjonalności
    int numOfCols = screenWidth/cellSide;
    if(j>=0 && j<numOfCols/6){
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
