#ifndef _HEARTS_H_
  #define _HEARTS_H_

#include <vector>
#include "Player.h"
#include <iostream>
#include <fstream>

enum GameStates{
  TYPE_ROUND_START,
  TYPE_ROUND_MIDDLE,
  TYPE_ROUND_RESOLUTION
};

class Player;

class Hearts {
public:

  void Test(int num_rounds);
  void Simulate(int num_rounds, bool **possible_cards);
  void TestAuxAct(unsigned long *total_scores = NULL);
  void SetupToContinueRound();

  void Setup();
  void Setup(std::vector<Player *> new_players);
  void Reset();
  void Reset(std::vector<Player *> new_players);
  void Act();
  Player *GetLead();
  int GetLeadCard() {return lead_card_;}
  bool IsFirstTrick() {return is_first_trick_;}
  bool GetHeartsBroken() {return hearts_broken_;}

  bool RandomlyDeal(int length=52);
  bool RandomlyDeal(int *deck, int length=52);
  bool RandomlyDeal(bool **allowed, int length=52);

  void AssignFirstLead();

  void AssignPoints();
  void AdjustPoints();

  void SetState(HeartsSimulationState state) { sim_state_ = state; }
  void SetMoonType(MoonType type) {moon_type_ = type;}

  bool IsValidPlay(int card, Player *player);

private:
  GameStates game_state_{TYPE_ROUND_START};
  HeartsSimulationState sim_state_{NORMAL};
  std::vector<Player *> players_;
  int *laid_cards_;
  int lead_card_{0};

  unsigned char num_players_{4};
  bool hearts_broken_{false};
  bool is_first_trick_{true};
  unsigned int tricks_played_{0};
  MoonType moon_type_{TYPE_OLD_MOON};
};
#endif // _HEARTS_H_
