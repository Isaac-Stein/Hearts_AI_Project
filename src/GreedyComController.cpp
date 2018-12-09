#include "GreedyComController.h"

/**
 * @brief Computer controller chooses which action to undertake.
 *        Choice of card to play is completely random.
 *
 * @return The value of the card that the player decided to play.
 */
int GreedyComController::Act() {
  if (curr_trick_ == 0) return PlayMaxFirstTrick();
  if (curr_turn_ == 0) return PlayMinCard();
  else if (curr_turn_ == 3) return PlayMinPointsMaxCard();
  else return PlayNormally();
}

void GreedyComController::SeeLaid(unsigned char card) {
  laid_cards_[curr_turn_] = card;

  if (card == Q_SPADES) {
    queen_played_ = true;
    trick_points_ += 13;
  }
  if (card > A_SPADES) trick_points_++;

  if (high_card_ == 0 || Suit(card) == Suit(high_card_) && Value(card) > Value(high_card_)) {
    high_card_ = card;
  }


  curr_turn_ = (curr_turn_+1)%4;


  if (curr_turn_ == 0) {
      curr_trick_++;
      trick_points_ = 0;
      high_card_ = 0;
  }
}

void GreedyComController::Reset() {
  curr_trick_ = 0;
  curr_turn_ = 0;
  queen_played_ = false;
  trick_points_ = 0;
  high_card_ = 0;
}

/**
 * @brief Plays the greedily best card on the first trick
 *
 * @return The value of the greedily best card on the first trick.
 */
int GreedyComController::PlayMaxFirstTrick() {
  int *hand = player_->GetHand();
  unsigned char hand_size = player_->GetHandSize();
  char max_index = -1;
  char max_value = -1;

  // Returns the 2 of Clubs if present
  if (hand[0] == 1) return player_->PullCard(0);

  // Returns the highest club if there are any clubs
  int itt = 0;
  for(; hand[itt]<=13; itt++);
  if (itt!=0) return player_->PullCard(itt-1);

  // Returns the Ace or King of spades if present, otherwise return highest non-heart card
  for (; hand[itt]<=A_SPADES && itt<hand_size; itt++) {
    if (hand[itt]!=Q_SPADES && Value(hand[itt]) > max_value || hand[itt] == K_SPADES || hand[itt] == A_SPADES) {
      max_index = itt;
      max_value = Value(hand[itt]);
    }
  }

  // If we reach this point with the max_index at -1 then we only have point cards
  // We play the Queen of Spades if we have it, otherwise we play our highest Heart
  if (max_index == -1 && hand[0] == Q_SPADES) return player_->PullCard(0);
  else if (max_index == -1) return player_->PullCard(hand_size-1);

  return player_->PullCard(max_index);
}

/**
 * @brief Plays the minimum value card in the hand.
 *        Assumes that the player is leading.
 *
 * @return The value of the greedily best minimum card.
 */
int GreedyComController::PlayMinCard() {
  int *hand = player_->GetHand();
  unsigned char hand_size = player_->GetHandSize();
  int end_card;
  int itt=0;
  char min_index = -1;
  char min_value = 13;

  // Gets the last card to search to, based off of if Hearts are broken or not.
  if (player_->GetHeartsBroken()) end_card = A_HEARTS;
  else end_card = A_SPADES;

  //printf("the end card is %d\n",end_card);
  // Returns the minimum value card in the hand, ignoring the Queen of Spades if the player has it
  for (; hand[itt]<=end_card && itt<hand_size; itt++) {
    if (hand[itt] != 0 && hand[itt]!=Q_SPADES && Value(hand[itt]) < min_value) {
      //printf("%d %d\n",itt, Value(hand[itt]));
      min_index = itt;
      min_value = Value(hand[itt]);
    }
  }

  // If there were no cards found, then we must only have points cards while hearts are not broken
  //   play the Queen of Spades if we have it
  //   otherwise we must play a heart
  if (min_index == -1) {
    for (itt=0; hand[itt]<=A_HEARTS && itt<hand_size; itt++) {
      if (hand[itt] != 0) {
        min_index = itt;
        break;
      }
    }
  }
  return player_->PullCard(min_index);
}

/**
 * @brief Plays the maximum card in hand.
 *
 * @return The value of the card
 */
int GreedyComController::PlayMaxCard() {
  int *hand = player_->GetHand();
  unsigned char hand_size = player_->GetHandSize();
  int itt;

  char max_index = -1;
  char max_value = -1;

  // Check if the queen was played to decide the value of cards.
  switch (queen_played_){
    case (false):{
      // Returns the Queen, Ace, or King of Spades if present
      //   otherwise fall down to the true case and play normally
      for (itt=0; hand[itt]<=A_SPADES && itt<hand_size; itt++) {
        if (hand[itt] == Q_SPADES) {
          return player_->PullCard(itt);
        }
        if (hand[itt] == K_SPADES || hand[itt] == A_SPADES) {
          max_index = itt;
          max_value = Value(hand[itt]);
        }
      }
      if (max_index != -1) return player_->PullCard(max_index);
    }
    case (true):{
      // Choose the max value card, favoring hearts over all other cards
      for (itt=0; hand[itt]<=A_HEARTS && itt<hand_size; itt++) {
        if (hand[itt] != 0 && Value(hand[itt]) > max_value || hand[itt] !=0 && hand[itt] > A_SPADES) {
          max_index = itt;
          max_value = Value(hand[itt]);
        }
      }
      //printf("max card of %d found at index %d\n",max_value, max_index);
      return player_->PullCard(max_index);
    }
  }

  // Returns the Ace or King of spades if present, otherwise return highest non-heart card
  for (; hand[itt]<=A_SPADES && itt<hand_size; itt++) {
    if (hand[itt]!=Q_SPADES && Value(hand[itt]) > max_value || hand[itt] == K_SPADES || hand[itt] == A_SPADES) {
      max_index = itt;
      max_value = Value(hand[itt]);
    }
  }

  // If we reach this point with the max_index at -1 then we only have point cards
  // We play the Queen of Spades if we have it, otherwise we play our highest Heart
  if (max_index == -1 && hand[0] == Q_SPADES) return player_->PullCard(0);
  else if (max_index == -1) return player_->PullCard(hand_size-1);

  return player_->PullCard(max_index);
}

/**
 * @brief Plays the maximum valued card which results in the minimum number of points.
 *        This is expected to be ran only when the player is the last to play in a trick.
 *
 * @return The value of the best card.
 */
int GreedyComController::PlayMinPointsMaxCard() {
  int *hand = player_->GetHand();
  unsigned char hand_size = player_->GetHandSize();
  int itt;

  char max_index = -1;
  char max_value = -1;

  // Tries to find a card in the same suit below the high card.
  for (itt=0; hand[itt] < high_card_ && itt<hand_size; itt++) {
    if (hand[itt] != 0 && Suit(hand[itt]) == Suit(high_card_)) {
      max_index = itt;
      max_value = Value(hand[itt]);
    }
  }
  //printf("max card normal of %d found at index %d\n",max_value, max_index);
  if (max_index != -1) return player_->PullCard(max_index);
  //printf("cont on\n");
  // If there is no card in the same suit below the high card then we simply play the highest card in the suit.
  // We also only take the queen of spades if it is the only spade we have
  for (; Suit(hand[itt]) == Suit(high_card_) && itt<hand_size; itt++) {
    if (hand[itt] != 0 && max_index == -1 || hand[itt] != Q_SPADES && hand[itt] != 0) {
      max_index = itt;
      max_value = Value(hand[itt]);
    }
  }
  if (max_index != -1) return player_->PullCard(max_index);
  //printf("resorting to playing max card in pmpmc\n");
  // If we have found no card to play at this point, then we do not have any cards in the suit that was led.
  //   Instead, we will simply play the maximum card in our hand.
  return PlayMaxCard();
}

int GreedyComController::PlayNormally() {
  int *hand = player_->GetHand();
  unsigned char hand_size = player_->GetHandSize();
  int itt;

  char max_index = -1;
  char max_value = -1;

  // Tries to find a card in the same suit below the high card.
  for (itt=0; hand[itt] < high_card_ && itt<hand_size; itt++) {
    if (hand[itt] !=0 && Suit(hand[itt]) == Suit(high_card_)) {
      max_index = itt;
      max_value = Value(hand[itt]);
    }
  }
  if (max_index != -1) return player_->PullCard(max_index);

  // If there is no card in the same suit below the high card then we simply play the lowest card we have above the high card.
  // We also only take the queen of spades if it is the only spade we have
  for (; Suit(hand[itt]) == Suit(high_card_) && itt<hand_size; itt++) {
    if (hand[itt] != 0 && max_index == -1 || hand[itt] == Q_SPADES) {
      max_index = itt;
      max_value = Value(hand[itt]);
      if (hand[itt] == Q_SPADES) break;
    }
  }
  if (max_index != -1) return player_->PullCard(max_index);

  // If we have found no card to play at this point, then we do not have any cards in the suit that was led.
  //   Instead, we will simply play the maximum card in our hand.
  return PlayMaxCard();
}
