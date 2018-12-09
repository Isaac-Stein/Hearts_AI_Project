#ifndef _CONTROLLER_H_
  #define _CONTROLLER_H_

#include <iostream>

#include "Player.h"
#include "Globals.h"

class Player;

class Controller {
public:

  void SetPlayer(Player *player) {player_ = player; }
  Controller() {}
  Controller(Player *player) {player_ = player;}

  virtual void Print() {}
  virtual int Act() { return 0; }
  virtual void SeeLaid(unsigned char card) {}
  virtual void SeeAddedToHand(unsigned char card) {}
  virtual void Reset() {}

protected:
  Player *player_;

};
#endif // _CONTROLLER_H_
