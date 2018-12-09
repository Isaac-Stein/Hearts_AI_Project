#ifndef _ROUND_BASED_SIMULATED_CONTROLLER_
  #define _ROUND_BASED_SIMULATED_CONTROLLER_

#include "Controller.h"
#include "MonteCarloNode.h"
#include "Hearts.h"

class RoundBasedSimulatedComController : public Controller {
  public:
    RoundBasedSimulatedComController() : Controller() {
      tree_head_ = new MonteCarloNode();
      current_node_ = tree_head_;
      possible_cards_ = (bool **)malloc(4 * sizeof(bool *));
      for (int i=0;i<4;i++) {
          possible_cards_[i] = (bool *)malloc(52*sizeof(bool));
          for (int j=0;j<52;j++) possible_cards_[i][j] = true;
      }
    }
    //RoundBasedSimulatedController(Player *player) {Controller(player);}
    int Act() override;
    void SeeLaid(unsigned char card) override;
    void SeeAddedToHand(unsigned char card) override;
    void Reset() override;
    void ResetTree();


    MonteCarloNode *GetTreeHead() { return tree_head_; }

    void Print() override;

  private:
    unsigned char curr_turn_{0};
    unsigned char curr_trick_{0};
    int round_points_{0};
    bool queen_played_{false};
    MonteCarloNode *tree_head_{NULL};
    MonteCarloNode *current_node_{NULL};
    bool **possible_cards_{NULL};
};
#endif // _ROUND_BASED_SIMULATED_CONTROLLER_


