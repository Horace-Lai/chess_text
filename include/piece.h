#pragma once 
#include <string>

struct Position
{
  int row, col;

  Position(int row = 0, int col = 0) : row(row), col(col) {}

  bool operator==(const Position &other) const;
  
  bool operator!=(const Position &other) const;

  bool isValid();

};

enum class PieceColour
{
  white,
  black,
  none
};

enum class PieceType
{
  pawn,
  knight,
  bishop,
  rook,
  queen,
  king,
  none
};

class Piece
{
private:
  PieceColour colour;
  PieceType type;

public:
  Piece(PieceColour colour = PieceColour::none,
        PieceType type = PieceType::none)
      : colour(colour), type(type) {}

  PieceColour getColour();

  PieceType getType();

  bool isEmpty();

  std::string toString();
  
};
