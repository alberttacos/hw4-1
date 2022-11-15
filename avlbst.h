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
    void insertFix( AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void rotateRight( AVLNode<Key,Value>* point);
    void rotateLeft( AVLNode<Key,Value>* point);
    void BSTInsert(const std::pair<const Key, Value> &keyValuePair);
    void deleteAVLNode(AVLNode<Key,Value>* cur);
    void removeFix(AVLNode<Key,Value>* n,int diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (BinarySearchTree<Key, Value>::empty()){
        AVLNode<Key, Value>* new_root=new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        this->root_ = new_root;
        new_root->setBalance(0);
    }
    else {
        BSTInsert(new_item);
        AVLNode<Key, Value>* new_item_node = static_cast<AVLNode<Key,Value>*>(BinarySearchTree<Key, Value>::internalFind(new_item.first));
        new_item_node->setBalance(0);
        if (new_item_node->getParent()){
            if (new_item_node->getParent()->getBalance()==-1){
                new_item_node->getParent()->setBalance(0);
            }
            else if ((new_item_node->getParent())->getBalance()==1){
                new_item_node->getParent()->setBalance(0);
            }
            else if (new_item_node->getParent()->getBalance()==0){
                if (new_item_node->getParent()->getLeft()==new_item_node){
                    new_item_node->getParent()->setBalance(-1);
                    insertFix(new_item_node->getParent(),new_item_node);
                }
                else if (new_item_node->getParent()->getRight()==new_item_node){
                    new_item_node->getParent()->setBalance(1);
                    insertFix(new_item_node->getParent(),new_item_node);
                }
                
            }
        }
    }
    

    //find x, y, and z
    //call LL,RR,LR,RL
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix( AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
    if (p==NULL){
        return;
    }
    if (p->getParent()==NULL){
        return;
    }
    AVLNode<Key,Value>* g=p->getParent();
    if (g->getLeft()==p){
        g->updateBalance(-1);
        if (g->getBalance()==0){
            return;
        }
        else if (g->getBalance()==-1){
            insertFix(g,p);
        }
        else if(g->getBalance()==-2){
            if (p->getLeft()==n){
                rotateRight(g);//complete this function
                p->setBalance(0);
                g->setBalance(0);
            }
            else if (p->getRight()==n){
                rotateLeft(p);
                rotateRight(g);
                
                if(n->getBalance()==-1){

                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                }
                else if(n->getBalance()==0){
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance()==1){
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
    else if(g->getRight()==p){
        g->updateBalance(1);
        if (g->getBalance()==0){
            return;
        }
        else if (g->getBalance()==1){
            insertFix(g,p);
        }
        else if(g->getBalance()==2){
            if (p->getRight()==n){
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else if (p->getLeft()==n){
                rotateRight(p);
                rotateLeft(g);
                if(n->getBalance()==1){

                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                }
                else if(n->getBalance()==0){
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance()==-1){
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
}



template<class Key, class Value>
void AVLTree<Key, Value>::BSTInsert(const std::pair<const Key, Value> &keyValuePair)
{
    // TODO
    AVLNode<Key, Value>* cur = static_cast<AVLNode<Key,Value>*>(this->root_);
    if (cur==NULL){
        this->root_=new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
        return;
    }
 
    while (cur!=NULL){
        if (keyValuePair.first<cur->getKey()){
            if (cur->getLeft()){
                cur=cur->getLeft();
            }
            else {
                AVLNode<Key, Value>* newNode=new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, cur);
                cur->setLeft(newNode);
                break;
            }
        }
        else if (keyValuePair.first>cur->getKey()){
            if (cur->getRight()){
                cur=cur->getRight();
            }
            else{
                AVLNode<Key, Value>* newNode=new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, cur);
                cur->setRight(newNode);
                break;
            }
        }
        else if (keyValuePair.first==cur->getKey()){
            cur->setValue(keyValuePair.second);
            break;
        }
    }
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight( AVLNode<Key,Value>* point) {
    AVLNode<Key,Value>* mainParent = point->getParent();
    AVLNode<Key,Value>* b= point->getLeft();
    AVLNode<Key,Value>* aLeft = b->getRight();
    if (point==this->root_){
        this->root_=b;
    }
    b->setRight(point);
    point->setParent(b);
    b->setParent(mainParent);
    if (mainParent){ //fix this
        if (mainParent->getRight()==point){
            mainParent->setRight(b);
        }
        else if (mainParent->getLeft()==point){
            mainParent->setLeft(b);
        }
    }
    point->setLeft(aLeft);
    if (aLeft){
        aLeft->setParent(point);
    }
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft( AVLNode<Key,Value>* point) {

    AVLNode<Key,Value>* mainParent = point->getParent();
    AVLNode<Key,Value>* b= point->getRight();
    AVLNode<Key,Value>* aRight = b->getLeft();
    if (point==this->root_){
        this->root_=b;
    }
    b->setLeft(point);
    point->setParent(b);
    b->setParent(mainParent);
    if (mainParent){ //fix this
        if (mainParent->getRight()==point){
            mainParent->setRight(b);
        }
        else if (mainParent->getLeft()==point){
            mainParent->setLeft(b);
        }
    }
    point->setRight(aRight);
    if (aRight) {
        aRight->setParent(point);
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
    AVLNode<Key, Value>* cur = static_cast<AVLNode<Key,Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
    int diff=0;
    if (cur==NULL){
        return;
    }
    while (cur->getRight() && cur->getLeft()){
        
        AVLNode<Key, Value>* prev=static_cast<AVLNode<Key,Value>*>(BinarySearchTree<Key, Value>::predecessor(cur));
        this->nodeSwap(prev,cur);//is this the correct syntax?
    }
    AVLNode<Key, Value>* parent = cur->getParent();
    if (parent!=NULL){
        if (parent->getLeft()==cur){
            diff=1;
        }
        else if (parent->getRight()==cur){
            diff=-1;
        }
    }

    if (cur->getRight()==NULL && cur->getLeft()==NULL){
        this->deleteAVLNode(cur);
    }
    else if (cur->getLeft()){
        AVLNode<Key, Value>* newNode=cur->getLeft();
        if (cur->getParent()){
            newNode->setParent(cur->getParent());
            if (cur->getParent()->getRight()==cur){
                cur->getParent()->setRight(newNode);
            }
            else if (cur->getParent()->getLeft()==cur){
                cur->getParent()->setLeft(newNode);
            }
        }
        else {
            newNode->setParent(NULL);
            this->root_=newNode;
        }
        delete cur;
    }
    else if (cur->getRight()){
        AVLNode<Key, Value>* newNode=cur->getRight();
         if (cur->getParent()){
            newNode->setParent(cur->getParent());
            if (cur->getParent()->getRight()==cur){
                cur->getParent()->setRight(newNode);
            }
            else if (cur->getParent()->getLeft()==cur){
                cur->getParent()->setLeft(newNode);
            }
        }
        else {
            newNode->setParent(NULL);
            this->root_=newNode;
        }
        delete cur;
    }
    removeFix(parent,diff);
}
template<typename Key, typename Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n,int diff) {
    if (n==NULL){
        return;
    }
    AVLNode<Key,Value>* p=n->getParent();
    int ndiff = 0;
    if (p){
        if (p->getRight()==n){
            ndiff=-1;
        }
        else if (p->getLeft()==n){
            ndiff=1;
        }
    }
    if (diff==-1){
        if (diff + n->getBalance()==-2){
            AVLNode<Key,Value>* c=n->getLeft();
            if (c->getBalance()==-1){
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p,ndiff);
            }
            else if (c->getBalance()==0){
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
            }
            else if (c->getBalance()==1){
                AVLNode<Key,Value>* g=c->getRight();
                rotateLeft(c);
                rotateRight(n);
                if (g->getBalance()==1){
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if (g->getBalance()==0){
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if (g->getBalance()==-1){
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        else if (diff + n->getBalance()==-1){
            n->setBalance(-1);
        }
        else if (diff + n->getBalance()==0){
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }

    else if (diff==1){
        if (diff + n->getBalance()==2){
            AVLNode<Key,Value>* c=n->getRight();//
            if (c->getBalance()==1){
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p,ndiff);
            }
            else if (c->getBalance()==0){
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
            }
            else if (c->getBalance()==-1){
                AVLNode<Key,Value>* g=c->getLeft();
                rotateRight(c);
                rotateLeft(n);
                if (g->getBalance()==-1){
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if (g->getBalance()==0){
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if (g->getBalance()==1){
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        else if (diff + n->getBalance()==1){
            n->setBalance(1);
        }
        else if (diff + n->getBalance()==0){
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::deleteAVLNode(AVLNode<Key,Value>* cur) {
    
    if (cur->getParent()){
        if (cur->getParent()->getRight()==cur){
            cur->getParent()->setRight(NULL);
        }
        else if(cur->getParent()->getLeft()==cur){
            cur->getParent()->setLeft(NULL);
        }
        delete cur;
    }
    else {
        delete cur;
        this->root_=NULL;
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
