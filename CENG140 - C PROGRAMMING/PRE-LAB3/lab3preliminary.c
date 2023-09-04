#include "stdio.h"
#include "stdlib.h"
#include "lab3.h"

/* Do not change the lines above */
/* Write your solutions below */

Student** create_student_array(int* student_id_list, int student_count){
    int i;
    Student** array = malloc(sizeof(Student*)*student_count);
    for (i=0;i<student_count;i++){
        Student* new_student = malloc(sizeof(Student));
        new_student->studentID = student_id_list[i];
        array[i] = new_student;
    }
    return array;
}

Node* insert_head(Node* head, int new_node_data){
   Node* create(int data);
    Node* new_node = create(new_node_data);
    if (head == NULL) return new_node;
    new_node->next = head;
    return new_node;
}

Node* remove_kth_node(Node* head, int index){
    Node* nth_node(Node* head,int n);
    if (head->next == NULL) return NULL;
    if (index == 0) return head->next;
    nth_node(head,index-1)->next = nth_node(head,index+1);
    return head;
}

int find_occurrences(Node* head, int data){
    Node* tracker = head;
    int occurence = 0;
    while (tracker){
        if (tracker->data == data) occurence++;
        tracker = tracker->next;
    }
    return occurence;
}

Node* copy_to_head(Node* head, int index){
    Node* create(int data);
    Node* nth_node(Node* head,int n);
    int length(Node* head);
    int l = length(head);
    int i;
    Node* start = create(-1);
    Node* tracker = start;
    for (i=index;i<l;i++){
        Node* new_node = create(nth_node(head,i)->data);
        tracker->next = new_node;
        tracker = tracker->next;
        if (i==l-1) tracker->next = head;
    }
    return start->next; 
}

Node* sum_consecutive_pairs(Node* head){
    Node* create(int data);
    Node* start = create(-1);
    Node* tracker = head;
    Node* s_tracker = start;
    while (tracker->next){
        int sum = tracker->data + tracker->next->data;
        Node* new_snode = create(sum);
        s_tracker->next = new_snode;
        s_tracker = s_tracker->next;
        tracker = tracker->next;
    }
    return start->next;
}

Node* create(int data){
    Node* new_node = malloc(sizeof(Node));
    new_node->next = NULL;
    new_node->data = data;
    return new_node;
}

Node* nth_node(Node* head,int n){
    Node* tracker = head;
    int i;
    for (i=0;i<n;i++) tracker = tracker->next;
    return tracker;
}

int length(Node* head){
    int n = 0;
    Node* tracker = head;
    while (tracker){
        n++;
        tracker = tracker->next;
    }
    return n;
}









