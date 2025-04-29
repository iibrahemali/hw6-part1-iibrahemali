//SWE:Ibraheem

#include "RedBlackTree.h"
#include <stdexcept>
#include <sstream>

/*
- ZyBook 16
-  https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
-  https://www.tutorialspoint.com/insertion-in-the-red-black-tree-in-data-structure
*/

// Constructor
RedBlackTree::RedBlackTree() {
    root = nullptr;
    numItems = 0;
}

// Constructor with data
RedBlackTree::RedBlackTree(int newData) {
    root = new RBTNode{newData, COLOR_BLACK};
    numItems = 1;
}

// Copy constructor
RedBlackTree::RedBlackTree(const RedBlackTree& rbt) {
    root = CopyOf(rbt.root);
    numItems = rbt.numItems;
}

// Copy helper
RBTNode* RedBlackTree::CopyOf(const RBTNode* node) {
    if (node == nullptr) return nullptr;
    RBTNode* copy = new RBTNode{node->data, node->color};
    copy->IsNullNode = node->IsNullNode;
    copy->left = CopyOf(node->left);
    copy->right = CopyOf(node->right);
    if (copy->left) copy->left->parent = copy;
    if (copy->right) copy->right->parent = copy;
    return copy;
}

// Insert
void RedBlackTree::Insert(int newData) {
    if (Contains(newData)) throw invalid_argument("Duplicate value");
    RBTNode* newNode = new RBTNode{newData, COLOR_RED};
    BasicInsert(newNode);
    InsertFixUp(newNode);
    numItems++;
}

// Insert the node like a regular binary search tree node
void RedBlackTree::BasicInsert(RBTNode* node) {
    RBTNode* curr = root;
    RBTNode* parent = nullptr;

    while (curr != nullptr) {
        parent = curr;
        if (node->data < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }

    node->parent = parent;

    if (parent == nullptr) {
        root = node;
    } else if (node->data < parent->data) {
        parent->left = node;
    } else {
        parent->right = node;
    }
}

// Fix violations caused by a red-red parent/child situation (ZyBook 16.3.1)
void RedBlackTree::InsertFixUp(RBTNode* node) {
    while (node != root && node->parent->color == COLOR_RED) {
        if (IsLeftChild(node->parent)) {
            RBTNode* uncle = GetUncle(node);
            if (uncle && uncle->color == COLOR_RED) {
                node->parent->color = COLOR_BLACK;
                uncle->color = COLOR_BLACK;
                node->parent->parent->color = COLOR_RED;
                node = node->parent->parent;
            } else {
                if (IsRightChild(node)) {
                    node = node->parent;
                    LeftRotate(node);
                }
                node->parent->color = COLOR_BLACK;
                node->parent->parent->color = COLOR_RED;
                RightRotate(node->parent->parent);
            }
        } else {
            RBTNode* uncle = GetUncle(node);
            if (uncle && uncle->color == COLOR_RED) {
                node->parent->color = COLOR_BLACK;
                uncle->color = COLOR_BLACK;
                node->parent->parent->color = COLOR_RED;
                node = node->parent->parent;
            } else {
                if (IsLeftChild(node)) {
                    node = node->parent;
                    RightRotate(node);
                }
                node->parent->color = COLOR_BLACK;
                node->parent->parent->color = COLOR_RED;
                LeftRotate(node->parent->parent);
            }
        }
    }
    root->color = COLOR_BLACK;
}

// Search if data exists
bool RedBlackTree::Contains(int data) const {
    return Get(data) != nullptr;
}

// Get node with the given data
RBTNode* RedBlackTree::Get(int data) const {
    RBTNode* curr = root;
    while (curr != nullptr) {
        if (data == curr->data) return curr;
        if (data < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return nullptr;
}

// Get the minimum value
int RedBlackTree::GetMin() const {
    if (!root) throw runtime_error("Empty tree");
    RBTNode* curr = root;
    while (curr->left != nullptr) curr = curr->left;
    return curr->data;
}

// Get the maximum value
int RedBlackTree::GetMax() const {
    if (!root) throw runtime_error("Empty tree");
    RBTNode* curr = root;
    while (curr->right != nullptr) curr = curr->right;
    return curr->data;
}

// String traversal helpers
string RedBlackTree::ToInfixString(const RBTNode* n) {
    if (!n) return "";
    return ToInfixString(n->left) + GetNodeString(n) + ToInfixString(n->right);
}

string RedBlackTree::ToPrefixString(const RBTNode* n) {
    if (!n) return "";
    return GetNodeString(n) + ToPrefixString(n->left) + ToPrefixString(n->right);
}

string RedBlackTree::ToPostfixString(const RBTNode* n) {
    if (!n) return "";
    return ToPostfixString(n->left) + ToPostfixString(n->right) + GetNodeString(n);
}

// Color string representation
string RedBlackTree::GetColorString(const RBTNode* n) {
    return n->color == COLOR_RED ? "R" : "B";
}

// Node string for ToString methods
string RedBlackTree::GetNodeString(const RBTNode* n) {
    if (!n) return "";
    return " " + GetColorString(n) + to_string(n->data) + " ";
}

// Perform a left rotation to rebalance tree structure
void RedBlackTree::LeftRotate(RBTNode* x) {
    RBTNode* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) root = y;
    else if (IsLeftChild(x)) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Perform a right rotation to rebalance tree structure
void RedBlackTree::RightRotate(RBTNode* x) {
    RBTNode* y = x->left;
    x->left = y->right;
    if (y->right != nullptr) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) root = y;
    else if (IsRightChild(x)) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// Check if node is a left child
bool RedBlackTree::IsLeftChild(RBTNode* node) const {
    return node->parent && node->parent->left == node;
}

// Check if node is a right child
bool RedBlackTree::IsRightChild(RBTNode* node) const {
    return node->parent && node->parent->right == node;
}

// Get the uncle of a node
RBTNode* RedBlackTree::GetUncle(RBTNode* node) const {
    if (!node->parent || !node->parent->parent) return nullptr;
    if (IsLeftChild(node->parent))
        return node->parent->parent->right;
    else
        return node->parent->parent->left;
}
