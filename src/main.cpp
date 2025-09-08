#include "moveGen.h"


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
