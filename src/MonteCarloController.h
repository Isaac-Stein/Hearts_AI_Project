#ifndef _MONTE_CARLO_CONTROLLER_
  #define _MONTE_CARLO_CONTROLLER_

#include "Controller.h"
#include "MonteCarloNode.h"

class MonteCarloComController : public Controller {
  public:
    MonteCarloComController() : Controller() {
      tree_head_ = new MonteCarloNode();
      current_node_ = tree_head_;
    }
    //MonteCarloController(Player *player) {Controller(player);}
    int Act() override;
    void SeeLaid(unsigned char card) override;
    void Reset() override;

  private:
    unsigned char curr_turn_{0};
    unsigned char curr_trick_{0};
    int round_points_{0};
    bool queen_played_{false};
    MonteCarloNode *tree_head_{NULL};
    MonteCarloNode *current_node_{NULL};
};
#endif // _MONTE_CARLO_CONTROLLER_

