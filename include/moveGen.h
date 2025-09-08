#pragma once
#include <vector>
#include "board.h"

class MoveGenerator {
private:
  Board& board;

  bool isEnemyAt(Position pos, PieceColour myColour);

  bool isEmptyAt(Position pos);

  bool isAllyAt(Position pos, PieceColour myColour);

  bool isInCheck(PieceColour myColour);

  bool isAttackedAt(const Position pos, PieceColour myColour);

  bool isMoveLegal();

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
  
  bool isAttackedByPawn(const Position pos, const PieceColour attackColour);

  bool isAttackedByKnight(const Position pos, const PieceColour attackColour);

  bool isAttackedByBishopOrQueen(const Position pos, const PieceColour attackColour);

  bool isAttackedByRookOrQueen(const Position pos, const PieceColour attackColour);

  bool isAttackedByKing(const Position pos, const PieceColour attackColour);

};
