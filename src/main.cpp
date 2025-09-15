#include "moveGen.h"


int main()
{
  Board board;
  MoveGenerator moveGen(board);
  

  std::string start;
  std::string des;

  PieceColour current_turn =  PieceColour::white;
  std::string current_in_str = (current_turn == PieceColour::white)? "White": "Black";
  std::cout << "Current turn: " << current_in_str << "\n";
  board.print();

  while(std::cin >> start) {
    if (start == "q") {
      std::cout << current_in_str << " resigned...\n";
      break;
    };

    std::cin >> des;
      
    Position from = Position(8 - (start[1] - '0'), start[0] - 'a' );
    Position to = Position(8 - (des[1] - '0'), des[0] - 'a' );

    Piece piece = board.getPiece(from);
    PieceColour myColour = piece.getColour();

    if (moveGen.isMoveLegal(from, to) && myColour == current_turn) {
      board.move(from, to);
      current_turn = (current_turn == PieceColour::white)? PieceColour::black: PieceColour::white; 
      std::string current_in_str = (current_turn == PieceColour::white)? "White": "Black"; 

      if (moveGen.isCheckmated(current_turn)) {
        board.print();
        std::cout << current_in_str << " is checkmated!\n";
        break;
      } else if (moveGen.isStalemated(current_turn)) {
        board.print();
        std::cout << "Draw!\n";
        break;
      }
      
    } else {
      std::cout << "Illegal move by " << current_in_str << "\n";
    }
    
    std::cout << "Current turn: " << current_in_str << "\n";
    board.print();
    
  }
}
