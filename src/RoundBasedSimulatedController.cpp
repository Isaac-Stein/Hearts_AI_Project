
#include "RoundBasedSimulatedController.h"


void RoundBasedSimulatedComController::Print() {
  tree_head_->Print();
}

/**
 * @brief Computer controller chooses which action to undertake.
 *        Choice of card to play is determined by a Monte Carlo decision tree.
 *
 * @return The value of the card that the player decided to play.
 */
int RoundBasedSimulatedComController::Act() {
  int *hand = player_->GetHand();
  //player_->PrintHand();
  double weights[13];
  int maximums[13];
  for (int i=0;i<13;i++) maximums[i]=-1;
  int m_index=0;
  int max_index = -1;
  for (int i=0; i<13; i++) {
    if (player_->IsValidPlay(i)) {
      weights[i] = current_node_->GetChildWeight(hand[i]);
      if (weights[i] == DBL_MAX) {
        maximums[m_index++] = i;
      } else if (max_index==-1 || weights[i] > weights[max_index]) {
        max_index = i;
      }
    }
  }
  if (m_index > 0) {
    int random_index = rand() % (m_index);
    current_node_ = current_node_->GetChild(hand[maximums[random_index]]);
    int val = player_->PullCard(maximums[random_index]);
    return val;
  }
  current_node_ = current_node_->GetChild(hand[max_index]);
  return player_->PullCard(max_index);
}

void RoundBasedSimulatedComController::SeeLaid(unsigned char card) {
  for(int i=0;i<4;i++) possible_cards_[i][card] = false;

  if (card == Q_SPADES) {
    queen_played_ = true;
    round_points_ += 13;
  }
  if (card > A_SPADES) round_points_++;
  curr_turn_ = (curr_turn_+1)%4;
  if (curr_turn_ == 0) {
      curr_trick_++;
  }
}

void RoundBasedSimulatedComController::SeeAddedToHand(unsigned char card) {
  for(int i=0;i<4;i++) possible_cards_[i][card] = false;
}

void RoundBasedSimulatedComController::Reset()   {
  current_node_->AddSuccess(26-player_->GetCurrentPoints());
  current_node_->AddNotSuccess(player_->GetCurrentPoints());
  curr_trick_ = 0;
  curr_turn_ = 0;
  round_points_ = 0;
  current_node_ = tree_head_;

  // Reset the possible cards each player can recieve.
  for (int i=0;i<4;i++) {
    for (int j=0;j<52;j++) possible_cards_[i][j] = true;
  }
}


void RoundBasedSimulatedComController::ResetTree() {
  tree_head_->Delete();
  delete tree_head_;
  tree_head_ = new MonteCarloNode();
  current_node_ = tree_head_;
}
