#include "moveGen.h"

bool MoveGenerator::isEnemyAt(const Position pos,const PieceColour myColour) {
  if (!pos.isValid()) return false;
  Piece piece = board.getPiece(pos);
  return !piece.isEmpty() && piece.getColour() != myColour;
}


bool MoveGenerator::isEmptyAt(const Position pos) {
  if (!pos.isValid()) return false;
  return board.getPiece(pos).isEmpty();
}


bool MoveGenerator::isAllyAt(const Position pos, const PieceColour myColour) {
  if (!pos.isValid()) return false;
  Piece piece = board.getPiece(pos);
  return !piece.isEmpty() && piece.getColour() == myColour;
}


bool MoveGenerator::isInCheck(const PieceColour myColour) {

  Position king_pos = board.getKingPos(myColour);

  PieceColour opponent_colour = (myColour == PieceColour::white)? PieceColour::black : PieceColour::white;

  return isAttackedAt(king_pos, opponent_colour);

}

bool MoveGenerator::isAttackedAt(const Position pos, const PieceColour attackerColour) {
  return (isAttackedByPawn(pos, attackerColour) ||
          isAttackedByKnight(pos, attackerColour) ||
          isAttackedByBishopOrQueen(pos, attackerColour) ||
          isAttackedByRookOrQueen(pos, attackerColour) ||
          isAttackedByKing(pos, attackerColour));
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

bool MoveGenerator::isMoveLegal(Position from, Position to, PieceColour myColour) {
  std::vector<Position> legalMoves = getLegalMoves(from);
  Piece capturedPiece = board.getPiece(to);
  for (const auto& move : legalMoves) {
    if (move == to) {
      board.move(from, to);
      if (!isInCheck(myColour)) {
        board.undoMove(from, to, capturedPiece);
        return true;
      } else {
        board.undoMove(from, to, capturedPiece);
        return false;
      }
    }
  }
  return false;
}

bool MoveGenerator::isCheckmated(const PieceColour myColour) {
  
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


void MoveGenerator::getSlidingMoves(const Position from, const PieceColour colour, std::vector<Position> &moves, const std::vector<Position> &directions) {
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

bool MoveGenerator::isAttackedByPawn(const Position pos, const PieceColour attackColour) {
  int direction = (attackColour == PieceColour::white)? 1: -1;

  std::vector<Position> attackPositions = {
         {pos.row + direction, pos.col + 1},
         {pos.row + direction, pos.col - 1}
      };

  for (const auto& attackPos : attackPositions) {
    if (attackPos.isValid()) {
      Piece attacker = board.getPiece(attackPos);
      if (attacker.getColour() == attackColour && attacker.getType() == PieceType::pawn) {
        return true;
      } 
    }
  }

  return false;
}

bool MoveGenerator::isAttackedByKnight(const Position pos, const PieceColour attackColour) {
  std::vector<Position> offsets = {
          {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
          {1, -2}, {1, 2}, {2, -1}, {2, 1}
      };

  for (const auto& offset : offsets) {
    Position attackPos(pos.row + offset.row, pos.col + offset.col);
    if (attackPos.isValid()) {
      Piece attacker = board.getPiece(attackPos);
      if (attacker.getColour() == attackColour && attacker.getType() == PieceType::knight) {
        return true;
      } 
    }
  }

  return false;
}



bool MoveGenerator::isAttackedByBishopOrQueen(const Position pos, const PieceColour attackColour) {
  std::vector<Position> directions = { 
      {-1, -1}, {-1, 1}, {1, 1}, {1, -1}
  };

  PieceColour myColour = (attackColour == PieceColour::white)? PieceColour::black: PieceColour::white;

  for(const auto& direction :directions) {
    for(int i  = 0; i < 8; i++) {
      Position attackPos(pos.row + i * direction.row, pos.col + i * direction.col);
      Piece attacker = board.getPiece(attackPos);
      if (!attackPos.isValid() || attacker.getColour() == myColour) {
        break;
      } else if (attacker.getColour() == attackColour && ((attacker.getType() == PieceType::bishop || attacker.getType() == PieceType::queen))) {
        return true;
      }
    }
  }

  return false;
}

bool MoveGenerator::isAttackedByRookOrQueen(const Position pos, const PieceColour attackColour) {
  std::vector<Position> directions = { 
      {-1, 0}, {0, 1}, {1, 0}, {0, -1}
  };

  PieceColour myColour = (attackColour == PieceColour::white)? PieceColour::black: PieceColour::white;

  for(const auto& direction :directions) {
    for(int i  = 0; i < 8; i++) {
      Position attackPos(pos.row + i * direction.row, pos.col + i * direction.col);
      Piece attacker = board.getPiece(attackPos);
      if (!attackPos.isValid() || attacker.getColour() == myColour) {
        break;
      } else if (attacker.getColour() == attackColour && ((attacker.getType() == PieceType::rook || attacker.getType() == PieceType::queen))) {
        return true;
      }
    }
  }

  return false;
}

bool MoveGenerator::isAttackedByKing(const Position pos, const PieceColour attackColour) {
  std::vector<Position> directions = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1},
    {-1, -1}, {-1, 1}, {1, 1}, {1, -1}
  };

  for(const auto& direction : directions) {
    Position attackPos(pos.row + direction.row, pos.col + direction.col);
    if (attackPos.isValid()) {
      Piece attacker = board.getPiece(attackPos);
      if (attacker.getColour() == attackColour && attacker.getType() == PieceType::king) {
        return true;
      }
    }
  }

  return false;
}

