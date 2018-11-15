#ifndef _GLOBALS_H_
  #define _GLOBALS_H_

static bool SameSuit() {


}

enum HeartsSimulationState{
  NORMAL,
  SINGLE_ROUND
};

static int Value(int card) {
  return ((card-1)%13);
}

static int Suit(int card) {
  return ((card-1)/13);
}

/**
 * @brief Takes in some integer and prints a card representation of that integer.
 *
 * @param The value to print the representation of.
 */
static void PrintCard(unsigned char num) {
  if (num==0) {
    std::cout << " XX";
  } else {
    switch ((num-1)%13) {
      case (8): {
        std::cout << "10";
        break;
      }
      case (9): {
        std::cout << " J";
        break;
      }
      case (10): {
        std::cout << " Q";
        break;
      }
      case (11): {
        std::cout << " K";
        break;
      }
      case (12): {
        std::cout << " A";
        break;
      }
      default: {
        std::cout << " " << ((num-1)%13+2);
      }
    }
    switch ((num-1)/13) {
      case (0): {
        std::cout << "♣";
        break;
      }
      case (1): {
        std::cout << "♦";
        break;
      }
      case (2): {
        std::cout << "♠";
        break;
      }
      case (3): {
        std::cout << "♥";
        break;
      }
      default: {}
    }
  }
}

#define Q_SPADES 37
#define K_SPADES 38
#define A_SPADES 39
#define A_HEARTS 52

#endif // _GLOBALS_H_
