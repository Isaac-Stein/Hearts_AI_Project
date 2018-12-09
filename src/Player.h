#ifndef _PLAYER_H_
  #define _PLAYER_H_

#include "stdlib.h"

#include "Controller.h"
#include "Globals.h"
#include "Hearts.h"

class Hearts;

class Controller;

class Player {
public:
  Player(Controller *controller, std::string name, Hearts *game, unsigned char hand_size = 13);

void Reset();
void ResetController();

void AddPoints(int num);
void ClearCurrentPoints() { current_points_ = 0; }
unsigned char GetCurrentPoints();
unsigned char GetTotalPoints();

std::string GetName();

bool HasFullHand() {
  return hand_[hand_size_-1] != 0;
  }

void PrintHand();
void SetHand(int *hand);

void PrintController();

unsigned char AddToHand(unsigned char card);

unsigned char PullCard(unsigned char index);

int *GetHand() { return hand_; }
unsigned char GetHandSize() { return hand_size_; }

int Act();

bool HasCardInSuit(unsigned char suit);
bool HasNonHeart();
bool HasNonPointCard();

bool GetHeartsBroken();

Controller *GetController() { return controller_; }

bool IsLead() { return leads_; }
void SetLead(bool lead) { leads_ = lead; }

void AssignFirstLead() { leads_ = (hand_[0] == 1);}

bool HasLost() { return (total_points_ >= 100); }

bool IsValidPlay(unsigned char index);

void SeeLaid(unsigned char card);

bool FinishRound() {
  total_points_ += current_points_;
  current_points_ = 0;
  return HasLost();
}

protected:
  void ClearHand() {
    for (int i=0; i<hand_size_; i++) hand_[i] = 0;
  }

protected:

  std::string name_;

  int *hand_;
  unsigned char hand_size_;

  unsigned char total_points_{0};
  unsigned char current_points_{0};

  bool leads_{false};

  Controller *controller_;
  Hearts *game_;

};

#endif // _PLAYER_H_
