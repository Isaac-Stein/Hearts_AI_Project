#ifndef _HUMAN_CONTROLLER_
  #define _HUMAN_CONTROLLER_

#include "Controller.h"

class HumanController : public Controller {
  public:
  HumanController() : Controller() {}
//  HumanController(Player *player) {Controller(player);}
  int Act() override;
};

#endif // _HUMAN_CONTROLLER_
