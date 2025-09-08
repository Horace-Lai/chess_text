#include <iostream>
#include <string>
#include <vector>

struct Position
{
  int row, col;

  Position(int row = 0, int col = 0) : row(row), col(col) {}

  bool operator==(const Position &other) const { return row == other.row && col == other.col; }
  
  bool operator!=(const Position &other) const { return !(*this == other); }

  bool isValid() { return (row < 8 && row >= 0 && col < 8 && col >= 0); }

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

  PieceColour getColour() {
    return colour;
  }

  PieceType getType() {
    return type;
  }

  bool isEmpty() {
    return (colour == PieceColour::none);
  }

  std::string toString()
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
};



class Board
{
private:
  Piece grid[8][8];

public:
  Board()
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

  Piece getPiece(Position pos) {
    return grid[pos.row][pos.col];
  }

  void print()
  {
    for (int row = 0; row < 8; row++)
    {
      std::cout << 8 - row  << " ";
      for (int col = 0; col < 8; col++)
      {
        std::cout << grid[row][col].toString() << " ";
      }
      std::cout << "\n";
    }

    std::cout << "  a  b  c  d  e  f  g  h\n";
    return;
  }

  void move(Position from, Position to) {
    Piece piece = grid[from.row][from.col];
    grid[from.row][from.col] = Piece(PieceColour::none, PieceType::none);
    grid[to.row][to.col] = piece;
  }

};


class MoveGenerator {
private:
  Board& board;

  bool isEnemyAt(Position pos, PieceColour myColour) {
    if (!pos.isValid()) return false;
    Piece piece = board.getPiece(pos);
    return !piece.isEmpty() && piece.getColour() != myColour;
  }

  bool isEmptyAt(Position pos) {
    if (!pos.isValid()) return false;
    return board.getPiece(pos).isEmpty();
  }

  bool isAllyAt(Position pos, PieceColour myColour) {
    if (!pos.isValid()) return false;
    Piece piece = board.getPiece(pos);
    return !piece.isEmpty() && piece.getColour() == myColour;
  }

  bool isCheckedAt(const Position pos, PieceColour myColour) {
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

public:

  MoveGenerator(Board &board): board(board) {}

  std::vector<Position> getLegalMoves(Position from) {
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
    }

    return moves;
  }

private:
  void getPawnMoves(const Position from, const PieceColour colour, std::vector<Position> &moves) {
    Position king_pos = board.getKingPos(colour);

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


  void getKnightMoves(const Position from, const PieceColour colour, std::vector<Position> &moves) {
    Position king_pos = board.getKingPos(colour);

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

  void getSlidingMoves(const Position from, const PieceColour colour, std::vector<Position> &moves, std::vector<Position> &directions) {
    Position king_pos = board.getKingPos(colour);

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

  void getBishopMoves(const Position from, const PieceColour colour, std::vector<Position> &moves) {
    std::vector<Position> directions = { 
        {-1, -1}, {-1, 1}, {1, 1}, {1, -1}
    };
    getSlidingMoves(from, colour, moves, directions);
  }

  void getRookMoves(const Position from, const PieceColour colour, std::vector<Position> &moves) {
    std::vector<Position> directions = {
      {-1, 0}, {0, 1}, {1, 0}, {0, -1}
    };
    getSlidingMoves(from, colour, moves, directions);
  }

  void getQueenMoves(const Position from, const PieceColour colour, std::vector<Position> &moves) {
    std::vector<Position> directions_1 = {
      {-1, 0}, {0, 1}, {1, 0}, {0, -1}
    };
    getSlidingMoves(from, colour, moves, directions_1);

    std::vector<Position> directions_2 = { 
        {-1, -1}, {-1, 1}, {1, 1}, {1, -1}
    };
    getSlidingMoves(from, colour, moves, directions_2);
  }

  void getKingMoves(const Position from, const PieceColour colour, std::vector<Position> &moves) {
    std::vector<Position> directions = {
      {-1, 0}, {0, 1}, {1, 0}, {0, -1},
      {-1, -1}, {-1, 1}, {1, 1}, {1, -1}
    };

    for (auto direction: directions) {
       Position target(from.row + direction.row, from.col + direction.col);
       if (!isAllyAt(target, colour) && target.isValid()) moves.push_back(target);
    }
  }

  
  
};


int main()
{
  Board board;
  MoveGenerator moveGen(board);
  board.print();

  std::string start;
  std::string des;

  while(std::cin >> start) {
    if (start == "q") break;
    std::cin >> des;
      
    Position from = Position(8 - (start[1] - '0'), start[0] - 'a' );
    Position to = Position(8 - (des[1] - '0'), des[0] - 'a' );
  
    bool legal = false;
    std::vector<Position> legalMoves = moveGen.getLegalMoves(from);
    for(auto legalMove : legalMoves) {
      std::cout << legalMove.row << " " << legalMove.col << "\n";
      if (legalMove == to) {
        legal = true;
        break;
      }
    }
  
    if (legal) {
      board.move(from, to);
    } else {
      std::cout << "illegal" << "\n";
    }
  
    board.print();
  }
}
