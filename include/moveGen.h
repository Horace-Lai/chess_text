#pragma once
#include <vector>
#include "board.h"

class MoveGenerator {
private:
  Board& board;

  bool isEnemyAt(Position pos, PieceColour myColour);

  bool isEmptyAt(Position pos);

  bool isAllyAt(Position pos, PieceColour myColour);

  bool isCheckedAt(const Position pos, PieceColour myColour);

public:

  MoveGenerator(Board &board): board(board) {}

  std::vector<Position> getLegalMoves(Position from);

private:
  void getPawnMoves(const Position from, const PieceColour colour, std::vector<Position> &moves);

  void getKnightMoves(const Position from, const PieceColour colour, std::vector<Position> &moves);

  void getSlidingMoves(const Position from, const PieceColour colour, std::vector<Position> &moves, std::vector<Position> &directions);

  void getBishopMoves(const Position from, const PieceColour colour, std::vector<Position> &moves);

  void getRookMoves(const Position from, const PieceColour colour, std::vector<Position> &moves);

  void getQueenMoves(const Position from, const PieceColour colour, std::vector<Position> &moves);

  void getKingMoves(const Position from, const PieceColour colour, std::vector<Position> &moves); 
  
};
