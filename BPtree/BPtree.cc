#include "BPtree.h"

namespace BP {

inline int BPTree::SearchKey(node* cur_node, KEY_TYPE key) {   //use binary search to search a key in nodes
  int start = 0, end = cur_node->get_slot_num() - 1;
  int middle;
  if (key < cur_node->get_key(0)) {
    return -1;
  }
  while (start != end - 1) {
    middle = (start + end) / 2;
    if (key >= cur_node->get_key(middle)) {
      start = middle;
    } else {
      end = middle;
    }
  }
  return start;
}

int BPTree::SearchAndInstead(node* cur_node, KEY_TYPE key) {   //instead the least key when search
  if (cur_node->get_key(0) > key) {
    cur_node->get_key(0) = key;
    return 0;
  }
  if (key > cur_node->get_max_key()) {
    cur_node->set_max_key(key);
  }
  return SearchKey(cur_node, key);
}

bool BPTree::Insert(KEY_TYPE key, VALUE_TYPE value) {
  node* cur_node = root;
  int index;
  while (cur_node!=NULL && !cur_node->is_leaf()) {      //get the leaf node
    index = SearchAndInstead(cur_node, key);
    cur_node = cur_node->get_value(index);
  }
  if (cur_node == NULL) {
    //TODO:log error
    return false;
  }
  index = SearchKey(cur_node, key);
  VALUE_TYPE* p_value = new VALUE_TYPE(value);   //TODO:when value become complex, change a function
  cur_node->InsertIntoNode(index, key, value);
  while (cur_node->get_slot_num() > MAX_SLOT_NUM) {    //node is full of value
    int split_index;
    node* split_node = cur_node->SplitNode();
    if (cur_node->get_father() != NULL) {
      cur_node = cur_node->get_father();
      split_index = SearchKey(cur_node, split_node->get_key(0));
      cur_node->InsertIntoNode(split_index, split_node->get_key(0), split_node);
    } else {
      root_ = new inner_node();
      root_->InsertIntoNode(cur_node->get_key(0), cur_node);
      root_->InsertIntoNode(split_node->get_key(0), split_node);
    }
    if (split_node->is_leaf()) {
      split_node->InsertLeafList(split_index);
    }
  }
  return true;
}

bool BPTree::Get(KEY_TYPE key, VALUE_TYPE* value) {
  node* cur_node = root;
  while (cur_node!=NULL && !cur_node->is_leaf()) {      //get the leaf node
    if (key > cur_node->get_max_key()) {
      return false;
    }
    index = SearchKey(cur_node, key);
    cur_node = cur_node->get_value(index);
  }
  if (cur_node == NULL) {
    //TODO:log error
    return false;
  }
  index = SearchKey(cur_node, key);
  if (key != cur_node->get_key(index)) {
    return false;
  }
  *value = cur_node->get_value(index);     //TODO:new function for value assign needed
  return true;
}

}
