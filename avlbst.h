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
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == NULL)
    {
        this->root_ = new AVLNode<Key, Value>(
            new_item.first, new_item.second, NULL);
        return;
    }

    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);

    AVLNode<Key, Value>* parent = NULL;

    while(current != NULL)
    {
        parent = current;

        if(new_item.first == current->getKey())
        {
            current->setValue(new_item.second);
            return;
        }
        else if(new_item.first < current->getKey()) current = current->getLeft();
        else current = current->getRight();
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(
        new_item.first,
        new_item.second,
        parent);

    if(new_item.first < parent->getKey()) parent->setLeft(newNode);
    else parent->setRight(newNode);

    insertFix(parent, newNode);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node)
{
    if(parent == NULL) return;

    if(parent->getLeft() == node) parent->updateBalance(-1);
    else parent->updateBalance(1);

    if(parent->getBalance() == 0) return;

    if(parent->getBalance() == -1 || parent->getBalance() == 1)
    {
        insertFix(parent->getParent(), parent);
        return;
    }

    if(parent->getBalance() == -2)
    {
        AVLNode<Key, Value>* child = parent->getLeft();

        if(child->getBalance() == -1)
        {
            rotateRight(parent);
            parent->setBalance(0);
            child->setBalance(0);
        }
        else
        {
            AVLNode<Key, Value>* grandchild = child->getRight();

            int8_t balance = grandchild->getBalance();

            rotateLeft(child);
            rotateRight(parent);

            if(balance == -1)
            {
                parent->setBalance(1);
                child->setBalance(0);
            }
            else if(balance == 0)
            {
                parent->setBalance(0);
                child->setBalance(0);
            }
            else
            {
                parent->setBalance(0);
                child->setBalance(-1);
            }

            grandchild->setBalance(0);
        }
    }
    else if(parent->getBalance() == 2)
    {
        AVLNode<Key, Value>* child = parent->getRight();

        if(child->getBalance() == 1)
        {
            rotateLeft(parent);
            parent->setBalance(0);
            child->setBalance(0);
        }
        else
        {
            AVLNode<Key, Value>* grandchild = child->getLeft();

            int8_t balance = grandchild->getBalance();

            rotateRight(child);
            rotateLeft(parent);

            if(balance == 1)
            {
                parent->setBalance(-1);
                child->setBalance(0);
            }
            else if(balance == 0)
            {
                parent->setBalance(0);
                child->setBalance(0);
            }
            else
            {
                parent->setBalance(0);
                child->setBalance(1);
            }

            grandchild->setBalance(0);
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* right = node->getRight();
    AVLNode<Key, Value>* parent = node->getParent();

    node->setRight(right->getLeft());

    if(right->getLeft() != NULL) right->getLeft()->setParent(node);

    right->setParent(parent);

    if(parent == NULL)this->root_ = right;
    else if(parent->getLeft() == node) parent->setLeft(right);
    else parent->setRight(right);

    right->setLeft(node);
    node->setParent(right);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* left = node->getLeft();
    AVLNode<Key, Value>* parent = node->getParent();

    node->setLeft(left->getRight());

    if(left->getRight() != NULL) left->getRight()->setParent(node);

    left->setParent(parent);

    if(parent == NULL) this->root_ = left;
    else if(parent->getLeft() == node) parent->setLeft(left);
    else parent->setRight(left);

    left->setRight(node);
    node->setParent(left);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* target = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if(target == NULL) return;

    if(target->getLeft() != NULL && target->getRight() != NULL)
    {
        AVLNode<Key, Value>* pred =static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(target));

        nodeSwap(target, pred);
    }

    AVLNode<Key, Value>* parent = target->getParent();

    AVLNode<Key, Value>* child;

    if(target->getLeft() != NULL) child = target->getLeft();
    else child = target->getRight();

    int8_t diff = 0;

    if(parent != NULL)
    {
        if(parent->getLeft() == target) diff = 1;
        else diff = -1;
    }

    if(child != NULL) child->setParent(parent);

    if(parent == NULL) this->root_ = child;
    else if(parent->getLeft() == target) parent->setLeft(child);
    else parent->setRight(child);
    delete target;

    if(parent != NULL) removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff)
{
    if(node == NULL) return;

    AVLNode<Key, Value>* parent = node->getParent();
    int8_t nextDiff = 0;

    if(parent != NULL)
    {
        if(parent->getLeft() == node) nextDiff = 1;
        else nextDiff = -1;
    }

    int balance = node->getBalance() + diff;

    if(balance == -2)
    {
        AVLNode<Key, Value>* child = node->getLeft();
        int8_t childBalance = child->getBalance();

        if(childBalance <= 0)
        {
            rotateRight(node);

            if(childBalance == 0)
            {
                node->setBalance(-1);
                child->setBalance(1);
                return;
            }

            node->setBalance(0);
            child->setBalance(0);

            removeFix(parent, nextDiff);
        }
        else
        {
            AVLNode<Key, Value>* grandchild = child->getRight();

            int8_t grandBalance = grandchild->getBalance();

            rotateLeft(child);
            rotateRight(node);

            if(grandBalance == 1)
            {
                node->setBalance(0);
                child->setBalance(-1);
            }
            else if(grandBalance == 0)
            {
                node->setBalance(0);
                child->setBalance(0);
            }
            else
            {
                node->setBalance(1);
                child->setBalance(0);
            }

            grandchild->setBalance(0);

            removeFix(parent, nextDiff);
        }
    }
    else if(balance == 2)
    {
        AVLNode<Key, Value>* child = node->getRight();
        int8_t childBalance = child->getBalance();

        if(childBalance >= 0)
        {
            rotateLeft(node);

            if(childBalance == 0)
            {
                node->setBalance(1);
                child->setBalance(-1);
                return;
            }

            node->setBalance(0);
            child->setBalance(0);

            removeFix(parent, nextDiff);
        }
        else
        {
            AVLNode<Key, Value>* grandchild = child->getLeft();

            int8_t grandBalance = grandchild->getBalance();

            rotateRight(child);
            rotateLeft(node);

            if(grandBalance == -1)
            {
                node->setBalance(0);
                child->setBalance(1);
            }
            else if(grandBalance == 0)
            {
                node->setBalance(0);
                child->setBalance(0);
            }
            else
            {
                node->setBalance(-1);
                child->setBalance(0);
            }

            grandchild->setBalance(0);

            removeFix(parent, nextDiff);
        }
    }
    else if(balance == -1 || balance == 1)
    {
        node->setBalance(balance);
        return;
    }
    else
    {
        node->setBalance(0);
        removeFix(parent, nextDiff);
    }
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
