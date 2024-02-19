#ifndef BST_H
#define BST_H

#include <cstdlib>
#include <exception>
#include <iostream>
#include <utility>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template<typename Key, typename Value>
class Node {
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
    void setValue(const Value& value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
 * Explicit constructor for a node.
 */
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent)
        : item_(key, value), parent_(parent), left_(NULL), right_(NULL) {}

/**
 * Destructor, which does not need to do anything since the pointers inside of a node
 * are only used as references to existing nodes. The nodes pointed to by parent/left/right
 * are freed within the deleteAll() helper method in the BinarySearchTree.
 */
template<typename Key, typename Value>
Node<Key, Value>::~Node() {}

/**
 * A const getter for the item.
 */
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const {
    return item_;
}

/**
 * A non-const getter for the item.
 */
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem() {
    return item_;
}

/**
 * A const getter for the key.
 */
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const {
    return item_.first;
}

/**
 * A const getter for the value.
 */
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const {
    return item_.second;
}

/**
 * A non-const getter for the value.
 */
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue() {
    return item_.second;
}

/**
 * An implementation of the virtual function for retreiving the parent.
 */
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const {
    return parent_;
}

/**
 * An implementation of the virtual function for retreiving the left child.
 */
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const {
    return left_;
}

/**
 * An implementation of the virtual function for retreiving the right child.
 */
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const {
    return right_;
}

/**
 * A setter for setting the parent of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent) {
    parent_ = parent;
}

/**
 * A setter for setting the left child of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left) {
    left_ = left;
}

/**
 * A setter for setting the right child of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right) {
    right_ = right;
}

/**
 * A setter for the value of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value) {
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
 * A templated unbalanced binary search tree.
 */
template<typename Key, typename Value>
class BinarySearchTree {
public:
    BinarySearchTree();                                                    // TODO
    virtual ~BinarySearchTree();                                           // TODO
    virtual void insert(const std::pair<const Key, Value>& keyValuePair);  // TODO
    virtual void remove(const Key& key);                                   // TODO
    void clear();                                                          // TODO
    bool isBalanced() const;                                               // TODO
    void print() const;
    bool empty() const;

public:
    /**
     * An internal iterator class for traversing the contents of the BST.
     */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key, Value>& operator*() const;
        std::pair<const Key, Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key, Value>* ptr);
        Node<Key, Value>* current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const;               // TODO
    Node<Key, Value>* getSmallestNode() const;                        // TODO
    static Node<Key, Value>* predecessor(Node<Key, Value>* current);  // TODO
    void insertHelper(const std::pair<const Key, Value>& keyValuePair, Node<Key, Value>*);
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.
    static Node<Key, Value>* Successor(Node<Key, Value>* current);
    // Provided helper functions
    virtual void printRoot(Node<Key, Value>* r) const;
    virtual void nodeSwap(Node<Key, Value>* n1, Node<Key, Value>* n2);
    int isBalancedHelper(Node<Key, Value>* cur) const;
    // Add helper functions here

protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
 * Explicit constructor that initializes an iterator with a given node pointer.
 */
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key, Value>* ptr) : current_(ptr) {
    // TODO
    // current_ = ptr;
    // while(current_->getLeft() != NULL){
    //  current_ = current_->getLeft();
    //}
}

/**
 * A default constructor that initializes the iterator to NULL.
 */
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() : current_(NULL) {  // defaults current_ to NULL
}

/**
 * Provides access to the item.
 */
template<class Key, class Value>
std::pair<const Key, Value>& BinarySearchTree<Key, Value>::iterator::operator*() const {
    return current_->getItem();
}

/**
 * Provides access to the address of the item.
 */
template<class Key, class Value>
std::pair<const Key, Value>* BinarySearchTree<Key, Value>::iterator::operator->() const {
    return &(current_->getItem());
}

/**
 * Checks if 'this' iterator's internals have the same value
 * as 'rhs'
 */
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(const BinarySearchTree<Key, Value>::iterator& rhs) const {
    if (rhs.current_ == NULL || current_ == NULL) {  // checks for null
        if (rhs.current_ == current_) {
            return true;
        } else {
            return false;
        }
    }
    if (rhs.current_->getKey() == current_->getKey()) {  // otherwise compares keys
        return true;
    }
    return false;
}

/**
 * Checks if 'this' iterator's internals have a different value
 * as 'rhs'
 */
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(const BinarySearchTree<Key, Value>::iterator& rhs) const {
    if (rhs.current_ == NULL || current_ == NULL) {  // checks for null
        if (rhs.current_ == current_) {
            return false;
        } else {
            return true;
        }
    }
    if (rhs.current_->getKey() != current_->getKey()) {  // compares keys
        return true;
    }
    return false;
}

/**
 * Advances the iterator's location using an in-order sequencing
 */
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator& BinarySearchTree<Key, Value>::iterator::operator++() {
    if (current_->getRight() != NULL) {  // checks for cases where it has reached null
        current_ = current_->getRight();
        while (current_->getLeft() != NULL) {
            current_ = current_->getLeft();
        }
        return *this;
    } else {
        if (current_->getParent() == NULL) {
            current_ = NULL;
            return *this;
        }
        if (current_->getParent()->getRight() == current_) {  // finds next smallest value
            while (current_->getParent()->getRight() == current_) {
                current_ = current_->getParent();
                if (current_->getParent() == NULL) {
                    current_ = NULL;
                    return *this;
                }
            }
            current_ = current_->getParent();
            return *this;
        }
        current_ = current_->getParent();
        return *this;
    }
    return *this;
}

/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
 * Default constructor for a BinarySearchTree, which sets the root to NULL.
 */
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() : root_(NULL) {
    // defaults root_
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree() {
    clear();  // clears
}

/**
 * Returns true if tree is empty
 */
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const {
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const {
    printRoot(root_);
    std::cout << "\n";
}

/**
 * Returns an iterator to the "smallest" item in the tree
 */
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::begin() const {
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
 * Returns an iterator whose value means INVALID
 */
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::end() const {
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
 * Returns an iterator to the item with the given key, k
 * or the end iterator if k does not exist in the tree
 */
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::find(const Key& k) const {
    Node<Key, Value>* curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * An insert method to insert into a Binary Search Tree.
 * The tree will not remain balanced when inserting.
 */
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value>& keyValuePair) {
    // TODO
    Node<Key, Value>* temp = root_;
    if (temp == NULL) {
        // create node here to place in case of empty insert
        Node<Key, Value>* newnode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, temp);
        root_ = newnode;
        return;
    }
    if (keyValuePair.first > temp->getKey()) {
        if (temp->getRight() == NULL) {  // checks if empty right then adds
            Node<Key, Value>* newnode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, temp);
            temp->setRight(newnode);
            return;
        }
        temp = temp->getRight();
        return insertHelper(keyValuePair, temp);
    } else if (keyValuePair.first < temp->getKey()) {
        if (temp->getLeft() == NULL) {  // checks if empty left then adds
            Node<Key, Value>* newnode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, temp);
            temp->setLeft(newnode);
            return;
        }
        temp = temp->getLeft();
        return insertHelper(keyValuePair, temp);
    } else if (keyValuePair.first == temp->getKey()) {
        temp->setValue(keyValuePair.second);  // overwrites existing value
        return;
    }
}
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insertHelper(
        const std::pair<const Key, Value>& keyValuePair, Node<Key, Value>* temp) {
    if (keyValuePair.first > temp->getKey()) {
        if (temp->getRight() == NULL) {  // finds empty right and adds
            Node<Key, Value>* newnode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, temp);
            temp->setRight(newnode);
            return;
        }
        temp = temp->getRight();
        return insertHelper(keyValuePair, temp);
    } else if (keyValuePair.first < temp->getKey()) {
        if (temp->getLeft() == NULL) {  // finds empty left and adds
            Node<Key, Value>* newnode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, temp);
            temp->setLeft(newnode);
            return;
        }
        temp = temp->getLeft();
        return insertHelper(keyValuePair, temp);
    } else if (keyValuePair.first == temp->getKey()) {
        temp->setValue(keyValuePair.second);  // overwrites existing
        return;
    }
}

/**
 * A remove method to remove a specific key from a Binary Search Tree.
 * The tree may not remain balanced after removal.
 */
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key) {
    Node<Key, Value>* found = internalFind(key);
    if (found == NULL) {
        return;  // not in the tree
    }
    if (found->getLeft() == NULL && found->getRight() == NULL) {  // no children
        if (found == root_) {
            root_ = NULL;
        } else {
            if (found == (found->getParent())->getLeft()) {  // remove connections to the tree
                (found->getParent())->setLeft(NULL);
            } else {
                (found->getParent())->setRight(NULL);
            }
        }
        delete found;
        return;
    }
    if (found->getLeft() == NULL && found->getRight() != NULL) {  // only right child
        (found->getRight())->setParent(found->getParent());
        if (found->getParent() != NULL) {
            if (found == (found->getParent())->getLeft()) {
                (found->getParent())->setLeft(found->getRight());  // push up right child
            } else {
                (found->getParent())->setRight(found->getRight());
            }
        } else {
            root_ = found->getRight();
        }
        delete found;
        return;
    }
    Node<Key, Value>* successor = Successor(found);
    if (successor == NULL) {  // only left child
        if (found->getLeft() != NULL) {
            (found->getLeft())->setParent(found->getParent());
            if (found == root_) {
                root_ = found->getLeft();
                delete found;
                return;
            }
            (found->getParent())->setLeft(found->getLeft());  // if not the root then you must have a parent
            delete found;
            return;
        }
        if (found->getParent() != NULL) {  // if it has a parent
            if (found == (found->getParent())->getLeft()) {
                (found->getParent())->setLeft(NULL);  // remove connections
            } else {
                (found->getParent())->setRight(NULL);
            }
        } else {
            root_ = NULL;
        }
        delete found;
        return;
    }

    if (successor->getRight() != NULL) {                             // case with both children
        (successor->getRight())->setParent(successor->getParent());  // changes the successor right to be
    }
    if (successor->getParent() != NULL) {
        if (successor == ((successor->getParent())->getLeft())) {  // checks which side it is one
            (successor->getParent())->setLeft(successor->getRight());
        } else {
            (successor->getParent())->setRight(successor->getRight());
        }
    }

    if (found == root_) {  // if it is the root update root
        root_ = successor;
    }
    nodeSwap(found, successor);  // swap with successor

    delete found;  // delete the node
}

template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::Successor(Node<Key, Value>* current) {
    if (current->getRight() == NULL) {
        return NULL;
    }  // goes right once then goes left as far as it can
    Node<Key, Value>* temp = current->getRight();

    while (temp->getLeft() != NULL) {
        temp = temp->getLeft();
    }
    return temp;
}
template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current) {
    if (current->getLeft() == NULL) {
        return NULL;
    }  // goes left once and then right as far as it can
    Node<Key, Value>* temp = current->getLeft();
    while (temp->getRight() != NULL) {
        temp = temp->getRight();
    }
    return temp;
}

/**
 * A method to remove all contents of the tree and
 * reset the values in the tree for use again.
 */
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear() {
    Node<Key, Value>* tempparent;
    Node<Key, Value>* temp = getSmallestNode();
    while (temp != NULL) {              // deletes leaf nodes in
        if (temp->getLeft() != NULL) {  // as far left before moving right guarantees order of child before parent
            temp = temp->getLeft();
        } else if (temp->getRight() != NULL) {
            temp = temp->getRight();
        } else if (temp->getRight() == NULL && temp->getLeft() == NULL) {
            tempparent = temp->getParent();
            delete temp;  // deletes when a leaf node
            temp = tempparent;
        }
    }
}

/**
 * A helper function to find the smallest node in the tree.
 */
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestNode() const {
    Node<Key, Value>* temp = root_;
    if (root_ == NULL) {
        return root_;
    }
    while (temp->getLeft() != NULL) {  // continues left as far as it can
        temp = temp->getLeft();
    }
    return temp;
}

/**
 * Helper function to find a node with given key, k and
 * return a pointer to it or NULL if no item with that key
 * exists
 */
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const {
    Node<Key, Value>* temp = root_;
    while (temp != NULL) {  // moves left or right until at temp or reaches a NULL pointer
        if (temp->getKey() == key) {
            return temp;
        }
        if (temp->getKey() > key) {
            temp = temp->getLeft();
        } else if (temp->getKey() < key) {
            temp = temp->getRight();
        }
    }
    return temp;
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const {
    int leftheight = 1 + isBalancedHelper(root_->getLeft());
    int rightheight = 1 + isBalancedHelper(root_->getRight());
    if (leftheight == 0 || rightheight == 0) {
        return false;  // this is the imbalanced case continueing up the tree
    }
    if (leftheight >= rightheight) {
        if (leftheight - rightheight <= 1) {
            return true;  // at root still balanced
        }
        return false;  // is imbalanced
    } else {
        if (rightheight - leftheight <= 1) {
            return true;
        }
        return false;
    }
}
template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::isBalancedHelper(Node<Key, Value>* cur) const {
    if (cur == NULL) {
        return 1;  // starts at height of 1 to match AVL settup
    }
    int leftheight = 1 + isBalancedHelper(cur->getLeft());
    int rightheight = 1 + isBalancedHelper(cur->getRight());
    if (leftheight == 0 || rightheight == 0) {
        return -1;  // this is the imbalanced case continueing up the tree
    }
    if (leftheight >= rightheight) {
        if (leftheight - rightheight <= 1) {
            return leftheight;
        }
        return -1;  // is imbalanced
    } else {
        if (rightheight - leftheight <= 1) {
            return rightheight;
        }
        return -1;
    }
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap(Node<Key, Value>* n1, Node<Key, Value>* n2) {
    if ((n1 == n2) || (n1 == NULL) || (n2 == NULL)) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if (n1p != NULL && (n1 == n1p->getLeft()))
        n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if (n2p != NULL && (n2 == n2p->getLeft()))
        n2isLeft = true;

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

    if ((n1r != NULL && n1r == n2)) {
        n2->setRight(n1);
        n1->setParent(n2);
    } else if (n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    } else if (n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    } else if (n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);
    }

    if (n1p != NULL && n1p != n2) {
        if (n1isLeft)
            n1p->setLeft(n2);
        else
            n1p->setRight(n2);
    }
    if (n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if (n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if (n2p != NULL && n2p != n1) {
        if (n2isLeft)
            n2p->setLeft(n1);
        else
            n2p->setRight(n1);
    }
    if (n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if (n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }

    if (this->root_ == n1) {
        this->root_ = n2;
    } else if (this->root_ == n2) {
        this->root_ = n1;
    }
}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif
