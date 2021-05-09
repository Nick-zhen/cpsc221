#include "chain.h"
#include <cmath>
#include <iostream>


// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
  /* your code here */
  clear();
}

/**
 * Inserts a new node after the node pointed to by p in the
 * chain (so p->next is the new node) and returns a pointer to
 * the newly created node.
 * If p is NULL, inserts a new head node to the chain.
 * This function **SHOULD** create a new Node and increase length_.
 *
 * @param p = The new node should be pointed to by p->next.
 *            If p is NULL, the new node becomes the head of the chain.
 * @param ndata = The data to be inserted.
 */
Chain::Node * Chain::insertAfter(Node * p, const Block &ndata) {
  Node* newNode = new Node(ndata);
  //newNode -> data = ndata;
  if (p == NULL) {
    head_ = newNode;
  } else if (p -> next == NULL) {
    p -> next = newNode;
    newNode -> prev = p;
    newNode -> next = NULL;
  } else {
    newNode -> next = p -> next;
    (p -> next) -> prev = newNode;
    p -> next = newNode;
    newNode -> prev = p;
  }

  length_++;
  return newNode;
}

/**
 * Swaps the position in the chain of the two nodes pointed to
 * by p and q.
 * If p or q is NULL or p==q, do nothing.
 * Change the chain's head pointer if necessary.
 */
void Chain::swap(Node *p, Node *q) {
  if (p == NULL || q == NULL || p == q) {
    //do nothing
  } else if (q == head_ && p -> next ==NULL) {
    p -> prev -> next = q;
    p -> next = q -> next;
    q -> prev = p -> prev;
    q -> next -> prev = p;
    q -> next = NULL;
    p -> prev = NULL;
    head_ = p;
  } else if (p == head_ && q -> next == NULL) {
    q -> prev -> next = p;
    q -> next = p -> next;
    p -> prev = q -> prev;
    p -> next -> prev = q;
    p -> next = NULL;
    q -> prev = NULL;
    head_ = q;
  } else if (p == head_ && p -> next == q) {
    p -> next = q -> next;
    (q -> next) -> prev = p;
    q -> next = p;
    p -> prev = q;
    q -> prev = NULL;
    head_ = q;
  } else if (q == head_ && q -> next == p) {
    q -> next = p -> next;
    (p -> next) -> prev = q;
    p -> next = q;
    q -> prev = p;
    p -> prev = NULL;
    head_ = p;
  } else if (p == head_ && p -> next != q) {
    Node* helper = q -> next;
    (q -> prev) -> next = p;
    p -> prev = q -> prev;
    q -> next = p -> next;
    (p -> next) -> prev = q;
    p -> next = helper;
    helper -> prev = p;
    q -> prev = NULL;
    head_ = q;
   } else if (q == head_ && q -> next != p) {
    Node* helper = p -> next;
    (p -> prev) -> next = q;
    q -> prev = p -> prev;
    p -> next = q -> next;
    (q -> next) -> prev = p;
    q -> next = helper;
    helper -> prev = q;
    p -> prev = NULL;
    head_ = p;
   } else if ((q -> next == NULL) && (p -> next == q)) {

    (p -> prev) -> next = q;
    q -> prev = p -> prev;
    p -> prev = q;
    q -> next = p;
    p -> next = NULL;

   } else if (p -> next == NULL && q -> next == p) {
    (q -> prev) -> next = p;
    p -> prev = q -> prev;
    q -> prev = p;
    p -> next = q;
    q -> next = NULL;
   } else if (q -> next == NULL && p != head_) {
    Node* temp = q -> prev;
    (p -> prev) -> next = q;
    q -> prev = p -> prev;
    p -> prev = temp;
    q -> next = p -> next;
    p -> next -> prev = q;
    temp -> next = p;
    p -> next = NULL;
   } else if (p -> next == NULL && q != head_) {
    Node* temp = p -> prev;
    (q -> prev) -> next = p;
    p -> prev = q -> prev;
    q -> prev = temp;
    p -> next = q -> next;
    q -> next -> prev = p;
    temp -> next = q;
    q -> next = NULL;

   } else if (p -> next == q && p != head_) {
    Node* helper = q -> next;
    (p -> prev) -> next = q;
    q -> prev = p -> prev;
    p -> prev = q;
    q -> next = p;
    p -> next = helper;
    helper -> prev = p; 
   } else if (q -> next == p && q != head_) {
    Node* helper = p -> next;
    (q -> prev) -> next = p;
    p -> prev = q -> prev;
    q -> prev = p;
    p -> next = q;
    q -> next = helper;
    helper -> prev = q;
   } else {
    Node* helper = q -> next;
    Node* helper2 = p -> next;
    Node* lastPrev = q -> prev;
    (p -> prev) -> next = q;
    q -> prev = p -> prev;
    p -> prev = lastPrev;
    lastPrev -> next = p;
    q -> next = helper2;
    helper2 -> prev = q;
    p -> next = helper;
    helper -> prev = p;
  }
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
  Node* temp = head_;
  while (temp -> next != NULL) {
    temp = temp -> next;
    delete(temp -> prev);
  }
  delete(temp);

  head_ = NULL;
  length_ = 0;
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other) {
  head_ = NULL;
  length_ = 1;
  head_ = new Node(other.head_ -> data);

  Node* curr = head_;
  Node* para = other.head_ -> next;

  while (para -> next != NULL) {

    insertAfter(curr, para -> data);
    para = para -> next;
    curr = curr -> next;
  }

  insertAfter(curr, para -> data);

}

/* Modifies the current chain: 
 * 1) Find the node with the first (leftmost) block in the unscrambled
 *    image and move it to the head of the chain.
 *	This block is the one whose closest match (to the left) is the
 *	largest.  That is, the distance (using distanceTo) to this block 
 *	is big for all other blocks.
 *	For each block B, find the distanceTo B from every other block
 *	and take the minimum of these distances as B's "value".
 *	Choose the block B with the maximum value over all blocks and
 *	swap its node to the head of the chain.
 *
 * 2) Starting with the (just found) first block B, find the node with
 *    the block that is the closest match to follow B (to the right)
 *    among the remaining blocks, move (swap) it to follow B's node,
 *    then repeat to unscramble the chain/image.
 */
void Chain::unscramble() {
  Node* tempPos = head_;
  while (tempPos != NULL) {
    Node* curr2 = tempPos;
    double max = 0.0; 
    Node* chosenNode = curr2;
    while(curr2 != NULL) {
      Node* curr3 = tempPos;
      double min = -1.0;
      while (curr3 != NULL) {
        double distance = curr3 -> data.distanceTo(curr2 -> data);
        if (min < 0) {
          min = distance;
        } else if (distance < min) {
          min = distance;
        }     
        curr3 = curr3 -> next;
      }
      if (max < min) {
        max = min;
        chosenNode = curr2;
      }
      curr2 = curr2 -> next;
    }
    swap(tempPos, chosenNode);
    tempPos = chosenNode -> next;
  }


}
