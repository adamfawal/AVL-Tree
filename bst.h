#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed by the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}


/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree(); 
    virtual ~BinarySearchTree(); 
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); 
    virtual void remove(const Key& key); 
    void clear(); 
    bool isBalanced() const; 
    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator  
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;
    Value& operator[](const Key& key);
    Value const & operator[](const Key& key) const;

protected:
    //helper functions
    Node<Key, Value>* internalFind(const Key& k) const; 
    Node<Key, Value> *getSmallestNode() const;  
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); 
   

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here
    void clearHelp(Node<Key, Value>* curr);
    int height(Node<Key, Value>* curr) const;
    bool isBalancedHelp(Node<Key, Value>* curr) const;
    void removeHelp(Node<Key, Value>* curr);

protected:
    Node<Key, Value>* root_;
    
};



/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    
    current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() 
{
    
    current_ = NULL;
}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    
    // if(rhs->second == NULL || this->current_ == NULL)
    //     return false;
    // if( this->current_->getValue() == rhs->second)
    //     return true;
    // else
    //     return false;

    return current_ == rhs.current_;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    
    // if(rhs->second == NULL)
    //     return false;
    // if( this->current_->getValue() != rhs->second)
    //     return true;
    // else
    //     return false;
    return current_ != rhs.current_;
}


/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{


 if(current_->getRight() != nullptr)
    {
        Node<Key, Value> *curr = current_->getRight();
        while(curr->getLeft() != nullptr)
        {
            curr = curr->getLeft();
        }
        current_ = curr;
        return *this;
    }
    else{
        Node<Key, Value> *curr = current_;
        Node<Key, Value> *par = curr->getParent();
        while(curr->getParent() != nullptr)
        {
            if(par->getLeft() == curr)
                break;
            else{
                curr = curr->getParent();
                if(par != NULL)
                    par = par->getParent();
            }

        }
        if(curr->getParent() != nullptr)
            current_ = par;
        else
            current_ = nullptr;
        return *this;
    }


}




/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() 
{
    
    root_ = NULL;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    
    clear();
}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * @precondition The key exists in the map
 * Returns the value associated with the key
 */
template<class Key, class Value>
Value& BinarySearchTree<Key, Value>::operator[](const Key& key)
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}
template<class Key, class Value>
Value const & BinarySearchTree<Key, Value>::operator[](const Key& key) const
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
* Recall: If key is already in the tree, you should 
* overwrite the current value with the updated value.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    
    Node<Key, Value> *key = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, nullptr);
    Node<Key, Value> *curr = root_;
    Node<Key, Value> *prev = nullptr;
    if(root_ == NULL)
    {
        root_ = key;
        //std::cout << "no root: it is now: " << keyValuePair.first << std::endl;
        return;
    }
    else
    {
        int x = 0;
        while(curr != nullptr)
        {
            if(key->getKey() < curr->getKey())
            {
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
            //std::cout << "added left: " << keyValuePair.first << std::endl;
        }
        else if(x == 1)
        {
            curr->setParent(prev);
            prev->setRight(curr);
            //std::cout << "added right: " << keyValuePair.first << std::endl;
        }
        return;
    }
    


}


/**
* A remove method to remove a specific key from a Binary Search Tree.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    
    //no children
    Node<Key, Value> *curr = internalFind(key);
    removeHelp(curr);
    this->printRoot(this->root_); 
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::removeHelp(Node<Key, Value>* curr)
{
 if(curr != NULL){
        if(curr->getRight() == nullptr && curr->getLeft() == nullptr)
        {
            Node<Key, Value> *parent = curr->getParent();
            if(parent != nullptr)
            {
                if(parent->getRight() == curr)
                {
                    parent->setRight(nullptr);
                }
                else{
                    parent->setLeft(nullptr);
                }
            }
            else{
                root_ = nullptr;
            }
            delete curr;
        }
        else if(curr->getRight() != nullptr && curr->getLeft() == nullptr)
        {
            Node<Key, Value> *parent = curr->getParent();
            Node<Key, Value> *child = curr->getRight();
            if(parent != nullptr)
            {
                if(parent->getRight() == curr)
                {
                    parent->setRight(child);
                }
                else{
                    parent->setLeft(child);
                }
            }
            else{
                root_ = child;
            }
            child->setParent(parent);
            delete curr;
        }
        else if(curr->getRight() == nullptr && curr->getLeft() != nullptr)
        {
            Node<Key, Value> *parent = curr->getParent();
            Node<Key, Value> *child = curr->getLeft();
            if(parent != nullptr)
            {
                if(parent->getRight() == curr)
                {
                    parent->setRight(child);
                }
                else{
                    parent->setLeft(child);
                }
            }
            else{
                root_ = child;
            }
            child->setParent(parent);
            delete curr;
        }
        else{
            Node<Key, Value> *pred = this->predecessor(curr);
            this->nodeSwap(curr, pred);
            this->removeHelp(curr);
        }
    }
}



template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    
    if(current->getLeft() != nullptr)
    {
        Node<Key, Value> *curr = current->getLeft();
        while(curr->getRight() != nullptr)
        {
            curr = curr->getRight();
        }
        return curr;
    }
    else{
        Node<Key, Value> *curr = current;
        Node<Key, Value> *par = curr->getParent();
        while(curr->getParent() != nullptr)
        {
            if(par->getRight() == curr)
                break;
            else{
                curr = curr->getParent();
                if(par != NULL)
                    par = par->getParent();
            }

        }
        if(curr->getParent() != nullptr)
            return par;
        else
            return nullptr;
    }
}


/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    
    clearHelp(root_);
    root_ = nullptr;
}
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearHelp(Node<Key, Value>* curr)
{
    if(curr == nullptr)
        return;
    clearHelp(curr->getRight());
    clearHelp(curr->getLeft());
    //remove(curr->getKey());
    delete curr;
}
/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    
    if(root_ == NULL)
    {
        return nullptr;
    }
    else if (root_->getRight() == nullptr && root_->getLeft() == nullptr)
    {
        return root_;
    }
    else{
        Node<Key,Value> *curr = root_;
        Node<Key,Value> *prev = nullptr;
        while(curr != nullptr)
        {
            prev = curr;
            curr = curr->getLeft();
        }
        return prev;
    }
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    
    Node<Key, Value> *curr = root_;
    //bool found = false;
    while(curr != nullptr)
    {
        if(curr->getKey() < key)
        {
            //std::cout << curr->getKey() << std::endl;
            curr = curr->getRight();
        }
        else if(curr->getKey() > key)
        {
           // std::cout << curr->getKey() << std::endl;
            curr = curr->getLeft();
        }
        else{
           // std::cout << curr->getKey() << std::endl;
            return curr;
        }
    }
    return NULL;


}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    
    Node<Key, Value> *curr = root_;
    return isBalancedHelp(curr);
}
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalancedHelp(Node<Key, Value>* curr) const
{
    
    if(curr == nullptr)
        return true;
    
    int left = height(curr->getLeft());
    int right = height(curr->getRight());
    bool valid = false;
    bool valid2 = false;

    if(left < right && ((right - left) <= 1))
        valid = true;
    else if(left >= right && ((left - right) <= 1))
        valid = true;
    
    if(valid)
    {
        valid2 = isBalancedHelp(curr->getRight()) && isBalancedHelp(curr->getLeft());
        return valid2; 
    }
    else
        return false;
    
}

template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::height(Node<Key, Value>* curr) const
{
    if(curr == nullptr)
    {
        return 0;
    }
    else{
        int left = height(curr->getLeft());
        int right = height(curr->getRight());

        int bigger = 0;
        if(left >= right)
            bigger = left;
        else
            bigger = right;
        return 1 + bigger;
    }
}


template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}

#endif
