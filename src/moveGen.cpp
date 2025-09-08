#include "moveGen.h"

bool MoveGenerator::isEnemyAt(Position pos, PieceColour myColour) {
  if (!pos.isValid()) return false;
  Piece piece = board.getPiece(pos);
  return !piece.isEmpty() && piece.getColour() != myColour;
}


bool MoveGenerator::isEmptyAt(Position pos) {
  if (!pos.isValid()) return false;
  return board.getPiece(pos).isEmpty();
}


bool MoveGenerator::isAllyAt(Position pos, PieceColour myColour) {
  if (!pos.isValid()) return false;
  Piece piece = board.getPiece(pos);
  return !piece.isEmpty() && piece.getColour() == myColour;
}


bool MoveGenerator::isCheckedAt(const Position pos, PieceColour myColour) {
  std::vector<Position> checks = {};
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col <8; col++) {
      if (isEmptyAt({row, col}) || isAllyAt({row, col}, myColour)) {
        continue;
      } else if (isEnemyAt({row, col}, myColour)) {
        std::vector<Position> enemy_move = getLegalMoves({row, col});
        checks.insert(checks.end(), enemy_move.begin(), enemy_move.end());
      }
    }
  }
  for (auto check : checks) {
    if ((pos == check)) return true;
  }
  return false;
}


std::vector<Position> MoveGenerator::getLegalMoves(Position from) {
  Piece piece = board.getPiece(from);
  if (piece.isEmpty()) return {};
  PieceType type = piece.getType();
  PieceColour colour = piece.getColour();
  std::vector<Position> moves = {};
  switch(type) {
    case PieceType::pawn: 
      getPawnMoves(from, colour, moves);
      break;
    case PieceType::knight: 
      getKnightMoves(from, colour, moves);
      break;
    case PieceType::bishop:
      getBishopMoves(from, colour, moves);
      break;
    case PieceType::rook: 
      getRookMoves(from, colour, moves);
      break;
    case PieceType::king:
      getKingMoves(from, colour, moves);
      break;
    case PieceType::queen: 
      getQueenMoves(from, colour, moves);
      break;
    default:
      break;
  }
  return moves;
}


void MoveGenerator::getPawnMoves(const Position from, const PieceColour colour, std::vector<Position> &moves) {
  int direction = (colour == PieceColour::white)? -1:1;
  int starting_row = (colour == PieceColour::white)? 6:1;
  Position foward1(from.row + direction, from.col);
  
  if (isEmptyAt(foward1) && foward1.isValid()) {
    moves.push_back(foward1);
  }
  Position foward2(from.row + direction *2, from.col);
  if (from.row == starting_row && isEmptyAt(foward2)) {
    moves.push_back(foward2);
  }
  
  Position diagLeft(from.row + direction,  from.col - 1);
  Position diagRight(from.row + direction,  from.col + 1);
  if (isEnemyAt(diagLeft, colour) && diagLeft.isValid()) moves.push_back(diagLeft);
  if (isEnemyAt(diagRight, colour) && diagRight.isValid()) moves.push_back(diagRight);
}


void MoveGenerator::getKnightMoves(const Position from, const PieceColour colour, std::vector<Position> &moves) {
  std::vector<Position> offsets = {
          {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
          {1, -2}, {1, 2}, {2, -1}, {2, 1}
      };
  for (auto offset: offsets) {
    Position target(from.row + offset.row, from.col + offset.col);
    if (target.isValid() && !isAllyAt(target, colour)) {
      moves.push_back(target);
    }
  }
}


void MoveGenerator::getSlidingMoves(const Position from, const PieceColour colour, std::vector<Position> &moves, std::vector<Position> &directions) {
  for (auto direction: directions) {
    for (int i = 1; i < 8; i++) {
      Position target(from.row + i * direction.row, from.col + i * direction.col);
      if (!target.isValid()) {
        break;
      } else if (isEmptyAt(target)) {
        moves.push_back(target);
      } else if (isEnemyAt(target, colour)) {
        moves.push_back(target);
        break;
      } else {
        break;
      }
    }
  }
}


void MoveGenerator::getBishopMoves(const Position from, const PieceColour colour, std::vector<Position> &moves) {
  std::vector<Position> directions = { 
      {-1, -1}, {-1, 1}, {1, 1}, {1, -1}
  };
  getSlidingMoves(from, colour, moves, directions);
}
void MoveGenerator::getRookMoves(const Position from, const PieceColour colour, std::vector<Position> &moves) {
  std::vector<Position> directions = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1}
  };
  getSlidingMoves(from, colour, moves, directions);
}
void MoveGenerator::getQueenMoves(const Position from, const PieceColour colour, std::vector<Position> &moves) {
  std::vector<Position> directions_1 = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1}
  };
  getSlidingMoves(from, colour, moves, directions_1);
  std::vector<Position> directions_2 = { 
      {-1, -1}, {-1, 1}, {1, 1}, {1, -1}
  };
  getSlidingMoves(from, colour, moves, directions_2);
}
void MoveGenerator::getKingMoves(const Position from, const PieceColour colour, std::vector<Position> &moves) {
  std::vector<Position> directions = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1},
    {-1, -1}, {-1, 1}, {1, 1}, {1, -1}
  };
  for (auto direction: directions) {
     Position target(from.row + direction.row, from.col + direction.col);
     if (!isAllyAt(target, colour) && target.isValid()) moves.push_back(target);
  }
}  
