#ifndef RBBST_H
#define RBBST_H

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
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
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
    void insertFix(AVLNode<Key,Value> * n);
    void removeFix(AVLNode<Key,Value> * n, int diff);
    void rotateRight(AVLNode<Key,Value> * root);
    void rotateLeft(AVLNode<Key,Value> * root);
};


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value> * root = static_cast<AVLNode<Key, Value>*>(this->root_);
    while (root != NULL) {
        if (new_item.first > root->getKey()) { // if the key is greater than the root
            if (root->getRight() == NULL) { // and there's empty space to the right
                AVLNode<Key, Value> * newN = new AVLNode<Key, Value>(new_item.first, new_item.second, root);
                root->setRight(newN);
                insertFix(newN); // fix the insert
                return;
            }
            root = root->getRight(); // not NULL to right, continue down right subtree
        }
        else if (new_item.first < root->getKey()) { // opposite as above
            if (root->getLeft() == NULL) {
                AVLNode<Key, Value> * newN = new AVLNode<Key, Value>(new_item.first, new_item.second, root);
                root->setLeft(newN);
                insertFix(newN);
                return;
            }
            root = root->getLeft();
        }
        else {
            root->setValue(new_item.second); // no need to rebalance here
            return;
        }
    }
    if (this->root_ == NULL) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value> * n)
{
    AVLNode<Key,Value> * p = n->getParent();
    if (p == NULL)
    {
        std::cout << "NULL parent not supposed to happen." << std::endl;
        return;
    }

    if (p->getBalance() == 1 || p->getBalance() == -1) {
        p->setBalance(0); // a parent with one child before insert is 0 balanced after insert
        return; // height does not change between 1 and 2 children
    }
    else if (p->getLeft() == n) {
        p->setBalance(-1); // right - left
    } 
    else if (p->getRight() == n) p->setBalance(1);
    else std::cout << "This is not supposed to execute. (1)" << std::endl;

    AVLNode<Key,Value> * g = p->getParent();
    if (g == NULL) return; // can't unbalance if inserting right after the root

    if (g->getBalance() == 0) {
        insertFix(p);
    }
    else if (g->getBalance() == 1) // right side out of wack
    {
        if (p->getBalance() == 1) rotateLeft(g);
        else if (p->getBalance() == -1) {
            rotateRight(p);
            rotateLeft(g);
        }
        else std::cout << "This is not supposed to execute. (3)" << std::endl;
        switch (n->getBalance()) {
            case -1:
                p->setBalance(0);
                g->setBalance(-1);
                break;
            case 0:
                p->setBalance(0);
                g->setBalance(0);
                break;
            case 1:
                p->setBalance(1);
                g->setBalance(0);
                break;
            default:
                std::cout << "This is not supposed to execute. (5)" << std::endl;
        }
    }
    else if (g->getBalance() == -1)
    {
        if (p->getBalance() == -1) rotateRight(g);
        else if (p->getBalance() == 1) {
            rotateLeft(p);
            rotateRight(g);
        }
        else std::cout << "This is not supposed to execute. (4)" << std::endl;
        switch (n->getBalance()) {
            case -1:
                p->setBalance(0);
                g->setBalance(1);
                break;
            case 0:
                p->setBalance(0);
                g->setBalance(0);
                break;
            case 1:
                p->setBalance(-1);
                g->setBalance(0);
                break;
            default:
                std::cout << "This is not supposed to execute. (6)" << std::endl;
        }
    }
    else std::cout << "This is not supposed to execute. (2)" << std::endl;
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value> * root)
{
    AVLNode<Key, Value> * p = root->getParent();
    int pIsLeft = 1;
    if (p == NULL) pIsLeft = -1;
    else if (p->getRight() == root) pIsLeft = 0;
    AVLNode<Key,Value> * right = root;
    AVLNode<Key,Value> * rootNew = root->getLeft();
    if (rootNew == NULL) {
        std::cout << "Trying to rotate NULL to root" << std::endl;
        return;
    }
    AVLNode<Key,Value> * rightsLeft = rootNew->getRight();

    rootNew->setParent(p);
    if (pIsLeft == 1) p->setLeft(rootNew);
    else if (pIsLeft == 0) p->setRight(rootNew);

    right->setLeft(rightsLeft);
    if (rightsLeft != NULL) rightsLeft->setParent(right);

    rootNew->setRight(right);
    right->setParent(rootNew);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value> * root)
{
    AVLNode<Key, Value> * p = root->getParent();
    int pIsLeft = 1;
    if (p == NULL) pIsLeft = -1;
    else if (p->getRight() == root) pIsLeft = 0;
    AVLNode<Key,Value> * left = root;
    AVLNode<Key,Value> * rootNew = root->getRight();
    if (rootNew == NULL) {
        std::cout << "Trying to rotate NULL to root" << std::endl;
        return;
    }
    AVLNode<Key,Value> * leftsRight = rootNew->getLeft();

    rootNew->setParent(p);
    if (pIsLeft == 1) p->setLeft(rootNew);
    else if (pIsLeft == 0) p->setRight(rootNew);

    left->setRight(leftsRight);
    if (leftsRight != NULL) leftsRight->setParent(left);

    rootNew->setLeft(left);
    left->setParent(rootNew);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value> * toremove = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if (toremove == NULL) return;

    if (toremove->getLeft() != NULL && toremove->getRight() != NULL) { // swaps toremove so now it's a leaf or has one child
        nodeSwap(toremove, static_cast<AVLNode<Key,Value>*>(this->predecessor(toremove)));
    }

    AVLNode<Key, Value>* p = toremove->getParent();
    bool isLeft = false;
    bool isRoot = p == NULL;
    if(!isRoot && (toremove == p->getLeft())) isLeft = true;

    if (toremove->getLeft() == NULL && toremove->getRight() == NULL) { // a leaf
        if (!isRoot) { 
            if (isLeft) p->setLeft(NULL);
            else p->setRight(NULL);
        }
        else {
            this->root_ = NULL;
        }
    }
    else { // promote one child
        AVLNode<Key, Value> * child = toremove->getLeft();
        if (child == NULL) child = toremove->getRight();
        if (!isRoot) {
            if (isLeft) p->setLeft(child);
            else p->setRight(child);
            child->setParent(p);
        }
        else {
            this->root_ = child;
            child->setParent(NULL);
        }
    }

    if (isLeft) removeFix(p, 1); // left
    else removeFix(p, -1); // right

    delete toremove;
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value> * n, int diff)
{
    if(n == NULL) return;
    AVLNode<Key,Value> * p = n->getParent();
    int ndiff = 1;
    if(!(p == NULL) && (n == p->getLeft())) ndiff = -1;

    switch(n->getBalance() + diff)
    {
        case -2:
            {
                AVLNode<Key,Value> * lc = n->getLeft();
                switch(lc->getBalance()) {
                    case -1:
                        rotateRight(n);
                        n->setBalance(0);
                        lc->setBalance(0);
                        removeFix(p, ndiff);
                        break;
                    case 0:
                        rotateRight(n);
                        n->setBalance(-1);
                        lc->setBalance(1);
                        break;
                    case 1:
                        AVLNode<Key,Value> * gc = lc->getRight();
                        rotateLeft(lc);
                        rotateRight(n);
                        switch(gc->getBalance()) {
                            case 1:
                                n->setBalance(0); lc->setBalance(-1); gc->setBalance(0); break;
                            case 0:
                                n->setBalance(0); lc->setBalance(0); gc->setBalance(0); break;
                            case -1:
                                n->setBalance(1); lc->setBalance(0); gc->setBalance(0); break;
                        }
                        removeFix(p, ndiff);
                }
                break;
            }
        case -1:
            n->setBalance(-1);
            break;
        case 0:
            n->setBalance(0);
            removeFix(p, ndiff);
            break;
        case 2:
            {
                AVLNode<Key,Value> * rc = n->getRight();
                switch(rc->getBalance()) {
                    case -1:
                        rotateLeft(n);
                        n->setBalance(0);
                        rc->setBalance(0);
                        removeFix(p, ndiff);
                        break;
                    case 0:
                        rotateLeft(n);
                        n->setBalance(1);
                        rc->setBalance(-1);
                        break;
                    case 1:
                        AVLNode<Key,Value> * gc = rc->getLeft();
                        rotateRight(rc);
                        rotateLeft(n);
                        switch(gc->getBalance()) {
                            case 1:
                                n->setBalance(0); rc->setBalance(1); gc->setBalance(0); break;
                            case 0:
                                n->setBalance(0); rc->setBalance(0); gc->setBalance(0); break;
                            case -1:
                                n->setBalance(-1); rc->setBalance(0); gc->setBalance(0); break;
                        }
                        removeFix(p, ndiff);
                }
                break;
            }
        case 1:
            n->setBalance(1);
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
