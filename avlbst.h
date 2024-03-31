#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateRight(AVLNode<Key, Value>* node);
    void rotateLeft(AVLNode<Key, Value>* node);

    void insertionFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* v);
    void removalFix(AVLNode<Key, Value>* node, int ndiff);
};

template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
  // AVL rotates around a subtree
  if(node == nullptr) { return; }

  AVLNode<Key, Value>* leftChild = node->getLeft();
  // changing the node's left child & updating parent-child relationship
  node->setLeft(leftChild->getRight());    
  if(leftChild->getRight() != nullptr) {
    leftChild->getRight()->setParent(node);
  }
  leftChild->setRight(node);

  // left child becomes root of subtree
  leftChild->setParent(node->getParent());
  if(node->getParent() == nullptr)
  {
    BinarySearchTree<Key, Value>::root_ = leftChild;
  }
  else if(node->getParent()->getLeft() == node)
  {
    node->getParent()->setLeft(leftChild);
  }
  else
  {
    node->getParent()->setRight(leftChild);
  }
  node->setParent(leftChild);

  return;
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
  // AVL rotates around a subtree
  if(node == nullptr) { return; }

  AVLNode<Key, Value>* rightChild = node->getRight();
  // changing the node's right child & updating parent-child relationship
  node->setRight(rightChild->getLeft());    
  if(rightChild->getLeft() != nullptr) {
    rightChild->getLeft()->setParent(node);
  }
  rightChild->setLeft(node);

  // right child becomes root of subtree
  rightChild->setParent(node->getParent());
  if(node->getParent() == nullptr)
  {
    BinarySearchTree<Key, Value>::root_ = rightChild;
  }
  else if(node->getParent()->getLeft() == node)
  {
    node->getParent()->setLeft(rightChild);
  }
  else
  {
    node->getParent()->setRight(rightChild);
  }
  node->setParent(rightChild);

  return;
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::insertionFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* b)
{
  if(p == nullptr || p->getParent() == nullptr)
  {
    return;
  }

  AVLNode<Key, Value>* g = p->getParent();
  // Assume p is left child of g
  if(g->getLeft() == p)
  {
    g->updateBalance(-1);
    // Case 1: b(g) == 0, return
    if(g->getBalance() == 0)
    {
      return;
    }
    else if(g->getBalance() == -1)
    {
      insertionFix(g, p);
    }
    else if(g->getBalance() == -2)
    {
      if(p->getBalance() == -1)
      {
        rotateRight(g);
        p->setBalance(0);
        g->setBalance(0);
      }
      if(p->getBalance() == 1)
      {
        rotateLeft(p);
        rotateRight(g);

        if(b->getBalance() == -1)
        {
          p->setBalance(0);
          g->setBalance(1);
          b->setBalance(0);
        }
        else if(b->getBalance() == 0)
        {
          p->setBalance(0);
          g->setBalance(0);
          b->setBalance(0);
        }
        else if(b->getBalance() == 1)
        {
          p->setBalance(-1);
          g->setBalance(0);
          b->setBalance(0);
        }
      }
    }
  }

  else
  {
    g->updateBalance(1);
    // Case 1: b(g) == 0, return
    if(g->getBalance() == 0)
    {
      return;
    }
    else if(g->getBalance() == 1)
    {
      insertionFix(g, p);
    }
    else if(g->getBalance() == 2)
    {
      if(p->getBalance() == 1)
      {
        rotateLeft(g);
        p->setBalance(0);
        g->setBalance(0);
      }
      if(p->getBalance() == -1)
      {
        rotateRight(p);
        rotateLeft(g);

        if(b->getBalance() == 1)
        {
          p->setBalance(0);
          g->setBalance(-1);
          b->setBalance(0);
        }
        else if(b->getBalance() == 0)
        {
          p->setBalance(0);
          g->setBalance(0);
          b->setBalance(0);
        }
        else if(b->getBalance() == -1)
        {
          p->setBalance(1);
          g->setBalance(0);
          b->setBalance(0);
        }
      }
    }
  }
}


template<typename Key, typename Value>
void AVLTree<Key, Value>::removalFix(AVLNode<Key, Value>* node, int diff)
{
  if(node == nullptr){ return; }
  AVLNode<Key, Value>* p = node->getParent();
  int ndiff;
  if(p != nullptr && p->getLeft() == node) {
    ndiff = 1;
  }
  else {
    ndiff = -1;
  }

  // Assume diff = -1
  if(diff == -1)
  {
    // Case 1: b(n) + diff == -2
    if(node->getBalance() + diff == -2)
    {
      AVLNode<Key, Value>* c = node->getLeft();
      // Case 1a: b(c) == -1 (zig zig case)
      if(c->getBalance() == -1) {
        rotateRight(node);
        node->setBalance(0);
        c->setBalance(0);
        removalFix(p, ndiff);
      }
      // Case 1b: b(c) == 0 (zig zig case)
      else if(c->getBalance() == 0)
      {
        rotateRight(node);
        node->setBalance(-1);
        c->setBalance(1);
        return;
      }

      // Case 1c: b(c) == +1 (zig zag case)
      else if(c->getBalance() == 1)
      {
        AVLNode<Key, Value>* g = c->getRight();
        rotateLeft(c); rotateRight(node);
        
        if(g->getBalance() == 1)
        {
          node->setBalance(0); c->setBalance(-1); g->setBalance(0);
        }
        else if(g->getBalance() == 0)
        {
          node->setBalance(0); c->setBalance(0); g->setBalance(0);
        }
        else if(g->getBalance() == -1)
        {
          node->setBalance(1); c->setBalance(0); g->setBalance(0);
        }

        removalFix(p, ndiff);
      }

    }

    else if(node->getBalance() + diff == -1)
    {
      node->setBalance(-1);
      return;
    }
    else if(node->getBalance() + diff == 0)
    {
      node->setBalance(0);
      removalFix(p, ndiff);
    }
  }
  
  // Mirror case when diff = +1
  else if(diff == 1)
  {
    // Case 1: b(n) + diff == 2
    if(node->getBalance() + diff == 2)
    {
      AVLNode<Key, Value>* c = node->getRight();
      // Case 1a: b(c) == 1 (zig zig case)
      if(c->getBalance() == 1) {
        rotateLeft(node);
        node->setBalance(0);
        c->setBalance(0);
        removalFix(p, ndiff);
      }
      // Case 1b: b(c) == 0 (zig zig case)
      else if(c->getBalance() == 0)
      {
        rotateLeft(node);
        node->setBalance(1);
        c->setBalance(-1);
        return;
      }

      // Case 1c: b(c) == -1 (zig zag case)
      else if(c->getBalance() == -1)
      {
        AVLNode<Key, Value>* g = c->getLeft();
        rotateRight(c); rotateLeft(node);
        
        if(g->getBalance() == -1)
        {
          node->setBalance(0); c->setBalance(1); g->setBalance(0);
        }
        else if(g->getBalance() == 0)
        {
          node->setBalance(0); c->setBalance(0); g->setBalance(0);
        }
        else if(g->getBalance() == 1)
        {
          node->setBalance(-1); c->setBalance(0); g->setBalance(0);
        }

        removalFix(p, ndiff);
      }

    }

    else if(node->getBalance() + diff == 1)
    {
      node->setBalance(1);
      return;
    }
    else if(node->getBalance() + diff == 0)
    {
      node->setBalance(0);
      removalFix(p, ndiff);
    }
  }
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* node = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    if(BinarySearchTree<Key, Value>::root_ == nullptr)
    {
      BinarySearchTree<Key, Value>::root_ = node;
      return;
    }

    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
    AVLNode<Key, Value>* lastNode = nullptr;
    while(temp != nullptr)
    {
      lastNode = temp;
      if(new_item.first > temp->getKey())
      {
        temp = temp->getRight();
      }
      else if(new_item.first < temp->getKey())
      {
        temp = temp->getLeft();
      }
      else
      {
        temp->setValue(new_item.second);
        delete node;
        return;
      }
    }

    node->setParent(lastNode);
    if(node->getKey() < lastNode->getKey())
    {
      lastNode->setLeft(node);
    }
    else
    {
      lastNode->setRight(node);
    }


    if(lastNode->getBalance() != 0)
    {
      lastNode->setBalance(0);
    }
    else
    {
      if(lastNode->getLeft() == node)
      {
        lastNode->setBalance(-1);
      }
      else
      {
        lastNode->setBalance(1);
      }

      insertionFix(lastNode, node);
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    Node<Key, Value>* node = (this->internalFind(key));
    // node not in the AVL tree
    if(node == nullptr){
      return;
    }

    // node has two children
    AVLNode<Key, Value>* pred = nullptr;
    if(node->getLeft() != nullptr && node->getRight() != nullptr)
    {
      pred = static_cast<AVLNode<Key,Value>*>(this->predecessor(*this, node));
      nodeSwap(static_cast<AVLNode<Key, Value>*>(node), pred);
    }

    // node has 0 or 1 children guaranteed
    int diff;
    Node<Key, Value>* parent = node->getParent();
    if(parent != nullptr){
      if(parent->getLeft() == node)
      {
        diff = 1;
      }
      else
      {
        diff = -1;
      }
    }

    if(node->getLeft() == nullptr && node->getRight() == nullptr)
    {
      this->fixParent(node->getParent(), node, nullptr);

    }
    else if(node->getRight() == nullptr)
    {
      this->fixParent(node->getParent(), node, node->getLeft());
    }
    else if(node->getLeft() == nullptr)
    {
      this->fixParent(node->getParent(), node, node->getRight());
    }
    
    delete node;
    removalFix(static_cast<AVLNode<Key, Value>*>(parent), diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
