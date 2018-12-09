#include "HumanController.h"

/**
 * @brief Human controller chooses which action to undertake.
 *
 * @return The value of the card that the player decided to play.
 */
int HumanController::Act() {
  int choice = 0, card;
  printf("It is your turn. Your hand is as follows:\n");
  player_->PrintHand();
  std::cout << "      ";
  for (int i=1;i<10;i++) {
    std::cout << "(" << i<< ") ";
  }
  for (int i=10;i<14;i++) {
    std::cout << "(" << i << ")";
  }
  std::cout << std::endl;
  printf("Which card do you wish to play?\n");

  do {std::cin >> choice;} while (!player_->IsValidPlay(choice-1));

  card = player_->PullCard(choice-1);
  std::cout << player_->GetName() << ": ";
  PrintCard(card);
  std::cout << std::endl;

  return card;
}
