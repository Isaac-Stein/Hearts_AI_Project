#include "MonteCarloNode.h"
MonteCarloNode::MonteCarloNode(int id, MonteCarloNode *parent) :
  parent_{parent},
  id_{id} {}


void MonteCarloNode::AddSuccess(unsigned long num) {
  success_ += num;
  total_ += num;
  if (parent_ != NULL) {
    parent_->AddSuccess(num);
    Reweigh();
  }
}

void MonteCarloNode::AddNotSuccess(unsigned long num) {
  total_ += num;
  if (parent_ != NULL) {
    parent_->AddNotSuccess(num);
    Reweigh();
  }
  for (int i=0;i<children_.size();i++) children_[i]->Reweigh();
}

void MonteCarloNode::AddSuccessTree(unsigned long num) {
  if (children_.size() > 0) {
    GetMaxChild()->AddSuccessTree(num);
  } else {
    AddSuccess(num);
  }
}

void MonteCarloNode::AddNotSuccessTree(unsigned long num) {
  if (children_.size() > 0) {
    GetMaxChild()->AddNotSuccessTree(num);
  } else {
    AddNotSuccess(num);
  }
}

unsigned long MonteCarloNode::GetSuccess() {return success_;}
unsigned long MonteCarloNode::GetTotal() {return total_;}
double MonteCarloNode::GetWeight() {return weight_;}
int MonteCarloNode::GetId() {return id_;}

double MonteCarloNode::GetChildWeight(int id) {
  for (int i=0; i<children_.size(); i++) {
    if (children_[i]->GetId() == id) return children_[i]->GetWeight();
  }
  children_.push_back(new MonteCarloNode(id, this));
  return DBL_MAX;
}

MonteCarloNode *MonteCarloNode::GetChild(int id) {
  for (int i=0; i<children_.size(); i++) {
    if (children_[i]->GetId() == id) return children_[i];
  }
  return NULL;
}

MonteCarloNode *MonteCarloNode::GetMaxChild() {
  double top_weight = -1;
  int top_index = -1;
  for (int i=0; i<children_.size(); i++) {
    if (children_[i]->GetWeight() >= top_weight) {
      top_weight = children_[i]->GetWeight();
      if (top_weight == DBL_MAX) return children_[i];
      else top_index = i;
    }
  }
  if (top_index != -1) {
    return children_[top_index];
  } else {
    children_.push_back(new MonteCarloNode(-1, this));
    return children_[children_.size()-1];
  }
}

void MonteCarloNode::Reweigh() {
  if (total_ == 0) return;
  if (parent_ != NULL) {
    weight_ = ((double)success_/total_) + sqrt((2 * log(parent_->GetTotal()))/total_);
  }
}

void MonteCarloNode::ReweighTree() {
  if (total_ == 0) return;
  if (parent_ != NULL) {
    weight_ = ((double)success_/total_) + sqrt((2 * log(parent_->GetTotal()))/total_);
  }
  for (int i=0;i<children_.size();i++) children_[i]->ReweighTree();
}

/**
 * @brief Merges the contents of another MCT into this MCT.
 *        The merged tree will need to be reweighed.
 *
 * @param A pointer to the other MTC.
 */
void MonteCarloNode::Merge(MonteCarloNode *other) {
  // Merging the success, totals of the current node.
  success_ += other->GetSuccess();
  total_ += other->GetTotal();

  // Merges every child of the MCT.
  std::vector<MonteCarloNode *> other_children = other->GetChildren();
  int num_children = children_.size();
  int num_other_children = other_children.size();
  bool share_child;

  for (int j=0; j<num_other_children; j++) {
    share_child = false;
    for (int i=0; i<num_children; i++) {
      // Both trees branch to the same child id, merge the two.
      if (children_[i]->GetId() == other_children[j]->GetId()) {
        children_[i]->Merge(other_children[j]);
        share_child = true;
      }
    }
    // If the id of the other child is not in this tree, add it, then merge the two.
    if (!share_child) {
      children_.push_back(new MonteCarloNode(other_children[j]->GetId(), this));
        children_[children_.size()-1]->Merge(other_children[j]);
    }
  }
}

void MonteCarloNode::Delete() {
  int num_child = children_.size();
  for (int i=0;i<num_child;i++) {
    children_[i]->Delete();
    delete children_[i];
  }
}

void MonteCarloNode::Print(int num_spaces) {
  for (int i=0;i<num_spaces;i++)std::cout << " ";
  std::cout << id_ << ": (" << success_ <<"/"<<total_<<") : ";
  if (weight_ == DBL_MAX) std::cout << "inf" <<std::endl;
  else std::cout<<weight_<<std::endl;
  for (int i=0;i<children_.size();i++) {
    children_[i]->Print(num_spaces+2);
  }
}
