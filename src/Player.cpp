#include "Player.h"

Player::Player(Controller *controller, std::string name, Hearts *game, unsigned char hand_size) :
  controller_{controller},
  name_{name},
  game_{game},
  hand_size_{hand_size} {
  hand_ = (int *) malloc(hand_size_ * sizeof(int));
  ClearHand();

  controller_ = controller;
  controller_->SetPlayer(this);
}

void Player::AddPoints(int num) { current_points_ += num; }

void Player::Reset() {
  controller_->Reset();
  for(int i=0; i<hand_size_; i++) hand_[i]=0;
  total_points_ = 0;
  current_points_ = 0;
  leads_ = false;
}

void Player::ResetController() {
  controller_->Reset();
}

unsigned char Player::GetCurrentPoints() { return current_points_; }

unsigned char Player::GetTotalPoints() { return total_points_; }

std::string Player::GetName() { return name_; }

/**
 * @brief Adds a card to the player's hand.
 *
 * @param The card to add.
 *
 * @return 0 if successful, the value of the card if some failure occurred
 */
unsigned char Player::AddToHand(unsigned char card) {
  if (HasFullHand()) return card;
  for (int i=0; i<hand_size_; i++) {
    if (hand_[i] == 0) {
      hand_[i] = card;
      controller_->SeeAddedToHand(card);
      return 0;
    }
  }
  std::cout << "Error: You should not reach this in AddToHand!" << std::endl;
  return card;
}

/**
 * @brief Pulls a card from the players hand at some index, leaves 0 at that index.
 *
 * @param The index to pull from.
 *
 * @return 0 if index is out of bounds, the value of the card otherwise.
 */
unsigned char Player::PullCard(unsigned char index) {
  if (index <0 || index >= hand_size_) return 0;
  unsigned char card = hand_[index];
  hand_[index] = 0;
  return card;
}

/**
 * @brief Passes the information of a card that has been played to the controller.
 *
 * @param The value of the card that was played.
 */
void Player::SeeLaid(unsigned char card) {
  controller_->SeeLaid(card);
}

/**
 * @brief Checks if playing a cards at an index is valid.
 *
 * @param The index of the card to play.
 *
 * @return Whether the play of the card at that index would be valid.
 */
bool Player::IsValidPlay(unsigned char index) {
  // Choice is index in hand.
  if (index >= hand_size_) return false;
  unsigned char card = hand_[index];

  // Choice is an index that still exists.
  if (card == 0) return false;

  return game_->IsValidPlay(card, this);
}

/**
 * @brief Returns if the player has a card in the suit parameter.
 *        0 - Clubs
 *        1 - Diamonds
 *        2 - Spades
 *        3 - Hearts
 *
 * @param The suit to check for.
 *
 * @return Whether the player has a card in that suit.
 */
bool Player::HasCardInSuit(unsigned char suit) {
  for (int i=0; i<hand_size_; i++) {
    if (hand_[i] == 0) continue;
    if ((hand_[i]-1)/13 == suit) return true;
  }
  return false;
}

/**
 * @brief Checks if the player has a card in their hand which is not a heart.
 *
 * @return true if player has a non-heart card, false otherwise
 */
bool Player::HasNonHeart() {
  for (int i=0; i<hand_size_; i++) {
    if (hand_[i] == 0) continue;
    if ((hand_[i]-1)/13 != 3) return true;
  }
  return false;
}

/**
 * @brief Returns if the player's hand contains any cards which are not worth any points.
 *
 * @return Whether the hand has a card which is not a point card.
 */
bool Player::HasNonPointCard() {
  for (int i=0; i<hand_size_; i++) {
    if (hand_[i] == 0) continue;
    // Choice cannot be heart or card 37, the queen of Spades.
    if ((hand_[i]-1)/13 != 3 && hand_[i] != Q_SPADES) return true;
  }
  return false;
}

/**
 * @brief Prints the contents of the player's hand.
 *        Primarily for testing purposes.
 */
void Player::PrintHand() {
  std::cout << "Cards:";
  for (unsigned char i=0; i<hand_size_; i++) {
    PrintCard(hand_[i]);
    std::cout << " ";
  }
  std::cout << std::endl;
}

void Player::PrintController() {
  controller_->Print();
}

/**
 * @brief The player acts based on what their controller tells them to do.
 */
int Player::Act() { int choice = controller_->Act(); }

bool Player::GetHeartsBroken() { return game_->GetHeartsBroken(); }
