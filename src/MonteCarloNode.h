#ifndef _MONTE_CARLO_NODE_H_
  #define _MONTE_CARLO_NODE_H_

#include <cfloat>
#include <vector>
#include <math.h>
#include <iostream>

class MonteCarloNode{
public:
  MonteCarloNode(int id = -1, MonteCarloNode *parent = NULL);

  void AddSuccess(unsigned long num);
  void AddNotSuccess(unsigned long num);
  void AddSuccessTree(unsigned long num);
  void AddNotSuccessTree(unsigned long num);
  unsigned long GetSuccess();
  unsigned long GetTotal();
  double GetWeight();
  int GetId();
  std::vector<MonteCarloNode *> GetChildren() { return children_; }
  double GetChildWeight(int id);
  MonteCarloNode *GetChild(int id);
  MonteCarloNode *GetMaxChild();

  void Merge(MonteCarloNode *other);

  void Print(int num_spaces=0);
  void Delete();

  void ReweighTree();

private:
  void Reweigh();

private:
  unsigned long success_{0};
  unsigned long total_{0};
  double weight_{DBL_MAX};
  int id_;

  MonteCarloNode *parent_{NULL};
  std::vector<MonteCarloNode *> children_;

};

#endif // _MONTE_CARLO_NODE_H_
