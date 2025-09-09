#pragma once
#include "piece.h"
#include <iostream>

class Board
{
private:
  Piece grid[8][8];

public:
  Board();

  Piece getPiece(const Position pos) const;

  Position getKingPos(const PieceColour myColour) const;
  
  void print();

  void move(Position from, Position to);

  void undoMove(Position from, Position to, Piece capturedPiece);

};

