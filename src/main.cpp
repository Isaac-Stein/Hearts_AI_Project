#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <vector>

#include "Hearts.h"
#include "Player.h"
#include "Globals.h"
#include "RandomComController.h"
#include "GreedyComController.h"
#include "MonteCarloController.h"
#include "HumanController.h"

int main ( int argc, char** argv ) {
  srand(time(NULL));
  Hearts *game = new Hearts;
  std::vector<Player *> players_;
  players_.push_back(new Player(new HumanController(), "Monte", game));
  players_.push_back(new Player(new GreedyComController(), "G1", game));
  players_.push_back(new Player(new GreedyComController(), "G2", game));
  players_.push_back(new Player(new GreedyComController(), "G3", game));
  //game->SetState(SINGLE_ROUND);
  game -> Setup(players_);
  //game -> Test(10000000);
  game->Act();
  return 0;
}
