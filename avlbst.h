#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    signed char getBalance () const;
    void setBalance (signed char balance);
    void updateBalance(signed char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    signed char balance_;
};


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
signed char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(signed char balance)
{
    balance_ = balance;

}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(signed char diff)
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





template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateRight(AVLNode<Key,Value>* x);
    void rotateLeft(AVLNode<Key,Value>* x);
    void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    bool isRightChild(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    bool isLeftChild(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void removeFix(AVLNode<Key, Value> *n, signed char diff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO        
    AVLNode<Key, Value> *key = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    std::cout << key->getKey() << std::endl;
    key->setBalance(0);
    if(this->root_ == nullptr){
        this->root_ = key;
        key->setBalance(0);
        //std::cout << key->getKey() <<" balance: " << (int)key->getBalance() << std::endl;
        //this->printRoot(this->root_);
        return;
    }
    else{
        AVLNode<Key, Value> *curr = static_cast<AVLNode<Key,Value>*>(this->root_);
        AVLNode<Key, Value> *prev = nullptr;

        int x = 0;
        while(curr != nullptr)
        {
           // std::cout <<"3possible seg fault here" << std::endl;
            if(key->getKey() < curr->getKey())
            {
                //std::cout <<"1possible seg fault here" << std::endl;
                prev = curr;
                curr = curr->getLeft();
                x = -1;
            } 
            else if (key->getKey() > curr->getKey())
            {
                prev = curr;
                curr = curr->getRight();
                x = 1;

            }
            else{
                curr->setValue(key->getValue());
                delete key;
                return;
            }
            
        }
        curr = key;
        if(x == -1)
        {
            curr->setParent(prev);
            prev->setLeft(curr);
            //std::cout << "i set " << curr->getKey() << "parent to " << prev->getKey() <<std::endl;
            //std::cout << "added left: " << keyValuePair.first << std::endl;
        }
        else if(x == 1)
        {
            curr->setParent(prev);
            prev->setRight(curr);
            //std::cout << "i set " << curr->getKey() << "parent to " << prev->getKey() <<std::endl;
            //std::cout << "added right: " << keyValuePair.first << std::endl;
        }
        //std::cout << prev->getKey() <<" balance: " << (int)prev->getBalance() << std::endl;
        if(prev->getBalance() == -1 ||prev->getBalance() == 1)
        {
            prev->setBalance( 0);
            //this->printRoot(this->root_);
            return;
        }
        else if(prev->getBalance() == 0)
        {
            //std::cout << "im here!" << std::endl;
            if(isLeftChild(prev, curr))
                prev->setBalance((signed char) -1);
            else if(isRightChild(prev, curr))
                prev->setBalance(1);
            //
            this->insertFix(prev, curr);
            //this->printRoot(this->root_);
            return;
        }
    }
}   


template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODo
    if(this->internalFind(key) == NULL){
        return;
    }
    AVLNode<Key, Value> *curr = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    int diff = 0;
    bool rt = false;
    AVLNode<Key, Value> *pred;
    std::cout << curr->getKey() << std::endl;
    this->printRoot(this->root_);
    if(curr != nullptr)
    {
        if(curr == this->root_)
            rt = true;
        if(curr->getRight() != nullptr && curr->getLeft() != nullptr)
        {
            pred = static_cast<AVLNode<Key,Value>*>(this->predecessor(curr));
            nodeSwap(pred, curr);
            if(rt)
                this->root_ = pred;
        }
        else if(curr->getRight() != nullptr && curr->getLeft() == nullptr)
        {
            AVLNode<Key, Value> *pred = curr->getRight();
            nodeSwap(pred, curr);
            if(rt)
                this->root_ = pred;
        }
        else if(curr->getRight() == nullptr && curr->getLeft() != nullptr)
        {
            pred = static_cast<AVLNode<Key,Value>*>(this->predecessor(curr));
            nodeSwap(pred, curr);
            if(rt)
                this->root_ = pred;
        }
        AVLNode<Key,Value> *p = curr->getParent();
        if(p != nullptr)
        {
            if(isLeftChild(p, curr))
            {
                diff = 1;
            }
            else if(isRightChild(p, curr))
            {
                diff = -1;
            }
        }
        //delete curr;
        this->printRoot(this->root_);
        AVLNode<Key, Value> *par = curr->getParent();
        if(par != nullptr ){
            bool left = false;
            bool right = false;
        if(isLeftChild(par, curr)){
            par->setLeft(nullptr);
            left = true;
        }
        else{
            par->setRight(nullptr);
            right = true;
        }
        
        if(curr->getRight() != nullptr)
        {
            if(right)
                par->setRight(curr->getRight());
            else
                par->setLeft(curr->getRight());
            curr->getRight()->setParent(par);
            curr->setRight(nullptr);
        }
        if(curr->getLeft() != nullptr)
        {
            if(left)
                par->setLeft(curr->getLeft());
            else 
                par->setRight(curr->getLeft());
            curr->getLeft()->setParent(par);
            curr->setLeft(nullptr);
        }
        }
        if(curr == this->root_)
            this->root_ = nullptr;
        delete curr;
        this->printRoot(this->root_);
        removeFix(p, diff);
    }

}
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value> *n, signed char diff)
{
    if(n == nullptr)
        return;
    AVLNode<Key, Value> *p = n->getParent();
    signed char ndiff = 0;
    if(p != nullptr)
    {
        if(isLeftChild(p, n))
        {
            ndiff = 1;
        }
        else if(isRightChild(p, n))
        {
            ndiff = -1;
        }
    }
    if(diff == -1)
    {
        if(n->getBalance() + diff == -2)
        {
            AVLNode<Key, Value> *c = n->getLeft();
            if(c->getBalance() == -1)
            {
                rotateRight(n);
                n->setBalance((signed char) 0);
                c->setBalance((signed char) 0);
                removeFix(p, ndiff);
            }
            else if(c->getBalance() == 0)
            {
                rotateRight(n);
                n->setBalance((signed char) -1);
                c->setBalance((signed char) 1);
            }
            else if(c->getBalance() == 1)
            {
                AVLNode<Key, Value> *g = c->getRight();
                rotateLeft(c);
                rotateRight(n);
                if(g->getBalance() == 1)
                {
                    n->setBalance((signed char) 0);
                    c->setBalance((signed char) -1);
                    g->setBalance((signed char) 0);
                }
                else if(g->getBalance() == 0)
                {
                    n->setBalance((signed char) 0);
                    c->setBalance((signed char) 0);
                    g->setBalance((signed char) 0);
                }
                else if(g->getBalance() == -1)
                {
                    n->setBalance((signed char) 1);
                    c->setBalance((signed char) 0);
                    g->setBalance((signed char) 0);
                }
                removeFix(p, ndiff);
            }
        }
        else if(n->getBalance() + diff == -1)
        {
            n->setBalance((signed char) -1);
        }
        else if(n->getBalance() + diff == 0)
        {
            n->setBalance((signed char) 0);
            removeFix(p, ndiff);
        }
    }
    else if(diff == 1)
    {
        if(n->getBalance() + diff == 2)
        {
            AVLNode<Key, Value> *c = n->getRight();
            if(c->getBalance() == 1)
            {
                rotateLeft(n);
                n->setBalance((signed char) 0);
                c->setBalance((signed char) 0);
                removeFix(p, ndiff);
            }
            else if(c->getBalance() == 0)
            {
                rotateLeft(n);
                n->setBalance((signed char) 1);
                c->setBalance((signed char) -1);
            }
            else if(c->getBalance() == -1)
            {
                AVLNode<Key, Value> *g = c->getLeft();
                rotateRight(c);
                rotateLeft(n);
                if(g->getBalance() == -1)
                {
                    n->setBalance((signed char) 0);
                    c->setBalance((signed char) 1);
                    g->setBalance((signed char) 0);
                }
                else if(g->getBalance() == 0)
                {
                    n->setBalance((signed char) 0);
                    c->setBalance((signed char) 0);
                    g->setBalance((signed char) 0);
                }
                else if(g->getBalance() == 1)
                {
                    n->setBalance((signed char) -1);
                    c->setBalance((signed char) 0);
                    g->setBalance((signed char) 0);
                }
                removeFix(p, ndiff);
            }
        }
        else if(n->getBalance() + diff == 1)
        {
            n->setBalance((signed char) 1);
        }
        else if(n->getBalance() + diff == 0)
        {
            n->setBalance((signed char) 0);
            removeFix(p, ndiff);
        }
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    signed char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* x){
    AVLNode<Key,Value>* p;
    AVLNode<Key,Value>* g;
    //AVLNode<Key,Value>* n;
    AVLNode<Key,Value>* gg; 
    AVLNode<Key,Value>* c;
    g = x;
    gg = g->getParent();
    p = g->getLeft();
    c = p->getRight();

    if(g == this->root_)
    {
        this->root_ = p;
    }
    g->setLeft(c);
    if(p->getRight() != nullptr)
    {
        c->setParent(g);
    }
    g->setParent(p);
    p->setRight(g);
    p->setParent(gg);
    if(gg != nullptr)
    {
        if(gg->getLeft() == g)
            gg->setLeft(p);
        else if(gg->getRight() == g)
            gg->setRight(p);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* x){
    AVLNode<Key,Value>* p;
    AVLNode<Key,Value>* g;
    //AVLNode<Key,Value>* n;
    AVLNode<Key,Value>* gg; 
    AVLNode<Key,Value>* c;
    g = x;
    gg = g->getParent();
    p = g->getRight();
    c = p->getLeft();
    if(g == this->root_)
    {
        this->root_ = p;
    }
    g->setRight(c);
    if(p->getLeft() != nullptr)
    {
        c->setParent(g);
    }
    g->setParent(p);
    p->setLeft(g);
    p->setParent(gg);
    if(gg != nullptr)
    {
        if(gg->getLeft() == g)
            gg->setLeft(p);
        else if(gg->getRight() == g)
            gg->setRight(p);
    }
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isLeftChild(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n){
    if(p != nullptr  && n != nullptr){
        if(p->getLeft() == n && n->getParent() == p)
            return true;
        else 
            return false;
    }
    else
        return false;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isRightChild(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n){
    if(p != nullptr  && n != nullptr){
        if(p->getRight() == n && n->getParent() == p)
            return true;
        else 
            return false;
    }
    else
        return false;
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n){
    if(p == nullptr)
        return;
    AVLNode<Key,Value>* g = p->getParent();
    //std::cout << n->getKey() <<" balance: " << (int)n->getBalance() << std::endl;
    //std::cout << p->getKey() <<" balance: " << (int)p->getBalance() << std::endl;
    if(g == nullptr){
        return;
        //std::cout << g->getKey() <<" balance: " << (int)g->getBalance() << std::endl;
    }
    if(isLeftChild(g, p)){
        g->updateBalance((signed char)-1);
        if(g->getBalance() == 0)
            return;
        else if(g->getBalance() == -1)
            insertFix(g, p);
        else if(g->getBalance() == -2)
        {
            if(isLeftChild(p, n)){
                rotateRight(g);
                p->setBalance((signed char) 0);
                g->setBalance((signed char) 0);
            }
            else if(isRightChild(p, n)){
                rotateLeft(p);
                rotateRight(g);
                if(p->getKey() > g->getKey())
                    std::cout << "wtfleft" << std::endl;
                if(n->getBalance() == -1)
                {
                    p->setBalance((signed char) 0);
                    g->setBalance((signed char) 1);
                    n->setBalance((signed char) 0); 
                }
                else if(n->getBalance() == 0){
                    p->setBalance((signed char) 0);
                    g->setBalance((signed char) 0);
                    n->setBalance((signed char) 0); 
                }
                else if(n->getBalance() == 1){
                    p->setBalance((signed char) -1);
                    g->setBalance((signed char) 0);
                    n->setBalance((signed char) 0); 
                }
            }
        }
    }
    else if(isRightChild(g, p)){
        //std::cout << " we out heeere" << std::endl;
        g->updateBalance((signed char)1);
        if(g->getBalance() == 0)
            return;
        else if(g->getBalance() == 1)
            insertFix(g, p);
        else if(g->getBalance() == 2)
        {
            if(isRightChild(p, n)){
                //std::cout << " we out heeere again" << std::endl;
                rotateLeft(g);
                p->setBalance((signed char) 0);
                g->setBalance((signed char) 0);
            }
            else if(isLeftChild(p, n)){
                rotateRight(p);
                rotateLeft(g);
                if(p->getKey() < g->getKey())
                    std::cout << "wtfright" << std::endl;
                if(n->getBalance() == 1)
                {
                    p->setBalance((signed char) 0);
                    g->setBalance((signed char) -1);
                    n->setBalance((signed char) 0); 
                }
                else if(n->getBalance() == 0){
                    p->setBalance((signed char) 0);
                    g->setBalance((signed char) 0);
                    n->setBalance((signed char) 0); 
                }
                else if(n->getBalance() == -1){
                    p->setBalance((signed char) 1);
                    g->setBalance((signed char) 0);
                    n->setBalance((signed char) 0); 
                }
            }
        }
    }
    

}


#endif
