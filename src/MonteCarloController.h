#ifndef _MONTE_CARLO_CONTROLLER_
  #define _MONTE_CARLO_CONTROLLER_

#include "Controller.h"
#include "MonteCarloNode.h"

class MonteCarloComController : public Controller {
  public:
    MonteCarloComController() : Controller() {
      tree_head_ = new MonteCarloNode();
      current_node_ = tree_head_;
      possible_cards_ = (bool **)malloc(4 * sizeof(bool *));
      for (int i=0;i<4;i++) {
          possible_cards_[i] = (bool *)malloc(52*sizeof(bool));
          for (int j=0;j<52;j++) possible_cards_[i][j] = true;
      }
    }
    //MonteCarloController(Player *player) {Controller(player);}
    int Act() override;
    void SeeLaid(unsigned char card) override;
    void SeeAddedToHand(unsigned char card) override;
    void Reset() override;

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
#endif // _MONTE_CARLO_CONTROLLER_

