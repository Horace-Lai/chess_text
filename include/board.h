#pragma once
#include "piece.h"
#include <iostream>

class Board
{
private:
  Piece grid[8][8];

public:
  Board();

  Piece getPiece(Position pos);

  Position getKingPos(PieceColour myColour);
  
  void print();

  void move(Position from, Position to);

};

