#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

#include "Node.h"

template<class T>
class LinkedList {
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const LinkedList<T> &obj);

    ~LinkedList();

    int getSize() const;
    bool isEmpty() const;
    bool contains(Node<T> *node) const;

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;
    Node<T> *getNodeAtIndex(int index) const;

    void insertAtTheFront(const T &data);
    void insertAtTheEnd(const T &data);
    void insertAfterNode(const T &data, Node<T> *node);
    void insertAsEveryKthNode(const T &data, int k);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeAllNodes();
    void removeEveryKthNode(int k);

    void swap(Node<T> *node1, Node<T> *node2);
    void shuffle(int seed);

    void print(bool reverse=false) const;

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    // My Helper Functions
    Node<T>* whereExist(const T& val) const;
    bool isExist(const T& val) const;

private: // DO NOT CHANGE THIS PART.
    Node<T> *head;

    int size;
};

template<class T>
LinkedList<T>::LinkedList() {
    head = NULL; // Initially empty list. 
    size = 0; // Empty list of size 0.
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj) {
    Node<T>* rhs_head = obj.head;

    size = obj.size; // Update the size private.
    int n = size;

    if (n == 0){
        head = NULL;
        return ;
    }

    // Create circular doubly linked list.
    Node<T>* new_head = new Node<T>(obj.head->data); // Head of the new linked list with nullptr as prev and next.
    Node<T>* new_tracker = new_head;

    int i;
    for (i=0;i<(n-1);i++){
        Node<T>* new_node = new Node<T>(obj.head->data);
        new_tracker->next = new_node;
        new_node->prev = new_tracker;
        new_tracker = new_tracker->next;
    }

    new_tracker->next = new_head;
    new_head->prev = new_tracker;
    // Linked list of size n has been created.

    // Copy values of rhs into newly created linked list.
    Node<T>* copied_tracker = rhs_head;
    Node<T>* copying_tracker = new_head;

    for (i=0;i<n;i++){
        copying_tracker->data = copied_tracker->data;
        copied_tracker = copied_tracker->next;
        copying_tracker = copying_tracker->next;
    }
    // Copy has been completed.

    // Assign new head to head private.
    head = new_head;
}

template<class T>
LinkedList<T>::~LinkedList() {
    if (head == NULL) return;
    Node<T>* tracker = head;
    int i,n = size;
    for (i=0;i<n;i++){
        Node<T>* store_next = tracker->next;
        if (tracker != NULL) delete tracker;
        tracker = store_next;
    }
}

template<class T>
int LinkedList<T>::getSize() const {
    return size;
}

template<class T>
bool LinkedList<T>::isEmpty() const {
    if (head == NULL && size == 0) return true;
    else return false;
}

template<class T>
bool LinkedList<T>::contains(Node<T> *node) const {
    if (head == NULL) return false;

    Node<T>* tracker = head;
    bool flag = false;
    
    while (1){
        if ((tracker == node) && (tracker->next == node->next) && (tracker->prev == node->prev)){
            flag = true;
            break;
        }
        if (tracker->next == head) break;
        tracker = tracker->next;
    }
    
    return flag;
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const {
    return head;
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const {
    if (head == NULL && size == 0) return NULL;
    Node<T>* tracker = head;
    while (tracker->next != head) tracker = tracker->next;
    return tracker;
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const {
    if (head == NULL && size == 0) return NULL;

    bool flag = false;

    Node<T>* tracker = head;
    while (1){
        if (tracker->data == data){
            flag = true;
            break;
        }
        if (tracker->next == head) break;
        tracker = tracker->next;
    }
    
    if (flag == false) return NULL;
    else return tracker;
}

template<class T>
Node<T> *LinkedList<T>::getNodeAtIndex(int index) const {
    if ((index < 0) || (index >= size)) return NULL; // Boundary check !
    int i;
    Node<T>* tracker = head;
    for (i=0;i<index;i++) tracker = tracker->next;
    
    return tracker;

}

template<class T>
void LinkedList<T>::insertAtTheFront(const T &data) {
    if (head == NULL && size == 0){ // In case of empty list!
        Node<T>* new_node = new Node<T>(data);
        new_node->next = new_node;
        new_node->prev = new_node;
        head = new_node;
        size++;
    }
    
    else{
        Node<T>* new_head = new Node<T>(data);
        Node<T>* last_node = this->getLastNode();
        Node<T>* old_head = head;
        
        new_head->next = old_head;
        old_head->prev = new_head;

        last_node->next = new_head;
        new_head->prev = last_node;

        head = new_head;
        size++; 
    }
}

template<class T>
void LinkedList<T>::insertAtTheEnd(const T &data) {
    if (head == NULL && size == 0){ // In case of empty list.
        Node<T>* new_node = new Node<T>(data);
        new_node->next = new_node;
        new_node->prev = new_node;
        head = new_node;
        size++;
    }

    else {
        // In case of non-empty list.
        Node<T>* new_node = new Node<T>(data);
        Node<T>* old_tail_node = this->getLastNode();
        old_tail_node->next = new_node;
        new_node->prev = old_tail_node;
        new_node->next = head;
        head->prev = new_node;
        size++;
    }
}

template<class T>
void LinkedList<T>::insertAfterNode(const T &data, Node<T> *node) {
    if (head == NULL && size == 0) return ;
    Node<T>* searched_node = NULL;
    Node<T>* tracker = head;
    while (1){
        if (tracker == node){
            searched_node = node;
            break;
        }
        if (tracker->next == head) break;
        tracker = tracker->next;
    }

    if (searched_node == NULL) return ;
    else {
        Node<T>* new_node = new Node<T>(data);
        Node<T>* tail_node = this->getLastNode();
        if (searched_node == tail_node){
            Node<T>* old_head = head;
            searched_node->next = new_node;
            new_node->prev = searched_node;
            new_node->next = old_head;
            old_head->prev = new_node;
            size++;
        }
        else{
            // Inserting in between or at the end.
            Node<T>* next_of_searched_node = searched_node->next;
            searched_node->next = new_node;
            new_node->prev = searched_node;
            new_node->next = next_of_searched_node;
            next_of_searched_node->prev = new_node;
            size++;
        }
    }
}

template<class T>
void LinkedList<T>::insertAsEveryKthNode(const T &data, int k) {
    if (k < 2) return ;
    int step = k;
    while ((this->getNodeAtIndex(step-1) != NULL) || (step == size+1)){ // Step-1 is because of 0-index getNodeAtIndex. 
        Node<T>* new_node = new Node<T>(data);
        if (step == size+1){
            Node<T>* tail_node = this->getLastNode();
            tail_node->next = new_node;
            new_node->prev = tail_node;
            new_node->next = head;
            head->prev = new_node;
            size++;
            step += k;
        }
        else{
            Node<T>* node_to_be_shifted = this->getNodeAtIndex(step-1);
            Node<T>* prev_of_shifted = node_to_be_shifted->prev;
            prev_of_shifted->next = new_node;
            new_node->prev = prev_of_shifted;
            new_node->next = node_to_be_shifted;
            node_to_be_shifted->prev = new_node;
            size++;
            step += k;
        }
    }
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node) {
    if (head == NULL) return ;
    Node<T>* searched_node = NULL;
    Node<T>* tracker = head;
    while (1){
        if ((tracker == node) && (tracker->next == node->next) && (tracker->prev ==  node->prev)){
            searched_node = tracker;
            break;
        }
        if (tracker == this->getLastNode()) break;
        tracker = tracker->next;
    }
    if (searched_node == NULL) return ;
    else {
        if (size == 1){
            delete searched_node;
            head = NULL;
            size--;
        }
        else {
            if (searched_node == head){
                Node<T>* prv_of_head = head->prev;
                Node<T>* next_of_head = head->next;
                delete head;
                prv_of_head->next = next_of_head;
                next_of_head->prev = prv_of_head;
                head = next_of_head;
                size--;
            }
            else {
                Node<T>* prv_s = searched_node->prev;
                Node<T>* next_s = searched_node->next;
                delete searched_node;
                prv_s->next = next_s;
                next_s->prev = prv_s;
                size--;
            }
        }
    }
}

template<class T>
void LinkedList<T>::removeNode(const T &data) {
    if (size == 0 && head == NULL) return ;
    if (size == 1){
        if (head->data == data){
            delete head;
            head = NULL;
            size--;
        }
        else return ;
    }
    
    while (this->isExist(data)){
        Node<T>* searched_node = this->whereExist(data);
        removeNode(searched_node);
    }
}

template<class T>
void LinkedList<T>::removeAllNodes() {
    if (head == NULL && size == 0) return;
    if (size == 1){
        delete head;
        size--;
        head = NULL;
        return ;
    }
    Node<T>* tracker = head;
    while (1){
        if (size == 1){
            delete head;
            size--;
            head = NULL;
            break;
        }
        Node<T>* tobe_deleted = tracker;
        tracker = tracker->next;
        removeNode(tobe_deleted);
    }
}

template<class T>
void LinkedList<T>::removeEveryKthNode(int k) {
    if (k<2) return ;
    int counter = 1;
    int initial_size = size;
    Node<T>* tracker = head;
    while (1){
        if ((counter % k) == 0){
            Node<T>* to_delete = tracker;
            tracker = tracker->next;
            removeNode(to_delete);
        }
        else {
            tracker = tracker->next;
        }
        if (counter == initial_size) break;
        counter++;
    }
}

template<class T>
void LinkedList<T>::swap(Node<T> *node1, Node<T> *node2) {
    if ((this->contains(node1) == false) || (this->contains(node2) == false)) return ;
    if (node1 == node2) return;
    
    // Both exists in the linked list.
    
    if (size == 2){
        if (head == node1) head = node2;
        else if (head == node2) head = node1;
        return;
    }

    if (node1->next == node2){
        Node<T>* pre = node1->prev;
        Node<T>* nxt = node2->next;

        pre->next = node2;
        node2->prev = pre;
        node2->next = node1;
        node1->prev = node2;
        node1->next = nxt;
        nxt->prev = node1;

        if (head == node1) head = node2;
        else if (head == node2) head = node1;
        return;
    }

    else if (node2->next == node1){
        Node<T>* pre = node2->prev;
        Node<T>* nxt = node1->next;

        pre->next = node1;
        node1->prev = pre;
        node1->next = node2;
        node2->prev = node1;
        node2->next = nxt;
        nxt->prev = node2;

        if (head == node1) head = node2;
        else if (head == node2) head = node1;
        return;
    }

    else{
        Node<T>* pre1 = node1->prev;
        Node<T>* nxt1 = node1->next;
        Node<T>* pre2 = node2->prev;
        Node<T>* nxt2 = node2->next;

        pre1->next = node2;
        node2->prev = pre1;
        node2->next = nxt1;
        nxt1->prev = node2;

        pre2->next = node1;
        node1->prev = pre2;
        node1->next = nxt2;
        nxt2->prev = node1;

        if (head == node1) head = node2;
        else if (head == node2) head = node1;
        return;
    }
}

template<class T>
void LinkedList<T>::shuffle(int seed) {
    int i;
    for (i=0;i<size;i++){
        this->swap(this->getNodeAtIndex(i),this->getNodeAtIndex((i*i+seed)%size));
    }
}

template<class T>
void LinkedList<T>::print(bool reverse) const {
    if (this->isEmpty()) {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    if (reverse) {
        // traverse in reverse order (last node to first node).

        Node<T> *node = this->getLastNode();

        do {
            std::cout << *node << std::endl;
            node = node->prev;
        }
        while (node != this->getLastNode());
    } else {
        // traverse in normal order (first node to last node).

        Node<T> *node = this->getFirstNode();

        do {
            std::cout << *node << std::endl;
            node = node->next;
        }
        while (node != this->getFirstNode());
    }
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs) {
    this->removeAllNodes();
    if (rhs.head == NULL) return *(this);
    else {
        head = new Node<T>(rhs.head->data);
        head->next = head;
        head->prev = head;
        int i,n=rhs.size;
        Node<T>* tracker = head;
        for (i=1;i<n;i++){
            Node<T>* new_node = new Node<T>(rhs.head->data);
            tracker->next = new_node;
            new_node->prev = tracker;
            new_node->next = head;
            head->prev = new_node;
            tracker = tracker->next;
        }

        tracker = head;
        Node<T>* tracker_rhs = rhs.head;
        while (1){
            tracker->data = tracker_rhs->data;
            if (tracker_rhs->next == rhs.head) break;
            tracker = tracker->next;
            tracker_rhs = tracker_rhs->next;
        }
        size = rhs.size;
        return *(this);
    }
}

// My Helper Functions

template<class T>
Node<T> *LinkedList<T>::whereExist(const T &val) const{
    if (size == 0) return NULL;
    if (size == 1){
        if (head->data == val) return head;
        else return NULL;
    }
    Node<T>* tracker = head;
    Node<T>* searched_node = NULL;

    while (1){
        if (tracker->data == val){
            searched_node = tracker;
            break;
        }
        if (tracker->next == head) break;
        tracker = tracker->next;
    }
    return searched_node;
}

template<class T>
bool LinkedList<T>::isExist(const T &val) const{
    if (size == 0) return false;
    if (size == 1){
        if (head->data == val) return true;
        else return false;
    }
    bool flag = false;
    Node<T>* tracker = head;
    while (1){
        if (tracker->data == val){
            flag = true;
            break;
        }
        if (tracker->next == head) break;
        tracker = tracker->next;
    }
    return flag;
}

#endif //LINKEDLIST_H