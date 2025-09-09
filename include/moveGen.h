#pragma once
#include <vector>
#include "board.h"

class MoveGenerator {
private:
  Board& board;

  bool isEnemyAt(const Position pos, const PieceColour myColour);

  bool isEmptyAt(const Position pos);

  bool isAllyAt(const Position pos, const PieceColour myColour);

  bool isInCheck(const PieceColour myColour);

  bool isAttackedAt(const Position pos, const PieceColour myColour);

  std::vector<Position> getLegalMoves(const Position from);

public:

  MoveGenerator(Board &board): board(board) {}
  
  bool isMoveLegal(Position from, Position to, PieceColour myColour);

  bool isCheckmated(const PieceColour myColour);

private:

  void getPawnMoves(const Position from, const PieceColour colour, std::vector<Position> &moves);

  void getKnightMoves(const Position from, const PieceColour colour, std::vector<Position> &moves);

  void getSlidingMoves(const Position from, const PieceColour colour, std::vector<Position> &moves, const std::vector<Position> &directions);

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
