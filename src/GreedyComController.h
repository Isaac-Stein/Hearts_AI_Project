#ifndef _GREEDYCOMCONTROLLER_
  #define _GREEDYCOMCONTROLLER_

#include "Controller.h"

class GreedyComController : public Controller {
  public:
    GreedyComController() : Controller() {}
    //GreedyComController(Player *player) {Controller(player);}
    int Act() override;
    void SeeLaid(unsigned char card) override;
    void Reset() override;

  private:
    int PlayMaxFirstTrick();
    int PlayMinCard();
    int PlayMaxCard();
    int PlayMinPointsMaxCard();
    int PlayMaxCardUnderHighCard();
    int PlayNormally();



  private:
    unsigned char curr_turn_{0};
    unsigned char curr_trick_{0};
    unsigned char laid_cards_[4];
    int trick_points_{0};
    unsigned char high_card_{0};
    bool queen_played_{false};
};
#endif // _GREEDYCOMCONTROLLER_



