#ifndef RBBST_H
#define RBBST_H

#include "bst.h"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>

struct KeyError {};

/**
 * A special kind of node for an AVL tree, which adds the height as a data member, plus
 * other additional helper functions. You do NOT need to implement any functionality or
 * add additional data members or helper functions.
 */
template<typename Key, typename Value>
class AVLNode : public Node<Key, Value> {
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight() const;
    void setHeight(int height);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int height_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
 * An explicit constructor to initialize the elements by calling the base class constructor.
 */
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
        : Node<Key, Value>(key, value, parent), height_(1) {}

/**
 * A destructor which does nothing.
 */
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode() {}

/**
 * A getter for the height of a AVLNode.
 */
template<class Key, class Value>
int AVLNode<Key, Value>::getHeight() const {
    return height_;
}

/**
 * A setter for the height of a AVLNode.
 */
template<class Key, class Value>
void AVLNode<Key, Value>::setHeight(int height) {
    height_ = height;
}

/**
 * An overridden function for getting the parent since a static_cast is necessary to make sure
 * that our node is a AVLNode.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const {
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
 * Overridden for the same reasons as above.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const {
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
 * Overridden for the same reasons as above.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const {
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}

/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/

template<class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value> {
public:
    virtual void insert(const std::pair<const Key, Value>& new_item);  // TODO
    virtual void remove(const Key& key);                               // TODO
protected:
    virtual void nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2);
    virtual void insertHelper(const std::pair<const Key, Value>& keyValuePair, AVLNode<Key, Value>* temp);
    void removeHelper(const Key& key, AVLNode<Key, Value>* temp);
    void AVLFixer(AVLNode<Key, Value>* edit);
    bool ZigZigLeft(AVLNode<Key, Value>* child);
    bool ZigZagLeft(AVLNode<Key, Value>* child);
    bool ZigZigRight(AVLNode<Key, Value>* child);
    bool ZigZagRight(AVLNode<Key, Value>* child);
    // Add helper functions here
};

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item) {
    // TODO
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);  // converts root to AVL node
    if (temp == NULL) {
        // create node here to place in case of empty insert
        AVLNode<Key, Value>* newnode = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
        this->root_ = newnode;
        return;
    }
    if (new_item.first > temp->getKey()) {
        if (temp->getRight() == NULL) {  // checks for empty right
            AVLNode<Key, Value>* newnode = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
            temp->setRight(newnode);  // adds and updates
            AVLFixer(newnode);
            return;
        }
        temp = temp->getRight();
        insertHelper(new_item, temp);
        return;
    } else if (new_item.first < temp->getKey()) {
        if (temp->getLeft() == NULL) {  // checks for empty left
            AVLNode<Key, Value>* newnode = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
            temp->setLeft(newnode);
            AVLFixer(newnode);  // adds and updates
            return;
        }
        temp = temp->getLeft();
        insertHelper(new_item, temp);
        return;
    } else if (new_item.first == temp->getKey()) {
        temp->setValue(new_item.second);  // overwrites if already in the tree
        return;
    }
}
template<class Key, class Value>
void AVLTree<Key, Value>::insertHelper(const std::pair<const Key, Value>& keyValuePair, AVLNode<Key, Value>* temp) {
    if (keyValuePair.first > temp->getKey()) {
        if (temp->getRight() == NULL) {  // checks for empty right
            AVLNode<Key, Value>* newnode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, temp);
            temp->setRight(newnode);  // add and check if updated
            AVLFixer(newnode);
            return;
        }
        temp = temp->getRight();
        insertHelper(keyValuePair, temp);
        return;
    } else if (keyValuePair.first < temp->getKey()) {
        if (temp->getLeft() == NULL) {  // checks for empty left
            AVLNode<Key, Value>* newnode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, temp);
            temp->setLeft(newnode);  // add and check if updated
            AVLFixer(newnode);
            return;
        }
        temp = temp->getLeft();
        insertHelper(keyValuePair, temp);
        return;
    } else if (keyValuePair.first == temp->getKey()) {  // update if already added
        temp->setValue(keyValuePair.second);
        return;
    }
    // do left and same :)
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    if (this->root_ == NULL) {
        return;
    }
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);  // converts to AVL node
    if (key > temp->getKey()) {
        if (temp->getRight() == NULL) {  // cannot find value
            return;
        }
        temp = temp->getRight();
        removeHelper(key, temp);
        return;
    }
    if (key < temp->getKey()) {
        if (temp->getLeft() == NULL) {  // cannot find value
            return;
        }
        temp = temp->getLeft();
        removeHelper(key, temp);  // keeps searching next node
        return;
    }

    if (key == temp->getKey()) {                                    // delete the root
        if (temp->getLeft() == NULL && temp->getRight() == NULL) {  // no child
            delete temp;
            this->root_ = NULL;
            return;
        }
        if (temp->getLeft() == NULL && temp->getRight() != NULL) {  // one child
            (temp->getRight())->setParent(temp->getParent());
            if (temp->getParent() != NULL) {
                if (temp == (temp->getParent())->getLeft()) {  // checks if their is a left node
                    (temp->getParent())->setLeft(temp->getRight());
                } else {
                    (temp->getParent())->setRight(temp->getRight());
                }
            }
            AVLNode<Key, Value>* fixnode = temp->getRight();
            delete temp;
            this->root_ = fixnode;

            return;
        }
        AVLNode<Key, Value>* successor = temp->getRight();  // if two children find the successor
        while (temp->getLeft() != NULL) {
            successor = successor->getLeft();
        }
        if (successor == NULL) {  // no successor
            if (temp->getLeft() != NULL) {
                (temp->getLeft())->setParent(temp->getParent());
                this->root_ = temp->getLeft();
                delete temp;
                return;
            }
            if (temp->getParent() != NULL) {
                if (temp == (temp->getParent())->getLeft()) {
                    (temp->getParent())->setLeft(NULL);
                } else {
                    (temp->getParent())->setRight(NULL);
                }
            }
            this->root_ = NULL;
            delete temp;
            return;
        }
        AVLNode<Key, Value>* fixpoint = successor->getRight();
        if (successor->getRight() != NULL) {
            (successor->getRight())->setParent(successor->getParent());  // changes the successor right to take its
                                                                         // place
        }
        if (successor->getParent() != NULL) {  // when parent exists update it too
            if (successor == ((successor->getParent())->getLeft())) {
                (successor->getParent())->setLeft(successor->getRight());
                if (fixpoint == NULL) {
                    fixpoint = (successor->getParent())->getRight();
                }
            } else {
                (successor->getParent())->setRight(successor->getRight());
                if (fixpoint == NULL) {
                    fixpoint = (successor->getParent())->getLeft();
                }
            }
        }
        nodeSwap(temp, successor);  // swap successor over

        delete temp;
        this->root_ = successor;
        if (fixpoint != NULL) {
            AVLFixer(fixpoint);  // fix it
        }
    }
}
template<class Key, class Value>
void AVLTree<Key, Value>::removeHelper(const Key& key, AVLNode<Key, Value>* temp) {
    if (key > temp->getKey()) {  // go right
        if (temp->getRight() == NULL) {
            return;
        }
        temp = temp->getRight();
        removeHelper(key, temp);
        return;
    }
    if (key < temp->getKey()) {  // go left
        if (temp->getLeft() == NULL) {
            return;
        }
        temp = temp->getLeft();
        removeHelper(key, temp);
        return;
    }
    if (key == temp->getKey()) {                                    // delete the node
        if (temp->getLeft() == NULL && temp->getRight() == NULL) {  // no children
            if (temp == (temp->getParent())->getLeft()) {           // is left child
                (temp->getParent())->setLeft(NULL);
                if ((temp->getParent())->getRight() == NULL) {
                    temp->getParent()->setHeight(1);  // update height of parent if only child
                    AVLNode<Key, Value>* fixnode = (temp->getParent());
                    delete temp;
                    AVLFixer(fixnode);
                    return;
                }
                AVLNode<Key, Value>* fixnode = (temp->getParent())->getRight();
                delete temp;
                AVLFixer(fixnode);  // check for rotations
                return;
            } else {  // is right child
                (temp->getParent())->setRight(NULL);
                if ((temp->getParent())->getLeft() == NULL) {
                    temp->getParent()->setHeight(1);  // update height of parent if only child
                    AVLNode<Key, Value>* fixnode = (temp->getParent());
                    delete temp;
                    AVLFixer(fixnode);
                    return;
                }
                AVLNode<Key, Value>* fixnode = (temp->getParent())->getLeft();
                delete temp;
                AVLFixer(fixnode);  // check for rotations
                return;
            }
        }
        if (temp->getLeft() == NULL && temp->getRight() != NULL) {  // only right child
            (temp->getRight())->setParent(temp->getParent());
            if (temp->getParent() != NULL) {
                if (temp == (temp->getParent())->getLeft()) {
                    (temp->getParent())->setLeft(temp->getRight());  // if left child update
                } else {
                    (temp->getParent())->setRight(temp->getRight());  // if right child update
                }
                AVLNode<Key, Value>* fixnode = temp->getRight();
                delete temp;
                if (fixnode->getRight() != NULL) {  // where to start fixing uneven nodes
                    AVLFixer(fixnode->getRight());
                } else if (fixnode->getLeft() != NULL) {
                    AVLFixer(fixnode->getLeft());
                } else {
                    AVLFixer(fixnode);
                }
            }
            return;
        }

        AVLNode<Key, Value>* successor = temp->getRight();  // finds successor
        while (temp->getLeft() != NULL) {
            successor = successor->getLeft();
        }
        if (successor == NULL) {
            if (temp->getLeft() != NULL) {  // if left child
                (temp->getLeft())->setParent(temp->getParent());
                (temp->getParent())->setLeft(temp->getLeft());  // if not the root then you must have a parent
                AVLNode<Key, Value>* fixnode = temp->getLeft();
                delete temp;
                AVLFixer(fixnode);
                return;
            }
            if (temp->getParent() != NULL) {
                if (temp == (temp->getParent())->getLeft()) {
                    (temp->getParent())->setLeft(NULL);
                }  // remove connection to deleted node
                else {
                    (temp->getParent())->setRight(NULL);
                }
            }
            delete temp;
            return;
        }
        AVLNode<Key, Value>* fixpoint = successor->getRight();
        if (successor->getRight() != NULL) {
            (successor->getRight())->setParent(successor->getParent());  // changes the successor right to be
        }
        if (successor->getParent() != NULL) {
            if (successor == ((successor->getParent())->getLeft())) {
                (successor->getParent())->setLeft(successor->getRight());
                if (fixpoint == NULL) {  // goes through all possible points that may exist where an uneven could occur
                    fixpoint = (successor->getParent())->getRight();
                }
            } else {
                (successor->getParent())->setRight(successor->getRight());
                if (fixpoint == NULL) {
                    fixpoint = (successor->getParent())->getLeft();
                }
            }
        }
        nodeSwap(temp, successor);
        delete temp;
        if (fixpoint != NULL) {  // if one exists we fix
            AVLFixer(fixpoint);
        }
    }
}
template<class Key, class Value>
void AVLTree<Key, Value>::AVLFixer(AVLNode<Key, Value>* edit) {
    int orientation;  // 0 for left 1 for right
    // finds the needed rotations to correct the AVLTree
    if (edit == NULL) {
        return;
    }
    AVLNode<Key, Value>* parent = edit->getParent();
    if (parent == NULL) {
        return;  // top of tree ends
    }
    AVLNode<Key, Value>* otherchild;
    if (parent->getLeft() == edit) {
        otherchild = parent->getRight();
        orientation = 0;
    }
    if (parent->getRight() == edit) {
        otherchild = parent->getLeft();
        orientation = 1;
    }
    if (otherchild == NULL) {
        if (edit->getHeight() > 1) {  // case for only the one child
            AVLNode<Key, Value>* right = edit->getRight();
            AVLNode<Key, Value>* left = edit->getLeft();
            if (orientation == 1) {  // is the right child
                if (left == NULL && right == NULL) {
                    edit->setHeight(1);
                    AVLFixer(parent);
                    return;
                } else if (left == NULL) {  // only right child
                    edit->setHeight(right->getHeight() + 1);
                    bool continued = ZigZigRight(right);
                    if (!continued) {
                        if (edit->getParent() != NULL)
                            AVLFixer(edit);
                        return;
                    }
                    return;
                } else if (right == NULL) {  // only left child
                    edit->setHeight(left->getHeight() + 1);
                    bool continued = ZigZagRight(left);
                    if (!continued) {
                        if (left->getParent() != NULL)
                            AVLFixer(left);
                        return;
                    }
                    return;
                } else if (right->getHeight() >= left->getHeight()) {  // right is taller
                    // zigzig case!
                    edit->setHeight(right->getHeight() + 1);
                    bool continued = ZigZigRight(right);
                    if (!continued) {
                        if (edit->getParent() != NULL)
                            AVLFixer(edit);
                        return;
                    }
                    return;
                } else {  // left is taller
                    // zigzag case!
                    edit->setHeight(left->getHeight() + 1);
                    bool continued = ZigZagRight(left);
                    if (!continued) {
                        if (left->getParent() != NULL)
                            AVLFixer(left);
                        return;
                    }
                    return;
                }
            } else {                                  // is the left child
                if (left == NULL && right == NULL) {  // no child
                    edit->setHeight(1);
                    AVLFixer(parent);
                    return;
                } else if (left == NULL) {  // only right
                    edit->setHeight(right->getHeight() + 1);
                    bool continued = ZigZagLeft(right);
                    if (!continued) {
                        if (right->getParent() != NULL)
                            AVLFixer(right);
                        return;
                    }
                    return;
                } else if (right == NULL) {  // only left
                    edit->setHeight(left->getHeight() + 1);
                    bool continued = ZigZigLeft(left);
                    if (!continued) {
                        if (edit->getParent() != NULL)
                            AVLFixer(edit);
                        return;
                    }
                    // call function again if totalled unbalanced

                    return;
                } else if (left->getHeight() >= right->getHeight()) {  // left taller
                    edit->setHeight(left->getHeight() + 1);
                    // zigzig case!
                    bool continued = ZigZigLeft(left);
                    if (!continued) {
                        if (edit->getParent() != NULL)
                            AVLFixer(edit);
                        return;
                    }
                    // call function again if totalled unbalanced
                    return;
                } else {  // right taller
                    // zigzag case!
                    edit->setHeight(right->getHeight() + 1);
                    bool continued = ZigZagLeft(right);
                    if (!continued) {
                        if (right->getParent() != NULL)
                            AVLFixer(right);
                        return;
                    }
                    return;
                }
            }
        } else {  // if balanced
            if (parent->getHeight() == (edit->getHeight() + 1)) {
                return;
            }
            parent->setHeight(edit->getHeight() + 1);  // not? continue
            AVLFixer(parent);
            return;
        }
    } else if (abs(edit->getHeight() - otherchild->getHeight()) > 1) {
        if (edit->getHeight() > otherchild->getHeight()) {  // this is the case in insert
            AVLNode<Key, Value>* right = edit->getRight();
            AVLNode<Key, Value>* left = edit->getLeft();
            if (orientation == 1) {                   // is the right child
                if (left == NULL && right == NULL) {  // no children
                    edit->setHeight(1);
                    AVLFixer(parent);
                    return;
                } else if (left == NULL) {  // only right
                    bool continued = ZigZigRight(right);
                    if (!continued) {
                        if (edit->getParent() != NULL)
                            AVLFixer(edit);
                        return;
                    }
                    return;
                } else if (right == NULL) {  // only left
                    bool continued = ZigZagRight(left);
                    if (!continued) {
                        if (left->getParent() != NULL)
                            AVLFixer(left);
                        return;
                    }
                    return;
                } else if (right->getHeight() >= left->getHeight()) {  // right taller
                    // zigzig case!
                    bool continued = ZigZigRight(right);
                    if (!continued) {
                        if (edit->getParent() != NULL)
                            AVLFixer(edit);
                        return;
                    }
                    return;
                } else {  // left taller
                    // zigzag case!
                    bool continued = ZigZagRight(left);
                    if (!continued) {
                        if (left->getParent() != NULL)
                            AVLFixer(left);
                        return;
                    }
                    return;
                }
            } else {  // is the left child
                if (left == NULL && right == NULL) {
                    edit->setHeight(1);
                    AVLFixer(parent);
                    return;
                } else if (left == NULL) {  // only right
                    bool continued = ZigZagLeft(right);
                    if (!continued) {
                        if (right->getParent() != NULL)
                            AVLFixer(right);
                        return;
                    }
                    return;
                } else if (right == NULL) {  // only left
                    bool continued = ZigZigLeft(left);
                    if (!continued) {
                        if (edit->getParent() != NULL)
                            AVLFixer(edit);
                        return;
                    }
                    // call function again if totalled unbalanced
                    return;
                } else if (left->getHeight() >= right->getHeight()) {  // left taller
                    // zigzig case!
                    bool continued = ZigZigLeft(left);
                    if (!continued) {
                        if (edit->getParent() != NULL)
                            AVLFixer(edit);
                        return;
                    }
                    // call function again if totalled unbalanced
                    return;
                } else {  // right taller
                    // zigzag case!
                    bool continued = ZigZagLeft(right);
                    if (!continued) {
                        if (right->getParent() != NULL)
                            AVLFixer(right);
                        return;
                    }
                    return;
                }
            }
        } else if (edit->getHeight() < otherchild->getHeight()) {  // this is the case for delete
            AVLNode<Key, Value>* right = otherchild->getRight();
            AVLNode<Key, Value>* left = otherchild->getLeft();
            if (orientation == 1) {                   // otherchild is the left child then
                if (left == NULL && right == NULL) {  // no child
                    otherchild->setHeight(1);
                    AVLFixer(parent);
                    return;
                } else if (left == NULL) {  // only right child
                    bool continued = ZigZagLeft(right);
                    if (!continued) {
                        if (right->getParent() != NULL)
                            AVLFixer(right);
                        return;
                    }
                    return;
                } else if (right == NULL) {  // only left
                    bool continued = ZigZigLeft(left);
                    if (!continued) {
                        if (otherchild->getParent() != NULL)
                            AVLFixer(otherchild);
                        return;
                    }
                    return;
                }
                if (left->getHeight() >= right->getHeight()) {
                    // zigzig case!
                    bool continued = ZigZigLeft(left);
                    if (!continued) {
                        if (otherchild->getParent() != NULL)
                            AVLFixer(otherchild);
                        return;
                    }
                    return;
                } else {  // right taller
                    // zigzag case!
                    bool continued = ZigZagLeft(right);
                    if (!continued) {
                        if (right->getParent() != NULL)
                            AVLFixer(right);
                        return;
                    }
                    return;
                }
            } else {                                  // other child is the right child
                if (left == NULL && right == NULL) {  // no children
                    otherchild->setHeight(1);
                    AVLFixer(parent);
                    return;
                } else if (left == NULL) {
                    bool continued = ZigZigRight(right);
                    if (!continued) {
                        if (otherchild->getParent() != NULL)
                            AVLFixer(otherchild);
                        return;
                    }
                    return;
                } else if (right == NULL) {
                    bool continued = ZigZagRight(left);
                    if (!continued) {
                        if (left->getParent() != NULL)
                            AVLFixer(left);
                        return;
                    }
                    return;
                }
                if (right->getHeight() >= left->getHeight()) {
                    // zigzig case!
                    bool continued = ZigZigRight(right);
                    if (!continued) {
                        if (otherchild->getParent() != NULL)
                            AVLFixer(otherchild);
                        return;
                    }
                    return;
                } else {  // left taller
                    // zigzag case!
                    bool continued = ZigZagRight(left);
                    if (!continued) {
                        if (otherchild->getParent() != NULL)
                            AVLFixer(otherchild);
                        return;
                    }
                    return;
                }
            }
        }
    } else {  // if within 1 of each other
        if (edit->getHeight() > otherchild->getHeight()) {
            if (parent->getHeight() == (edit->getHeight() + 1)) {
                return;
            }
            parent->setHeight(edit->getHeight() + 1);
            AVLFixer(parent);
            return;
        }
        if (parent->getHeight() == (otherchild->getHeight() + 1)) {
            return;
        }
        parent->setHeight(otherchild->getHeight() + 1);
        AVLFixer(parent);
        return;
    }
}
template<class Key, class Value>
bool AVLTree<Key, Value>::ZigZigLeft(AVLNode<Key, Value>* child) {
    AVLNode<Key, Value>* middle = child->getParent();
    AVLNode<Key, Value>* top = middle->getParent();  // gets the three we are swapping
    int ogheight = top->getHeight();
    if (top->getParent() != NULL) {
        if (top == (top->getParent())->getLeft()) {  // updates all relevant pointers
            (top->getParent())->setLeft(middle);
        } else {
            (top->getParent())->setRight(middle);
        }
    }
    if (middle->getRight() != NULL) {
        (middle->getRight())->setParent(top);
    }
    middle->setParent(top->getParent());
    top->setLeft(middle->getRight());
    middle->setRight(top);
    top->setParent(middle);
    if (top->getLeft() == NULL && top->getRight() == NULL) {  // check if heights have changed
        top->setHeight(1);
    } else if (top->getLeft() == NULL && top->getRight() != NULL) {
        top->setHeight((top->getRight())->getHeight() + 1);
    } else if (top->getLeft() != NULL && top->getRight() == NULL) {
        top->setHeight((top->getLeft())->getHeight() + 1);
    } else if ((top->getLeft())->getHeight() > (top->getRight())->getHeight()) {
        top->setHeight((top->getLeft())->getHeight() + 1);
    } else {
        top->setHeight((top->getRight())->getHeight() + 1);
    }

    if (top->getHeight() > child->getHeight()) {
        middle->setHeight(top->getHeight() + 1);
    } else {
        middle->setHeight(child->getHeight() + 1);
    }
    if (top == this->root_) {
        this->root_ = middle;
    }
    return middle->getHeight() == ogheight;  // if height has changed return false and continue
}
template<class Key, class Value>
bool AVLTree<Key, Value>::ZigZagLeft(AVLNode<Key, Value>* child) {
    AVLNode<Key, Value>* middle = child->getParent();  // gets all three nodes
    AVLNode<Key, Value>* top = middle->getParent();
    int ogheight = top->getHeight();
    middle->setRight(child->getLeft());
    if (child->getLeft() != NULL) {  // update relevant pointers
        (child->getLeft())->setParent(middle);
    }
    top->setLeft(child->getRight());
    if (child->getRight() != NULL) {
        (child->getRight())->setParent(top);
    }
    if (top->getParent() != NULL) {
        if (top == (top->getParent())->getLeft()) {
            (top->getParent())->setLeft(child);
        } else {
            (top->getParent())->setRight(child);
        }
    }
    child->setParent(top->getParent());  // overwrite pointers to pre saved nodes
    child->setLeft(middle);
    child->setRight(top);
    middle->setParent(child);
    top->setParent(child);
    // update all the heights
    if (top->getLeft() == NULL && top->getRight() == NULL) {  // check for changed heights
        top->setHeight(1);
    } else if (top->getLeft() == NULL && top->getRight() != NULL) {
        top->setHeight((top->getRight())->getHeight() + 1);
    } else if (top->getLeft() != NULL && top->getRight() == NULL) {
        top->setHeight((top->getLeft())->getHeight() + 1);
    } else if ((top->getLeft())->getHeight() > (top->getRight())->getHeight()) {
        top->setHeight((top->getLeft())->getHeight() + 1);
    } else {
        top->setHeight((top->getRight())->getHeight() + 1);
    }

    if (middle->getLeft() == NULL && middle->getRight() == NULL) {
        middle->setHeight(1);
    } else if (middle->getLeft() == NULL && middle->getRight() != NULL) {
        middle->setHeight((middle->getRight())->getHeight() + 1);
    } else if (middle->getLeft() != NULL && middle->getRight() == NULL) {
        middle->setHeight((middle->getLeft())->getHeight() + 1);
    } else if ((middle->getLeft())->getHeight() > (middle->getRight())->getHeight()) {
        middle->setHeight((middle->getLeft())->getHeight() + 1);
    } else {
        middle->setHeight((middle->getRight())->getHeight() + 1);
    }
    if (top == this->root_) {
        this->root_ = child;
    }

    if (top->getHeight() > middle->getHeight()) {
        child->setHeight(top->getHeight() + 1);
    } else {
        child->setHeight(middle->getHeight() + 1);
    }
    return child->getHeight() == ogheight;  // if changed continue fixing
}
template<class Key, class Value>
bool AVLTree<Key, Value>::ZigZigRight(AVLNode<Key, Value>* child) {
    AVLNode<Key, Value>* middle = child->getParent();
    AVLNode<Key, Value>* top = middle->getParent();  // gets three points
    int ogheight = top->getHeight();
    if (top->getParent() != NULL) {
        if (top == (top->getParent())->getLeft()) {  // updates pointers acordingly
            (top->getParent())->setLeft(middle);
        } else {
            (top->getParent())->setRight(middle);
        }
    }
    if (middle->getLeft() != NULL) {
        (middle->getLeft())->setParent(top);
    }
    middle->setParent(top->getParent());
    top->setRight(middle->getLeft());
    middle->setLeft(top);
    top->setParent(middle);
    if (top->getLeft() == NULL && top->getRight() == NULL) {  // update any heights that change
        top->setHeight(1);
    } else if (top->getLeft() == NULL && top->getRight() != NULL) {
        top->setHeight((top->getRight())->getHeight() + 1);
    } else if (top->getLeft() != NULL && top->getRight() == NULL) {
        top->setHeight((top->getLeft())->getHeight() + 1);
    } else if ((top->getLeft())->getHeight() > (top->getRight())->getHeight()) {
        top->setHeight((top->getLeft())->getHeight() + 1);
    } else {
        top->setHeight((top->getRight())->getHeight() + 1);
    }
    if (top->getHeight() > child->getHeight()) {
        middle->setHeight(top->getHeight() + 1);
    } else {
        middle->setHeight(child->getHeight() + 1);
    }
    if (top == this->root_) {
        this->root_ = middle;
    }
    return middle->getHeight() == ogheight;  // if tree changes update and continue
}
template<class Key, class Value>
bool AVLTree<Key, Value>::ZigZagRight(AVLNode<Key, Value>* child) {
    AVLNode<Key, Value>* middle = child->getParent();
    AVLNode<Key, Value>* top = middle->getParent();  // get all relevant nodes
    int ogheight = top->getHeight();
    middle->setLeft(child->getRight());
    if (child->getRight() != NULL) {
        (child->getRight())->setParent(middle);
    }
    top->setRight(child->getLeft());  // update pointers
    if (child->getLeft() != NULL) {
        (child->getLeft())->setParent(top);
    }
    if (top->getParent() != NULL) {
        if (top == (top->getParent())->getLeft()) {
            (top->getParent())->setLeft(child);
        } else {
            (top->getParent())->setRight(child);
        }
    }
    child->setParent(top->getParent());
    child->setRight(middle);
    child->setLeft(top);
    middle->setParent(child);
    top->setParent(child);
    // update all the heights
    if (top->getLeft() == NULL && top->getRight() == NULL) {
        top->setHeight(1);
    } else if (top->getLeft() == NULL && top->getRight() != NULL) {
        top->setHeight((top->getRight())->getHeight() + 1);
    } else if (top->getLeft() != NULL && top->getRight() == NULL) {
        top->setHeight((top->getLeft())->getHeight() + 1);
    } else if ((top->getLeft())->getHeight() > (top->getRight())->getHeight()) {
        top->setHeight((top->getLeft())->getHeight() + 1);
    } else {
        top->setHeight((top->getRight())->getHeight() + 1);
    }

    if (middle->getLeft() == NULL && middle->getRight() == NULL) {
        middle->setHeight(1);
    } else if (middle->getLeft() == NULL && middle->getRight() != NULL) {
        middle->setHeight((middle->getRight())->getHeight() + 1);
    } else if (middle->getLeft() != NULL && middle->getRight() == NULL) {
        middle->setHeight((middle->getLeft())->getHeight() + 1);
    } else if ((middle->getLeft())->getHeight() > (middle->getRight())->getHeight()) {
        middle->setHeight((middle->getLeft())->getHeight() + 1);
    } else {
        middle->setHeight((middle->getRight())->getHeight() + 1);
    }
    if (top == this->root_) {
        this->root_ = child;
    }
    if (top->getHeight() > middle->getHeight()) {
        child->setHeight(top->getHeight() + 1);
    } else {
        child->setHeight(middle->getHeight() + 1);
    }
    return child->getHeight() == ogheight;  // if changes continue changing up the tree
}
template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2) {
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int tempH = n1->getHeight();
    n1->setHeight(n2->getHeight());
    n2->setHeight(tempH);
}

#endif
