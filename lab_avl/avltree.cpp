/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file. IMPORTANT: read avltree.h to understand
 * what to do in this lab.
 */

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    /** 
     * TODO: your code here. Make sure the following cases are included:
     * Case 1: subtree is NULL
     * Case 2: (key, value) pair should be inserted into left subtree
     * Case 3: (key, value) pair should be inserted into right subtree
     */
    if (subtree == NULL)
        subtree = new Node(key, value);
    else {
        if (key < subtree->key) {
            insert(subtree->left, key, value);
        } else {
            insert(subtree->right, key, value);
        }
    }
  

    // Rebalance tree after insertion (don't remove this)
    rebalance(subtree);
}

/**
 * Update the height of the given node by checking the height of the node's
 * left and right subtrees. This is a NON-recursive function.
 * @param node The node's height to update
 */
template <class K, class V>
void AVLTree<K, V>::updateHeight(Node* node) {
    if (height(node->left) > height(node->right)) {
        node->height = height(node->left) + 1;
    } else {
        node->height = height(node->right) + 1;
    }
    // node->height = max(height(node->left) > height(node->right));
    // node->height += 1;
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t) {
    *_out << __func__ << endl; // Outputs the rotation name (don't remove this)

    Node* newSubRoot = t->right;
    t->right = newSubRoot->left;
    newSubRoot->left = t;
    t = newSubRoot;

    // TODO: update the heights for t->left and t (in that order)
    updateHeight(t->left);
    updateHeight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    *_out << __func__ << endl; // Outputs the rotation name (don't remove this)

    Node* temp = t->left;
    t->left = temp->right;
    temp->right = t;
    t = temp;

    updateHeight(t->right);
    updateHeight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    *_out << __func__ << endl; // Outputs the rotation name (don't remove this)

    // TODO: your code here
    // HINT: you should make use of the other functions defined in this file,
    // instead of manually changing the pointers again
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    *_out << __func__ << endl; // Outputs the rotation name (don't remove this)

   // TODO: your code here
   rotateRight(t->right);
   rotateLeft(t);
}

/**
 * Rebalance a node by performing rotations. You can assume that node->left
 * and node->right are both balanced. Even if no rotations are required,
 * you should update the node's height.
 * @param node The node to balance.
 */
template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree) {
    /** 
      * TODO: your code here. Make sure the following cases are included:
      * Cases 1-4: the four cases of tree imbalance as discussed in lecture
      * Case 5: the tree is already balanced. You MUST still correctly update
      * subtree's height 
      */
     int defBalance = height(subtree->right)- height(subtree->left);
     
     //caseLeft: 
     if (defBalance == -2) {
        int childDefBalance = height(subtree->left->right)- height(subtree->left->left);
        //caseLR: LeftRight
        if (childDefBalance == 1) {
            rotateLeftRight(subtree);
        } else {
            //caseLL: rotateRight
            rotateRight(subtree);
        }

     } else if (defBalance == 2) {
         //caseRight:
         int childDefBalance = height(subtree->right->right)- height(subtree->right->left);
         //caseRightLef
         if (childDefBalance == -1) {
            rotateRightLeft(subtree);
         } else {
            //caseRR: rotateLef
            rotateLeft(subtree);
         }
     }
    // updateHeight(subtree->left);
    // updateHeight(subtree->right);
    updateHeight(subtree);
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key) {
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        remove(subtree->left, key);
        rebalance(subtree);
    } else if (key > subtree->key) {
        remove(subtree->right, key);
        rebalance(subtree);
    } else {
        /* Reached the node that we need to delete */
        if (subtree->left == NULL && subtree->right == NULL) {
            /* Case 1: Node to remove has no children */
            delete subtree;
            subtree = NULL;
            return;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /**
             * Case 2: Node to remove has two children
             * TODO: your code here. For testing purposes, you
             * should use the PREDECESSOR.
             */
            Node* predecessor = subtree->left;
            while (predecessor->right != NULL) {
                predecessor = predecessor->right;
            }
            // swap(subtree, predecessor);
            subtree->value = predecessor->value;
            K predecessorK = predecessor->key;
            remove(subtree, predecessorK);
            subtree->key = predecessorK;
        } else {
            /* Case 3: Node to remove has one child */
            Node* curr = subtree;
            subtree = max(subtree->left, subtree->right);
            delete curr;
        }
        rebalance(subtree);
    }
}


