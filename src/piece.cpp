#include <string>
#include "piece.h"


bool Position::operator==(const Position &other) const { return row == other.row && col == other.col; }
  
bool Position::operator!=(const Position &other) const { return !(*this == other); }

bool Position::isValid() { return (row < 8 && row >= 0 && col < 8 && col >= 0); }


PieceColour Piece::getColour() {
  return colour;
}
PieceType Piece::getType() {
  return type;
}
bool Piece::isEmpty() {
  return (colour == PieceColour::none);
}
std::string Piece::toString()
{
  if (isEmpty()) return "--";
  std::string result = "";
  result += (colour == PieceColour::white)? 'w': 'b';
  switch (type)
  {
  case PieceType::pawn: result += 'p' ; break;
  case PieceType::knight: result += 'n' ; break;
  case PieceType::bishop: result += 'b' ; break;
  case PieceType::king: result += 'k' ; break;
  case PieceType::queen: result += 'q' ; break;
  case PieceType::rook: result += 'r' ; break;
  default: break;
  }
  return result;
}

