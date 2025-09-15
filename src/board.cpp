#include "board.h"


Board::Board()
{
  for (int row = 0; row < 8; row++)
  {
    for (int col = 0; col < 8; col++)
    {
      grid[row][col] = Piece(PieceColour::none, PieceType::none);
    }
  }
  for (int col = 0; col < 8; col++)
  {
    grid[1][col] = Piece(PieceColour::black, PieceType::pawn);
    grid[6][col] = Piece(PieceColour::white, PieceType::pawn);
  }
  PieceType backrank[8] = {
      PieceType::rook, PieceType::knight, PieceType::bishop, PieceType::queen,
      PieceType::king, PieceType::bishop, PieceType::knight, PieceType::rook};
  for (int col = 0; col < 8; col++)
  {
    grid[0][col] = Piece(PieceColour::black, backrank[col]);
    grid[7][col] = Piece(PieceColour::white, backrank[col]);
  }
}



Piece Board::getPiece(const Position pos) const {
  return grid[pos.row][pos.col];
}

Position Board::getKingPos(const PieceColour myColour) const{
  for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++) {
      Piece piece(grid[row][col]);
      if (piece.getColour() == myColour && piece.getType() == PieceType::king) {
        return {row, col};
      }
    }
  }

  return {0, 0};
}


void Board::print()
{
  for (int row = 0; row < 8; row++)
  {
    std::cout << 8 - row  << " ";
    for (int col = 0; col < 8; col++)
    {
      std::cout << grid[row][col].typeToString() << " ";
    }
    std::cout << "\n";
  }
  std::cout << "  a  b  c  d  e  f  g  h\n";
  std::cout << "\n" << "Input move: ";
  
}


void Board::move(Position from, Position to) {
  Piece piece = grid[from.row][from.col];
  grid[from.row][from.col] = Piece(PieceColour::none, PieceType::none);
  grid[to.row][to.col] = piece;
}

void Board::undoMove(Position from, Position to, Piece CapturedPiece) {
  Piece piece = grid[to.row][to.col];
  grid[to.row][to.col] = CapturedPiece;
  grid[from.row][from.col] = piece;
}
