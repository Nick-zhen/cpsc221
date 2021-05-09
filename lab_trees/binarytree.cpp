/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file, as well as binarytree.h.
 */

#include <iostream>

using namespace std;

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function, with an additional
 * Node* parameter to allow for recursive calls. NOTE: use this as an example
 * for writing your own private helper functions.
 * @param subRoot
 * @return The height of the subtree.
 */
template <typename T>
int BinaryTree<T>::height(const Node* subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + max(height(subRoot->left), height(subRoot->right));
}


template <typename T>
void BinaryTree<T>::printLeftToRight() const {
    // Call recursive helper function on root
    printLeftToRight(root);
    cout << endl;
}
/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node* subRoot) const
{
    // base case
    if (subRoot == NULL) {
        return;
    }
    // cout << to_string(subRoot->elem) << endl;
    printLeftToRight(subRoot->left);
    // Do not remove this line - used for correct print output
    cout << to_string(subRoot->elem) + " ";
    // cout << endl;
    
    printLeftToRight(subRoot->right);
}


template <typename T>
void BinaryTree<T>::mirror() {
    mirror(root);
}
/**
 * Flips the tree over a vertical axis, modifying the tree itself
 * (i.e. not creating a flipped copy).
 */
template <typename T>
void BinaryTree<T>::mirror(Node* subRoot) {
    //base case
    if (subRoot->left == NULL && subRoot->right == NULL) {
        return;
    } else {
        Node* temp = subRoot->left;
        subRoot->left = subRoot->right;
        subRoot->right = temp;
    }
    
    if (subRoot->right == NULL) {
        mirror(subRoot->left);
    } else if (subRoot->left == NULL) {
        mirror(subRoot->right);
    } else {
        mirror(subRoot->left);
        mirror(subRoot->right);
    }
}

template <typename T>
void BinaryTree<T>::printPaths() const {
    printPaths(root, "Path: ");
}

/**
 * Prints out all the possible paths from the root of the tree to any leaf node.
 * That is, all sequences starting at the root node and continuing downwards,
 * ending at a leaf node. Paths ending in a left node should be printed before
 * paths ending in a node further to the right.
 */
template <typename T>
void BinaryTree<T>::printPaths(const Node* subRoot, string s) const {
    //base case

    s += to_string(subRoot->elem) + " ";

    if (subRoot->left == NULL && subRoot->right == NULL) {
        cout << s << endl;
        return;
    }

    if (subRoot->left == NULL) {
        printPaths(subRoot->right,s);
    } else if (subRoot->right == NULL) {
        printPaths(subRoot->left,s);
    } else {
        printPaths(subRoot->left,s);
        printPaths(subRoot->right,s);
    }
}


template <typename T>
int BinaryTree<T>::sumDistances() const {
    return sumDistances(root, 0);
}
/**
 * Each node in a tree has a distance from the root node - the depth of that
 * node, or the number of edges along the path from that node to the root. This
 * function returns the sum of the distances of all nodes to the root node (the
 * sum of the depths of all the nodes). Your solution should take O(n) time,
 * where n is the number of nodes in the tree.
 * @return The sum of the distances of all nodes to the root.
 */
template <typename T>
int BinaryTree<T>::sumDistances(const Node* subRoot, int distance) const {
    
    // if (subRoot == root) {
    //     distance = 0;
    // }
    
    //base case

    if (subRoot->left == NULL && subRoot->right == NULL) {
        return distance;
    }

    if (subRoot->left == NULL) {
        distance += sumDistances(subRoot->right, distance+1);
    } else if (subRoot->right == NULL) {
        distance += sumDistances(subRoot->left, distance+1);
    } else {
        distance += sumDistances(subRoot->left,distance+1) + sumDistances(subRoot->right,distance+1);
    }

    return distance;



    // if (subRoot->left == NULL && subRoot->right == NULL) {
    //     return distance;
    // }

    // if (subRoot->left == NULL) {
    //     return distance + sumDistances(subRoot->right, distance+1);
    // } else if (subRoot->right == NULL) {
    //     return distance + sumDistances(subRoot->left, distance+1);
    // } else {
    //     return distance + sumDistances(subRoot->left,distance+1) + sumDistances(subRoot->right,distance+1);
    // }

}


template <typename T>
bool BinaryTree<T>::isOrdered() const {
    return isOrdered(root);
}
/**
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrdered(const Node* subRoot) const {
    // base case
    if (subRoot->left == NULL && subRoot->right == NULL) {
        return true;
    }
    if (subRoot->left == NULL) {
        if (subRoot->elem <= subRoot->right->elem) {
            return true && isOrdered(subRoot->right);
        }
        return false;
    } else if (subRoot->right == NULL) {
        if (subRoot->elem >= subRoot->left->elem) {
            return true && isOrdered(subRoot->left);
        }
        return false;
    } else {
        if (subRoot->elem >= subRoot->left->elem && subRoot->elem <= subRoot->right->elem) {
            return true && isOrdered(subRoot->left) && isOrdered(subRoot->right);
        }
        return false;
    }
}
