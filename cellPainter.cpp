// konstruktor klasy CellPainter

CellPainter::CellPainter(QPainter &painter, int cellSide) {
    this->painter = &painter;
    this->cellSide = cellSide;
}

void CellPainter::drawAliveCell(int i, int j){

}

void CellPainter::drawDeadCell(int i, int j){

}

void BlackAndWhiteSquares::drawAliveCell(int i, int j){
    painter->setBrush(QBrush(Qt::black, Qt::BrushStyle::SolidPattern));
    painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
}

void BlackAndWhiteSquares::drawDeadCell(int i, int j){
    painter->setBrush(QBrush(Qt::white, Qt::BrushStyle::SolidPattern));
    painter->drawRect(QRect(cellSide*j-cellSide, cellSide*i-cellSide, cellSide, cellSide));
}
