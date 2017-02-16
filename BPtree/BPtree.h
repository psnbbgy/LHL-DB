#include "node.h"

#ifndef _BP_TREE_H_
#define _BP_TREE_H_
namespace BP {

class BPTree {
 public:
  BPTree() {
    root_ = new leaf_node();
    leaf_link_ = dynamic_cast<leaf_node*>(root_);
    if (leaf_link_ == NULL) {
      //log error
    }
  }
  bool Insert(KEY_TYPE key, VALUE_TYPE value); //insert records into B+ tree

  inline int SearchKey(node* cur_node, KEY_TYPE key); // search keys in nodes

  int Get(KEY_TYPE key, VALUE_TYPE* value);  //get value by key, TODO:use smart pointer instead of normal pointer
 private:
  node* root_;
  leaf_node* leaf_link_;     //link list to connect leaf node
}
}
#endif
