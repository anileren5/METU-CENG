#ifndef BST_H
#define BST_H

#include <iostream>

#include "BSTNode.h"

enum TraversalPlan {preorder, inorder, postorder};

template<class T>
class BST {
public: // DO NOT CHANGE THIS PART.
    BST();
    BST(const BST<T> &obj);

    ~BST();

    BSTNode<T> *getRoot() const;
    bool isEmpty() const;
    bool contains(BSTNode<T> *node) const;

    void insert(const T &data);

    void remove(const T &data);
    void removeAllNodes();

    BSTNode<T> *search(const T &data) const;
    BSTNode<T> *getSuccessor(BSTNode<T> *node, TraversalPlan tp) const;

    void print(TraversalPlan tp=inorder) const;

    BST<T> &operator=(const BST<T> &rhs);

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void print(BSTNode<T> *node, TraversalPlan tp) const;
    bool contains_helper(BSTNode<T>* tree,BSTNode<T>* node) const;
    BSTNode<T> *search_helper(BSTNode<T>* tree,const T& searched_data) const;
    void destructor_helper(BSTNode<T>* root);
    void insert_helper(BSTNode<T>*& root,const T &data);
    void remove_helper(BSTNode<T>*& root,const T &removed_data);
    T getMin(BSTNode<T>*& root);
    BSTNode<T>* findMinNode(BSTNode<T>*& root) const;
    void removeAllNodes_helper(BSTNode<T>*& root);
    void copyConstructor_helper(BSTNode<T>*& root,const BSTNode<T>* rhs);
    void assignment_helper(BSTNode<T>*& root,const BSTNode<T>* rhs);
    BSTNode<T>* getSuccessorPre_Helper(BSTNode<T>* root,BSTNode<T>* node) const;
    BSTNode<T>* getSuccessorIn_Helper(BSTNode<T>* root,BSTNode<T>* node) const;
    BSTNode<T>* getSuccessorPost_Helper(BSTNode<T>* root,BSTNode<T>* node) const;
    BSTNode<T>* findParent(BSTNode<T>* root,BSTNode<T>* node) const;
    bool isLeftChild(BSTNode<T>* root,BSTNode<T>* node) const;
    bool isRightChild(BSTNode<T>* root,BSTNode<T>* node) const;
    BSTNode<T>* findSibling(BSTNode<T>* root,BSTNode<T>* node) const;

private: // DO NOT CHANGE THIS PART.
    BSTNode<T> *root;
};

#endif //BST_H

template<class T>
BST<T>::BST() {
    /* TODO */
    root = NULL;
}

template<class T>
BST<T>::BST(const BST<T> &obj) {
    /* TODO */
    if (obj.root == NULL){
        root = NULL;
        return;
    }
    else {
        root = new BSTNode<T>(obj.root->data,NULL,NULL);
        copyConstructor_helper(root,obj.root);
    }
}

template<class T>
void BST<T>::copyConstructor_helper(BSTNode<T>*& root,const BSTNode<T>* rhs){
    if (rhs->left != NULL){
        root->left = new BSTNode<T>(rhs->left->data,NULL,NULL);
        copyConstructor_helper(root->left,rhs->left);
    }
    if (rhs->right != NULL){
        root->right = new BSTNode<T>(rhs->right->data,NULL,NULL);
        copyConstructor_helper(root->right,rhs->right);
    }
}

template<class T>
BST<T>::~BST() {
    /* TODO */
    destructor_helper(root);
}

template<class T>
void BST<T>::destructor_helper(BSTNode<T>* root){
    if (root == NULL) return ;
    else {
        destructor_helper(root->left);
        destructor_helper(root->right);
        delete root;
    }
}

template<class T>
BSTNode<T> *BST<T>::getRoot() const {
    /* TODO */
    return root;
}

template<class T>
bool BST<T>::isEmpty() const {
    /* TODO */
    if (root == NULL) return true;
    else return false;
}

template<class T>
bool BST<T>::contains(BSTNode<T> *node) const {
    /* TODO */
    return contains_helper(root,node);
    
}

template<class T>
bool BST<T>::contains_helper(BSTNode<T>* tree,BSTNode<T>* node) const{
    if (tree == NULL) return false;
    else {
        if (tree == node && tree->data == node->data) return true;
        else if (tree != node && node->data > tree->data) return contains_helper(tree->right,node);
        else if (tree != node && node->data < tree->data) return contains_helper(tree->left,node);
    }
}

template<class T>
void BST<T>::insert(const T &data) {
    /* TODO */
    insert_helper(root,data);
}

template<class T>
void BST<T>::insert_helper(BSTNode<T>*& root,const T &data){
    if (root == NULL){
        BSTNode<T>* new_node = new BSTNode<T>();
        new_node->data = data;
        new_node->left = NULL;
        new_node->right = NULL;
        root = new_node;
        return ;
    }
    else {
        if (data > root->data) insert_helper(root->right,data);
        else if (data < root->data) insert_helper(root->left,data); 
    }
}

template<class T>
void BST<T>::remove(const T &data) {
    /* TODO */
    remove_helper(root,data);
}

template<class T>
void BST<T>::remove_helper(BSTNode<T>*& root,const T &removed_data){
    if (root == NULL) return ;
    else if (removed_data < root->data) remove_helper(root->left,removed_data);
    else if (removed_data > root->data) remove_helper(root->right,removed_data);
    else {
        if (root->left != NULL && root->right != NULL){
            root->data = getMin(root->right);
            remove_helper(root->right,root->data);
        }
        else {
            BSTNode<T>* old_node = root;
            root = (root->left != NULL) ? root->left : root->right;
            delete old_node;
        }
    }
}

template<class T>
void BST<T>::removeAllNodes() {
    /* TODO */
    removeAllNodes_helper(root);
    root = NULL; /* ????? */
}

template<class T>
void BST<T>::removeAllNodes_helper(BSTNode<T>*& root){
    if (root == NULL) return;
    
    removeAllNodes_helper(root->left);
    removeAllNodes_helper(root->right);
    delete root;
}


template<class T>
BSTNode<T> *BST<T>::search(const T &data) const {
    /* TODO */
    return search_helper(root,data);
}

template<class T>
BSTNode<T> *BST<T>::search_helper(BSTNode<T>* tree,const T& searched_data) const {
    if (tree == NULL) return NULL;
    else {
        if (tree->data == searched_data) return tree;
        else {
            if (searched_data > tree->data) return search_helper(tree->right,searched_data);
            else if (searched_data < tree->data) return search_helper(tree->left,searched_data);
        }
    }
}

template<class T>
BSTNode<T> *BST<T>::getSuccessor(BSTNode<T> *node, TraversalPlan tp) const {

    if (tp == inorder) {
        /* TODO */
        return getSuccessorIn_Helper(root,node);
    } else if (tp == preorder) {
        /* TODO */
        return getSuccessorPre_Helper(root,node);
    } else if (tp == postorder) {
        /* TODO */
        return getSuccessorPost_Helper(root,node);
    }
}



template<class T>
BSTNode<T>* BST<T>::getSuccessorPre_Helper(BSTNode<T>* root,BSTNode<T>* node) const{
    if (node->left != NULL) return node->left;
    else if (node->left == NULL && node->right != NULL) return node->right;
    else {
        BSTNode<T>* tracker1 = node;
        BSTNode<T>* tracker2 = findParent(root,node);
        
        while (tracker2->right == NULL && findParent(root,tracker2) != NULL){
            tracker1 = tracker2;
            tracker2 = findParent(root,tracker2);
        }
        
        while (tracker2 != NULL && tracker1 == tracker2->right){
            tracker1 = tracker2;
            tracker2 = findParent(root,tracker2);
        }
        
        return tracker2->right;
    }
}

template<class T>
BSTNode<T>* BST<T>::getSuccessorIn_Helper(BSTNode<T>* root,BSTNode<T>* node) const{
    if (node->right != NULL) return findMinNode(node->right);
    else {
        BSTNode<T>* tracker = root;
        BSTNode<T>* store = NULL;
        while (tracker->data != node->data){
            if (node->data < tracker->data){
                store = tracker;
                tracker = tracker->left;
            }
            else tracker = tracker->right;
        }
        return store;
    }
}


template<class T>
BSTNode<T>* BST<T>::getSuccessorPost_Helper(BSTNode<T>* root,BSTNode<T>* node) const{
    if (node == root) return NULL;
    else if (isRightChild(root,node)) return findParent(root,node);
    else if (isLeftChild(root,node)){
        if (findParent(root,node)->right == NULL) return findParent(root,node);
        else {
            BSTNode<T>* tracker = findParent(root,node)->right;
            while (!(tracker->left == NULL && tracker->right == NULL)){
                if (tracker->left != NULL) tracker = tracker->left;
                else if (tracker->left == NULL && tracker->right != NULL) tracker = tracker->right;
            }
            return tracker;
        }
    }
}


template<class T>
void BST<T>::print(TraversalPlan tp) const {

    if (tp == inorder) {
        // check if the tree is empty?
        if (isEmpty()) {
            // the tree is empty.
            std::cout << "BST_inorder{}" << std::endl;
            return;
        }

        // the tree is not empty.

        // recursively output the tree.
        std::cout << "BST_inorder{" << std::endl;
        print(root, inorder);
        std::cout << std::endl << "}" << std::endl;
    } else if (tp == preorder) {
        /* TODO */
        if (isEmpty()){
            // the tree is empty.
            std::cout << "BST_preorder{}" << std::endl;
            return ;
        }
        
        // the tree is not empty.
        
        // recursively output the tree.
        std::cout << "BST_preorder{" << std::endl;
        print(root,preorder);
        std::cout << std::endl << "}" << std::endl;
    } else if (tp == postorder) {
        /* TODO */
        if (isEmpty()){
            // the tree is empty.
            std::cout << "BST_postorder{}" << std::endl;
            return ;
        }
        
        // the tree is not empty.
        
        // recursively output the tree.
        std::cout << "BST_postorder{" << std::endl;
        print(root,postorder);
        std::cout << std::endl << "}" << std::endl;
    }
}

template<class T>
BST<T> &BST<T>::operator=(const BST<T> &rhs) {
    /* TODO */
    removeAllNodes();
    if (rhs.root == NULL){
        root = NULL;
        return *(this);
    }
    else {
        root = new BSTNode<T>(rhs.root->data,NULL,NULL);
        assignment_helper(root,rhs.root);
        return *(this);
    }
}

template<class T>
void BST<T>::assignment_helper(BSTNode<T>*& root,const BSTNode<T>* rhs){
    if (rhs->left != NULL){
        root->left = new BSTNode<T>(rhs->left->data,NULL,NULL);
        copyConstructor_helper(root->left,rhs->left);
    }
    if (rhs->right != NULL){
        root->right = new BSTNode<T>(rhs->right->data,NULL,NULL);
        copyConstructor_helper(root->right,rhs->right);
    }
}

template<class T>
void BST<T>::print(BSTNode<T> *node, TraversalPlan tp) const {

    // check if the node is NULL?
    if (node == NULL)
        return;

    if (tp == inorder) {
        // first, output left subtree and comma (if needed).
        print(node->left, inorder);
        if (node->left) {
            std::cout << "," << std::endl;
        }

        // then, output the node.
        std::cout << "\t" << node->data;

        // finally, output comma (if needed) and the right subtree.
        if (node->right) {
            std::cout << "," << std::endl;
        }
        print(node->right, inorder);
    } else if (tp == preorder) {
        /* TODO */
        
        
        std::cout << "\t" << node->data;
        
        if (node->left) {
            std::cout << "," << std::endl;
        }
        print(node->left,preorder);
        
        if (node->right) {
            std::cout << "," << std::endl;
        }
        print(node->right,preorder);
        
    } else if (tp == postorder) {
        /* TODO */
        print(node->left,postorder);
        if (node->left) {
            std::cout << "," << std::endl;
        }
        
        print(node->right, postorder);
        if (node->right) {
            std::cout << "," << std::endl;
        }
        
        std::cout << "\t" << node->data;
        
    }
}

template<class T>
T BST<T>::getMin(BSTNode<T>*& root){
    if (root->left == NULL) return root->data;
    else return getMin(root->left);
}

template<class T>
BSTNode<T>* BST<T>::findMinNode(BSTNode<T>*& root) const{
    if (root == NULL) return NULL;
    if (root->left == NULL) return root;
    else return findMinNode(root->left);
}

template<class T>
BSTNode<T>* BST<T>::findParent(BSTNode<T>* root,BSTNode<T>* node) const{
    if (node == root) return NULL;
    if (root->left == node || root->right == node) return root;
    else {
        if (node->data > root->data) return findParent(root->right,node);
        else if (node->data < root->data) return findParent(root->left,node);
    }
}

template<class T>
bool BST<T>::isLeftChild(BSTNode<T>* root,BSTNode<T>* node) const{
    BSTNode<T>* parent = findParent(root,node);
    if (parent == NULL) return false;
    else return ((parent->left == node) ? true : false);
}

template<class T>
bool BST<T>::isRightChild(BSTNode<T>* root,BSTNode<T>* node) const{
    BSTNode<T>* parent = findParent(root,node);
    if (parent == NULL) return false;
    else return ((parent->right == node) ? true : false);
}

template<class T>
BSTNode<T>* BST<T>::findSibling(BSTNode<T>* root,BSTNode<T>* node) const{
    if (node == root) return NULL;
    else {
        BSTNode<T>* parent = findParent(root,node);
        if (parent->left == node) return parent->right;
        if (parent->right == node) return parent->left;
    }
}
















