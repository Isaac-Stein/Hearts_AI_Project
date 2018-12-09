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
#include "RoundBasedSimulatedController.h"

int main ( int argc, char** argv ) {
  srand(time(NULL));
  Hearts *game = new Hearts;
  std::vector<Player *> players_;
  players_.push_back(new Player(new MonteCarloComController(), "Monte", game));
  players_.push_back(new Player(new HumanController(), "Human", game));
  players_.push_back(new Player(new RandomComController(), "Rand", game));
  players_.push_back(new Player(new GreedyComController(), "Greed", game));
  //game->SetState(SINGLE_ROUND);
  game -> Setup(players_);
  //game -> Test(100);
  game->Act();

  return 0;
}
