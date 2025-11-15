//pasted from my vscode to run tests

#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

using namespace std;

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
    void rotateLeft(AVLNode<Key,Value>* n);
    void rotateRight(AVLNode<Key,Value>* n);
    void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void removeFix(AVLNode<Key,Value>* n, int8_t diff);
};


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    if(this->root_ == nullptr)
    {
        this->root_ = new AVLNode<Key,Value>(new_item.first,new_item.second,nullptr);
        return;
    }
    AVLNode<Key,Value>* cur = static_cast<AVLNode<Key,Value>*>(this->root_);
    AVLNode<Key,Value>* parent = nullptr;
    bool leftChild = false;
    while(cur != nullptr)
    {
        parent = cur;

        if(new_item.first < cur->getKey())
        {
            cur = cur->getLeft();
            leftChild = true;
        }
        else if(new_item.first > cur->getKey())
        {
            cur = cur->getRight();
            leftChild = false;
        }
        else
        {
            cur->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key,Value>* newNode = new AVLNode<Key,Value>(new_item.first, new_item.second, parent);

    if(leftChild == true)
    {
    parent->setLeft(newNode);
    }
    else
    {
    parent->setRight(newNode);
}

int8_t pb = parent->getBalance();

    if(pb == 1 || pb == -1)
    {
    parent->setBalance(0);
    }
    else
    {
    if(leftChild == true)
    {
        parent->setBalance(parent->getBalance() - 1);
    }
    else
    {
        parent->setBalance(parent->getBalance() + 1);
    }

    insertFix(parent, newNode);
}

}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
    if(p == nullptr){
      return;  
    } 

    AVLNode<Key,Value>* g = p->getParent();

    if(g == nullptr) return;

    if(g->getLeft() == p)
    {
        int8_t b = g->getBalance();
        b = b - 1;
        g->setBalance(b);

        if(g->getBalance() == 0){
           return; 
        } 
            else if(g->getBalance() == -1) insertFix(g,p);
            else if(g->getBalance() == -2)
        {
            if(p->getLeft() == n)
            {
                rotateRight(g);
                g->setBalance(0);
                p->setBalance(0);
            }
            else
            {
                rotateLeft(p);
                rotateRight(g);

                if(n->getBalance() == -1){ 
                    p->setBalance(0); g->setBalance(1); 
                }
                else if(n->getBalance() == 0){ 
                    p->setBalance(0); g->setBalance(0); 
                }
                else{ 
                    p->setBalance(-1); g->setBalance(0); 
                }
                n->setBalance(0);
            }
        }
    }
    else
{
    g->updateBalance(1);
    
    if(g->getBalance() == 0)
    {
        return;
    }
    else if(g->getBalance() == 1)
    {
        insertFix(g, p);
    }
    else if(g->getBalance() == 2)
    {
        if(p->getRight() == n)
        {
            rotateLeft(g);
            g->setBalance(0);
            p->setBalance(0);
        }
        else
        {
            rotateRight(p);
            rotateLeft(g);

            if(n->getBalance() == 1)
            { 
                p->setBalance(0);
                g->setBalance(-1); 
            }
            else if(n->getBalance() == 0)
            { 
                p->setBalance(0);
                g->setBalance(0); 
            }
            else
            { 
                p->setBalance(1);
                g->setBalance(0); 
            }

            n->setBalance(0);
        }
    }
}
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key,Value>* n = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if(n == nullptr)
    {
        return;
    }

    int8_t diff = 0;

    AVLNode<Key,Value>* parent = n->getParent();
    if(parent != nullptr)
    {
        if(parent->getLeft() == n) diff = 1;
        else diff = -1;
    }

    bool hasLeft = n->getLeft() != nullptr;
    bool hasRight = n->getRight() != nullptr;

    if(hasLeft && hasRight)
    {
        Node<Key,Value>* tempNode = BinarySearchTree<Key,Value>::predecessor(n);
        AVLNode<Key,Value>* pred = static_cast<AVLNode<Key,Value>*>(tempNode);

        nodeSwap(n, pred);

        parent = n->getParent();
        if(parent != nullptr)
        {
            if(parent->getLeft() == n){
              diff = 1;  
            } 
            else diff = -1;
        }
    }

    AVLNode<Key,Value>* child = nullptr;
    if(n->getLeft() != nullptr){
         child = n->getLeft();
    }  
    else if(n->getRight() != nullptr){
         child = n->getRight();
    }

    if(child != nullptr)
    {
        child->setParent(parent);
    }

    if(parent == nullptr)
    {
        this->root_ = child;
    }
    else
    {
        if(parent->getLeft() == n){
          parent->setLeft(child);  
        } 
        else parent->setRight(child);
    }

    if(parent != nullptr)
    {
        removeFix(parent, diff);
    }

    delete n;
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int8_t diff)
{
    if(n == nullptr)
    {
        return;
    }

    AVLNode<Key,Value>* p = n->getParent();
    int8_t ndiff = 0;

    if(p != nullptr)
    {
        if(p->getLeft() == n)
        {
            ndiff = 1;
        }
        else
        {
            ndiff = -1;
        }
    }

    if(diff == 1)
    {
        n->updateBalance(1);

        if(n->getBalance() == 1)
        {
            return;
        }
        else if(n->getBalance() == 0)
        {
            removeFix(p, ndiff);
        }
        else if(n->getBalance() == 2)  
        {
            AVLNode<Key,Value>* c = n->getRight(); 
            if(c->getBalance() == 0)
            {
                rotateLeft(n); 
                n->setBalance(1);
                c->setBalance(-1);
                return;
            }
            else if(c->getBalance() == 1)  
            {
                rotateLeft(n); 
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff); 
            }
            else  
            {
                AVLNode<Key,Value>* g = c->getLeft();  
                rotateRight(c);  
                rotateLeft(n);  

                if(g->getBalance() == 1)  
                {
                    n->setBalance(-1); 
                    c->setBalance(0);
                }
                else if(g->getBalance() == 0)
                {
                    n->setBalance(0);
                    c->setBalance(0);
                }
                else  
                {
                    n->setBalance(0);
                    c->setBalance(1);  
                }

                g->setBalance(0);
                removeFix(p, ndiff);  
            }
        }
    }
    else if(diff == -1)
    {
        n->updateBalance(-1);

        if(n->getBalance() == -1)
        {
            return;
        }
        else if(n->getBalance() == 0)
        {
            removeFix(p, ndiff);
        }
        else if(n->getBalance() == -2)  
        {
            AVLNode<Key,Value>* c = n->getLeft();  
            if(c->getBalance() == 0)
            {
                rotateRight(n); 
                n->setBalance(-1);
                c->setBalance(1);
                return;
            }
            else if(c->getBalance() == -1)  
            {
                rotateRight(n);  
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);  
            }
            else  
            {
                AVLNode<Key,Value>* g = c->getRight();  
                rotateLeft(c);  
                rotateRight(n);  

                if(g->getBalance() == -1)  
                {
                    n->setBalance(1);  
                    c->setBalance(0);
                }
                else if(g->getBalance() == 0)
                {
                    n->setBalance(0);
                    c->setBalance(0);
                }
                else  
                {
                    n->setBalance(0);
                    c->setBalance(-1);  
                }

                g->setBalance(0);
                removeFix(p, ndiff);  
            }
        }
    }
}

 
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* n)
{
    AVLNode<Key,Value>* r = n->getRight();
    AVLNode<Key,Value>* rl = nullptr;
    if(r != nullptr)
    {
        rl = r->getLeft();
    }

    AVLNode<Key,Value>* p = n->getParent();

    if(p == nullptr)
    {
        this->root_ = r;
    }
    else
    {
        if(p->getLeft() == n)
        {
            p->setLeft(r);
        }
        else
        {
            p->setRight(r);
        }
    }

    if(r != nullptr)
    {
        r->setParent(p);
        r->setLeft(n);
    }

    n->setParent(r);

    n->setRight(rl);
    if(rl != nullptr)
    {
        rl->setParent(n);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* n)
{
    AVLNode<Key,Value>* l = n->getLeft();
    AVLNode<Key,Value>* lr = nullptr;
    if(l != nullptr)
    {
        lr = l->getRight();
    }

    AVLNode<Key,Value>* p = n->getParent();

    if(p == nullptr)
    {
        this->root_ = l;
    }
    else
    {
        if(p->getLeft() == n)
        {
            p->setLeft(l);
        }
        else
        {
            p->setRight(l);
        }
    }

    if(l != nullptr)
    {
        l->setParent(p);
        l->setRight(n);
    }

    n->setParent(l);

    n->setLeft(lr);
    if(lr != nullptr)
    {
        lr->setParent(n);
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);

    int8_t tempB = n1->getBalance();

    n1->setBalance(n2->getBalance());
    
    n2->setBalance(tempB);

    if(n1->getLeft() != nullptr){
        n1->getLeft()->setParent(n1);
    }
    if(n1->getRight() != nullptr){
        n1->getRight()->setParent(n1);
    }

    if(n2->getLeft() != nullptr){
        n2->getLeft()->setParent(n2);
    }

    if(n2->getRight() != nullptr){
        n2->getRight()->setParent(n2);
    }
}


#endif