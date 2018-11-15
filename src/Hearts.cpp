#include "Hearts.h"
#include "RandomComController.h"
#include "HumanController.h"

/**
 * @brief Sets up all necessary values for the game.
 */
void Hearts::Setup() {
  players_.push_back(new Player(new HumanController, "HUMAN", this));
  players_.push_back(new Player(new RandomComController(), "COM_WEST", this));
  players_.push_back(new Player(new RandomComController(), "COM_NORTH", this));
  players_.push_back(new Player(new RandomComController(), "COM_EAST", this));
  laid_cards_ = (int *) malloc(num_players_ * sizeof(int));
}

/**
 * @brief Sets up all necessary values for the game.
 *
 * @param The vector of players to play the game.
 */
void Hearts::Setup(std::vector<Player *> new_players) {
  players_ = new_players;
  laid_cards_ = (int *) malloc(num_players_ * sizeof(int));
}

/**
 * @brief Assigns to points gained so far in the tricks to each of the players.
 */
void Hearts::AssignPonts() {
  for (int i=0; i < players_.size(); i++) {
    // Rules for point distributions when the moon has been shot.
    if (players_[i]->GetCurrentPoints() == 26) {
      for (int j=0; j<players_.size(); j++) {
        if (j!=i) {
          switch(moon_type_){
            case(TYPE_OLD_MOON): {
              players_[j]->AddPoints(26);
              players_[j]->FinishRound();
              break;
            }
            case(TYPE_NEW_MOON):{}
            default:{
              players_[j]->FinishRound();
            }
          }
        } else {
          switch(moon_type_){
            case(TYPE_OLD_MOON): {
              players_[j]->AddPoints(-26);
              players_[j]->FinishRound();
              break;
            }
            case(TYPE_NEW_MOON):{
              players_[j]->AddPoints(-52);
              players_[j]->FinishRound();
              break;
            }
            default:{
              players_[j]->FinishRound();
            }
          }
        }
      }
    } else {
      // Standard point distribution rules.
      players_[i]->FinishRound();
    }
  }
}

/**
 * @brief The game of Hearts undertakes the next action in a game.
 *
 * @todo Break this up into separate functions.
 */
void Hearts::Act() {
  bool game_over_ = false;
  while(!game_over_) {
    switch(game_state_ ) {
    // Setup for the start of a game.
    case(TYPE_ROUND_START) : {
      RandomlyDeal();
      AssignFirstLead();
      game_state_ = TYPE_ROUND_MIDDLE;
      hearts_broken_ = false;
      is_first_trick_ = true;
      tricks_played_ = 0;
      break;
    }

    case(TYPE_ROUND_MIDDLE) : {
      // Printing out the current scores of each player.
      // Strange syntax in printing points due to some unknown error in not printing anything.
      for (int i=0; i<players_.size(); i++) {
        std::cout << players_[i]->GetName() << " : (" << (players_[i]->GetCurrentPoints()+0) << "/" << (players_[i]->GetTotalPoints()+0) << ")" << std::endl;
      }
      for (int i=0; i<players_.size(); i++) {
        //std::cout << players_[i]->GetName() << ": ";
        //players_[i]->PrintHand();
      }

      tricks_played_++;
      /********************/// trash for humans
      std::cout << "Begin round " << tricks_played_ << std::endl;

      {
        std::string trash;
        std::cin >> trash;
      }
      /*********************/

      // Getting the lead player.
      Player *lead = GetLead();

      // Checking to make sure that the lead player is valid.
      if (lead == NULL) {
        printf("Failure in finding lead player.\n");
        if (players_.size() > 0) {
            lead = players_[0];
        } else {
          printf("No players exist!! Aborting!!\n");
          return;
        }
      }

      // Setting i to the index of the lead player.
      int i;
      for (i=0; i<players_.size(); i++) {
        if (players_[i] == lead) break;
      }
      int lead_number = i;
      int trick_score = 0;
      lead_card_ = 0;
      int high_index = i;
      // Having each player take their turn in order.
      for (; i<lead_number+players_.size(); i++) {
        // The player takes their turn.
        laid_cards_[i%players_.size()] = players_[i%players_.size()]->Act();
        // All players are informed of the play.
        for (int p=0; p<players_.size(); p++) {
            players_[p]->SeeLaid(laid_cards_[i%players_.size()]);
        }
        std::cout << "Player " << players_[i%players_.size()]->GetName() << " played card ";
        PrintCard(laid_cards_[i%players_.size()]);
        printf("\n");

        if ((laid_cards_[i%players_.size()]-1)/13 == 3) trick_score++;
        else if (laid_cards_[i%players_.size()] == Q_SPADES) trick_score += 13;
        if ((laid_cards_[i%players_.size()]-1)/13 == (laid_cards_[high_index]-1)/13 &&
            (laid_cards_[i%players_.size()]-1)%13 > (laid_cards_[high_index]-1)%13) {

              high_index = i%players_.size();
        }
        if (lead_card_==0) lead_card_ = laid_cards_[i];
      }

      // Setting next lead, giving points to trick winner.
      if (!hearts_broken_ && trick_score>0) hearts_broken_ = true;
      for (i=0; i<players_.size(); i++) {
        if (i == high_index) {
          players_[i]->AddPoints(trick_score);
          players_[i]->SetLead(true);
          std::cout << "\n\n Player " << players_[i]->GetName() << " got that trick with " << trick_score << " points!\n\n";
        } else {
          players_[i]->SetLead(false);
        }
      }

      is_first_trick_ = false;
      if (tricks_played_ == 13) game_state_ = TYPE_ROUND_RESOLUTION;
    break;
    }
    case(TYPE_ROUND_RESOLUTION) : {
      if (sim_state_ == NORMAL) AssignPonts();
      for (int p=0;p<players_.size();p++) players_[p]->ResetController();
      game_state_ = TYPE_ROUND_START;
      break;
    }
    default: { printf("This should not appear in Hears::Act\n"); }
    }
  }
}

/**
 * @brief Testing and getting the scores of a single round of Hearts.
 */
void Hearts::TestAuxAct(unsigned long *total_scores) {
  bool round_over_ = false;
  while(!round_over_) {
    switch(game_state_ ) {
    // Setup for the start of a game.
    case(TYPE_ROUND_START) : {
      RandomlyDeal();
      AssignFirstLead();
      game_state_ = TYPE_ROUND_MIDDLE;
      hearts_broken_ = false;
      is_first_trick_ = true;
      tricks_played_ = 0;
      break;
    }

    case(TYPE_ROUND_MIDDLE) : {
      tricks_played_++;

      // Getting the lead player.
      Player *lead = GetLead();

      // Setting i to the index of the lead player.
      int i;
      for (i=0; i<players_.size(); i++) {
        if (players_[i] == lead) break;
      }

      int lead_number = i;
      int trick_score = 0;
      lead_card_ = 0;
      int high_index = i;
      // Having each player take their turn in order.
      for (; i<lead_number+players_.size(); i++) {
        // The player takes their turn.
        laid_cards_[i%players_.size()] = players_[i%players_.size()]->Act();

        // All players are informed of the play.
        for (int p=0; p<players_.size(); p++) players_[p]->SeeLaid(laid_cards_[i%players_.size()]);

        // Increases the value of the trick if the card is a points card
        if ((laid_cards_[i%players_.size()]-1)/13 == 3) trick_score++;
        else if (laid_cards_[i%players_.size()] == Q_SPADES) trick_score += 13;

        // If the laid card is higher than the current high card then the laid card is the new high card.
        if ((laid_cards_[i%players_.size()]-1)/13 == (laid_cards_[high_index]-1)/13 &&
            (laid_cards_[i%players_.size()]-1)%13 > (laid_cards_[high_index]-1)%13) {
              high_index = i%players_.size();
        }
        // If this is the first trick then the lead card is the currently laid card.
        if (lead_card_==0) lead_card_ = laid_cards_[i];
      }

      // Setting next lead, giving points to trick winner.
      if (!hearts_broken_ && trick_score>0) hearts_broken_ = true;
      for (i=0; i<players_.size(); i++) {
        if (i == high_index) {
          players_[i]->AddPoints(trick_score);
          players_[i]->SetLead(true);
        } else {
          players_[i]->SetLead(false);
        }
      }

      is_first_trick_ = false;
      if (tricks_played_ == 13) game_state_ = TYPE_ROUND_RESOLUTION;
    break;
    }
    case(TYPE_ROUND_RESOLUTION) : {
      for (int i=0;i<num_players_;i++) {
          //std::cout << "ponts of " << i << ": " << players_[i]->GetCurrentPoints()+0 << std::endl;
          total_scores[i] += players_[i]->GetCurrentPoints();
          players_[i]->ClearCurrentPoints();
          players_[i]->ResetController();
      }
      game_state_ = TYPE_ROUND_START;
      round_over_ = true;
      break;
    }
    default: { printf("This should not appear in Hears::TestAuxAct\n"); }
    }
  }
}

void Hearts::Test(int num_rounds) {
  unsigned long *total_scores = (unsigned long *)malloc(num_players_ * sizeof(unsigned long));
  double *average_scores = (double *)malloc(num_players_ * sizeof(double));
  double sum_averages = 0;
  int report_period = 100000;
  int num_periods = num_rounds/report_period;
  std::ofstream reportfile;
  reportfile.open("./report.csv");
  for (int i=0;i<num_players_;i++) {
    reportfile << (players_[i]->GetName());
    if (i==num_players_-1) reportfile << "\n";
    else reportfile << ", ";
  }

  for (int p=0;p<num_periods; p++) {
    printf("Begin period %d\n",p);
    for (int i=0; i<num_players_; i++) {
        total_scores[i] = 0;
        players_[i]->Reset();
    }
    for (int i=0; i<report_period; i++) {
      //printf("begin round %d\n",i);
      TestAuxAct(total_scores);
      //printf("end round %d\n",i);
    }
    for (int i=0; i<num_players_; i++) {
      std::cout << "Player " << players_[i]->GetName() << " Total Score: " << total_scores[i] << std::endl;
      average_scores[i] = ((double)total_scores[i]/report_period);
      reportfile << ((double)total_scores[i]/report_period);
      if (i==num_players_-1) reportfile << "\n";
      else reportfile << ", ";
      sum_averages += average_scores[i];
    }
    for (int i=0; i<num_players_; i++) {
      std::cout << "Player " << players_[i]->GetName() << " Average Score: " << average_scores[i] << std::endl;
    }
    printf("Total of average scores = %lf\n\n",sum_averages);
  }
  reportfile.close();
}

/**
 * @brief Return the player which leads the current trick.
 *        It is assumed that exactly one player among the players leads the trick.
 *
 * @return A pointer to the player which leads the current trick.
 *         In the event of failure to find lead player, return NULL.
 */
Player *Hearts::GetLead() {
  for (unsigned int i=0; i<players_.size(); i++) {
    if (players_[i]->IsLead()) return players_[i];
  }
  return NULL;
}

/**
 * @brief Randomly deals out a deck to the players.
 *        It is assumed that the players_ is populated and able to accept all cards in a deck.
 *
 * @param The deck to deal.
 * @param The number of cards in the deck.
 *
 * @return A bool value of the success of the deal.
 */
bool Hearts::RandomlyDeal(int length) {
  /* For use later with non-standard number of players
    Can be safely ignored for 4 player games */
  // if (length != length / players_.size() * players_.size()) {}
  for (unsigned char i=1; i<=length; i++) {
    while (players_[rand() % players_.size()]->AddToHand(i) == i);
  }
  return true;
}

/**
 * @brief Cycles through every player and has them check if they lead the trick.
 *        It is assumed that only a maximum of one player will find that they lead a trick.
 */
void Hearts::AssignFirstLead() {
  for (int i=0; i<players_.size(); i++) players_[i]->AssignFirstLead();
}

/**
 * @brief Checks if playing a card is a valid move for a player.
 *
 * @param The integer representation of the card to play.
 * @param The player trying to make the play.
 *
 * @return The validity of the play.
 */
bool Hearts::IsValidPlay(int card, Player *player) {
  // Lead is the value of the card which led the trick.
  //   If this is the first play then lead is 0.
  //   Otherwise, this value is the card that the lead player led with.
  int lead = GetLeadCard();

  //printf("your card %d lead card %d\n",card,lead);

  // Each of the following is each of the atomic rules in Hearts that prevents a player from playing a specific card.

  // Choice is invalid if is not 2 of clubs on lead card of first trick.
  if (IsFirstTrick() && lead == 0 && card != 1) return false;

  // Choice is invalid if it is not in lead's suit when player has a card in the same suit.
  if (lead !=0 && (card-1)/13 != (lead-1)/13 && player->HasCardInSuit((lead-1)/13)) return false;

  // Cannot lead hearts if hearts not broken unless the player has no other non-heart cards to play.
  if (lead == 0 && (card-1)/13==3 && !GetHeartsBroken() && player->HasNonHeart()) return false;

  // Choice cannot be a points card in first trick unless there are no other cards to play.
  if (IsFirstTrick() && player->HasNonPointCard() && ((card-1)/13 == 3 || card == Q_SPADES)) return false;

  // Choice must be the queen of Spades if possible in first trick if the player only has points cards.
  if (IsFirstTrick() && player->HasNonHeart() && (card-1)/13 == 3) return false;

  return true;
}
