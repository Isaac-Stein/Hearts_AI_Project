#include "MonteCarloController.h"


void MonteCarloComController::Print() {
  tree_head_->Print();
}

/**
 * @brief Computer controller chooses which action to undertake.
 *        Choice of card to play is determined by a Monte Carlo decision tree.
 *
 * @return The value of the card that the player decided to play.
 */
int MonteCarloComController::Act() {
  if (!ran_simulation_) SimulateGame();

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

void MonteCarloComController::SimulateGame() {
  //printf("here1\n");
  Hearts *sim = new Hearts;
  std::vector<Player *> players;
  //printf("here2\n");
  players.push_back(new Player(new RoundBasedSimulatedComController(), "", sim));
  players.push_back(new Player(new GreedyComController(), "", sim));
  players.push_back(new Player(new GreedyComController(), "", sim));
  players.push_back(new Player(new GreedyComController(), "", sim));
  sim -> Setup(players);
  sim->SetMoonType(TYPE_NONE);
  //printf("here3\n");

  int *alphabet = (int *)malloc(52*sizeof(int));
  for (int i=0;i<52;i++) alphabet[i]=(i+1);
  for (int i=0;i<13;i++) alphabet[full_hand_[i]-1] = 0;
  //printf("here4\n");
  /*int *destroyed_hand = players_[0]->GetHand();
  int *hand = (int *)malloc(13 * sizeof(int));
  for (int i=0;i<13; i++) hand[i] = destroyed_hand[i];
  */

  int num_simulations = 100;
  int rounds_per_sim = 100;
  time_t round_seed;
  //printf("here5\n");
  RoundBasedSimulatedComController *sim_controller = (RoundBasedSimulatedComController *)players[0]->GetController();
  //printf("here6\n");

  for (int p=0;p<num_simulations; p++) {
    round_seed = time(0);
    //  printf("here7 - %d\n",p);
    for (int r=0; r<rounds_per_sim; r++) {
      //printf("here8 - %d\n",r);
      srand(round_seed);
      for (int p=0;p<4;p++)players[p]->Reset();
      players[0]->SetHand(full_hand_);
      //printf("here9\n");
      //for (int i=0;i<52;i++) if (alphabet[i]==0) printf("X"); else printf("O");
      //printf("\n");
      //for (int i=0; i<players.size(); i++) {
      //  std::cout << players[i]->GetName() << ": ";
      //  players[i]->PrintHand();
      //}
      sim->RandomlyDeal(alphabet);
      //printf("here10\n");
      sim->SetupToContinueRound();
      //printf("here11\n");
      sim->TestAuxAct();
      //printf("here12\n");
    }
    tree_head_->Merge(sim_controller->GetTreeHead());
    sim_controller->ResetTree();
  }
  tree_head_->ReweighTree();
  ran_simulation_ = true;

  for (int i=0;i<4;i++) delete players[i];
  delete sim;
  free(alphabet);
}

void MonteCarloComController::SeeLaid(unsigned char card) {
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

void MonteCarloComController::SeeAddedToHand(unsigned char card) {
  if (full_hand_ == NULL) {
    full_hand_ = (int *)malloc(13 * sizeof(int));
    for (int i=0;i<13;i++)full_hand_[i]=0;
  }
  for (int i=0;i<13;i++) if (full_hand_[i] == 0) {
    full_hand_[i] = card;
    break;
  }

  for(int i=0;i<4;i++) possible_cards_[i][card] = false;
}

void MonteCarloComController::Reset() {
  curr_trick_ = 0;
  curr_turn_ = 0;
  round_points_ = 0;
  ResetTree();
  ran_simulation_ = false;

  // Reset the possible cards each player can recieve.
  for (int i=0;i<4;i++) {
    for (int j=0;j<52;j++) possible_cards_[i][j] = true;
  }
}

void MonteCarloComController::ResetTree() {
  tree_head_->Delete();
  delete tree_head_;
  tree_head_ = new MonteCarloNode();
  current_node_ = tree_head_;
}
