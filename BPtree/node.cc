#include "node.h"

namespace BP {

void leaf_node::InsertLeafList(int index) {  //index tell us the pos of leaf node in inner node
  leaf_node* target_node;
  if (index < father->get_slot_num()-1) {
    target_node = dynamic_cast<leaf_node*>(father()->get_value(index + 1));
    set_next_prev(target_node->get_prev(), target_node);
    target_node->set_next_prev(this, target_node->get_next());
    get_prev()->set_next_prev(get_prev()->get_prev(), this);
  } else {
    target_node = dynamic_cast<leaf_node*>(father()->get_value(index - 1));
    set_next_prev(target_node, target_node->get_next());
    target_node->set_next_prev(target_node->get_prev(), this);
    get_next()->set_next_prev(this, get_next()->get_next());
  }
}

void node::InsertIntoNode(int index, KEY_TYPE key, void* value) {
  if (key > max_key_) {
    max_key_ = key;
  }
  if (index == MAX_SLOT_NUM - 1) {
    key[index + 1] = key;
    *(get_pos(index + 1)) = value;
  }
  if (index < 0 || key != key[index]) {          //insert a new key
    memcpy(&(key[index + 2]), &(key[index + 1]), 
           sizeof(KEY_TYPE) * (slot_num_ - index));
    memcpy(get_pos(index + 2), get_pos(index + 1), 
           get_value_size() * (slot_num_ - index));
    key[index + 1] = key;
    *(get_pos(index + 1)) = value;
    ++slot_num_;
    InsertLeafList(index + 1);
  } else {                                   //insert a existing key
    key[index] = key;
    DeleteNode(index);
    *(get_pos(index)) = value;
  }
}

virtual node* node::SplitNode() {
  node* res = NewSelfObject();
  if (slot_num_ != MAX_SLOT_NUM + 1) {
    //log error
    return NULL;
  }
  memcpy(res->get_key(0), &key_[slot_num_ / 2], slot_num_ - slot_num_/2);
  memcpy(res->get_pos(0), &value_[slot_num_ / 2], slot_num_ - slot_num_/2);
  res->AddSlotNum(slot_num_ - slot_num_/2);
  slot_num_ -= (slot_num_ - slot_num_/2);
  if (is_leaf()) {
    max_key_ = key_[slot_num_/2 - 1];
    res->set_max_key(res->get_key(slot_num_ - slot_num_/2 - 1));
  } else {
    max_key_ = get_value(slot_num_/2 - 1)->get_max_key();
    res->set_max_key(res->get_value(slot_num_ - slot_num_/2 - 1)->get_max_key());
  }
  return res;
}

}
