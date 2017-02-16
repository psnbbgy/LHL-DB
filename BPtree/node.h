
#ifndef _BP_NODE_H_
#define _BP_NODE_H_
namespace BP {

#define MAX_SLOT_NUM 20   //max key number of node

typedef int  KEY_TYPE;
typedef int  VALUE_TYPE;

class node {
 public:
  void InsertIntoNode(node* cur_node, int index, KEY_TYPE key, void* value);  //insert data into node
  node* SplitNode();    //split full node to two

  void AddSlotNum(int add_num) {
    slot_num_ += add_num;
  }
  
  node* get_father() {
    return father_;
  }

  KEY_TYPE& get_key(int index) {
    return key[index];
  }

  int get_slot_num() {
    return slot_num_;
  }

  KEY_TYPE get_max_key() {
    return max_key_;
  }

  void set_max_key(KEY_TYPE max_key) {
    max_key_ = max_key;
  }

  virtual void InsertLeafList(int index) = 0;     //insert into link list of leaf node
  virtual bool is_leaf() = 0;
  virtual void* get_pos(int index) = 0;
  virtual size_t get_value_size() = 0;
  virtual void DeleteNode() = 0;
  virtual node* NewSelfObject() = 0;
  virtual node* get_value(int index) = 0;

  node() {
    slot_num_ = 0;
    father_ = NULL;
  }

 private:
  KEY_TYPE key[MAX_SLOT_NUM + 1];
  int slot_num_;
  KEY_TYPE max_key_;    //max key below this node
  node* father_;     //point to father node
};

typedef VALUE_TYPE* LEAF_VALUE;
typedef node* INNER_VALUE;

//leaf node for B+ tree
class leaf_node : public node {   //version 1: use key-value in-memory model
 public:
  virtual void InsertLeafList(int index);
  
  leaf_node() {
    next = NULL;
    prev = NULL;
  }

  virtual bool is_leaf() {
    return true;
  }
  
  virtual void* get_pos(int index) {
    return &value_[index];
  }

  virtual size_t get_value_size() {
    return sizeof(LEAF_VALUE);
  }

  virtual void DeleteNode(int index) {
    delete value_[index];     //TODO:a delete function for type needed
  }

  virtual node* NewSelfObject() {
    return new leaf_node();
  }

  virtual node* get_value(int index) {
    return value_[index];
  }

  leaf_node* get_next() {
    return next_;
  }

  leaf_node* get_prev() {
    return prev_;
  }

  void set_next_prev(leaf_node* prev, leaf_node* next) {
    next_ = next;
    prev_ = prev;
  }
  
 private:
  LEAF_VALUE value_[MAX_SLOT_NUM + 1];   //value point to data in db, TODO: use node
  leaf_node* next_;      //use link list to connect leaf node
  leaf_node* prev_;
};

//inner node for B+ tree
class inner_node : public node {
 public:
  virtual void InsertLeafList(int index) {}
  
  virtual bool is_leaf() {
    return false;
  }
  
  virtual void* get_pos(int index) {
    return &child_node_[index];
  }
  
  virtual size_t get_value_size() {
    return sizeof(INNER_VALUE);
  }

  virtual void DeleteNode(int index) {
    delete child_node_[index];
  }

  virtual node* NewSelfObject() {
    return new inner_node();
  }

  virtual node* get_value(int index) {
    return child_node_[index];
  }

 private:
  INNER_VALUE child_node_[MAX_SLOT_NUM + 1];
};

#endif
