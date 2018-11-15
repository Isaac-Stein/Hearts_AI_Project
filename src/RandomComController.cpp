#include "RandomComController.h"

/**
 * @brief Computer controller chooses which action to undertake.
 *        Choice of card to play is completely random.
 *
 * @return The value of the card that the player decided to play.
 */
int RandomComController::Act() {
  int choice, card;
  do {choice = (rand()%player_->GetHandSize()) + 1;} while (!player_->IsValidPlay(choice-1));
  //std::cout << player_->GetName() << ": ";
  card = player_->PullCard(choice-1);
  //PrintCard(card);
//  std::cout << std::endl;
  return card;
}
