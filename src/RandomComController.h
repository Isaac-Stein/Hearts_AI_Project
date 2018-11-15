#ifndef _RANDOMCOMCONTROLLER_
  #define _RANDOMCOMCONTROLLER_

#include "Controller.h"

class RandomComController : public Controller {
  public:
  RandomComController() : Controller() {}
  //RandomComController(Player *player) {Controller(player);}
  int Act() override;
};
#endif // _RANDOMCOMCONTROLLER_
