#include "stdio.h"
#include "stdlib.h"
#include "lab3.h"

/* Do not change the lines above */
/* Write your solutions below */

Student** create_student_array_with_teachers(int* student_id_list, int* teacher_id_list, int student_and_teacher_count)
{
    Student** result = malloc(sizeof(Student*)*student_and_teacher_count);
    int i;
    for (i=0;i<student_and_teacher_count;i++){
        Student *new_student = malloc(sizeof(Student));
        Teacher *new_teacher = malloc(sizeof(Teacher));
        new_student->studentID = student_id_list[i];
        new_teacher->teacherID = teacher_id_list[i];
        new_student->best_teacher = new_teacher;
        result[i] = new_student;
    }
    return result ;
}

Node* insert_tail(Node* head, int new_node_data)
{
    Node* tail(Node* head);
    Node *old_tail;
    Node *new_tail = malloc(sizeof(Node));
    if (head == NULL){
        new_tail->data = new_node_data;
        new_tail->next = NULL;
        return new_tail;
    }
    old_tail = tail(head);
    new_tail->data = new_node_data;
    old_tail->next = new_tail;
    new_tail->next = NULL;
    
    return head;
    
}

Node* remove_kth_node_reversely(Node* head, int index)
{
    int length(Node* head);
    Node* nth_node(Node* head,int n);
    Node *pre,*post;
    int l = length(head);
    int real_index = length(head)-index-1;
    if (length(head) == 1) return NULL;
    if (index == length(head)-1) return head->next;
    if (index == 0){
        nth_node(head,l-2)->next = NULL;
        return head;
    }
    pre = nth_node(head,real_index-1);
    post = nth_node(head,real_index+1);
    pre->next = post;
    return head;
}

int find_occurrences_of_second_list(Node* head, Node* second_head)
{
    Node* nth_node(Node* head,int n);
    int length(Node* head);
    int l1,l2,n,i,flag;
    if (head == NULL || second_head == NULL) return 0;
    l1 = length(head);
    l2 = length(second_head);
    n = 0;
    for (i=0;i<=(l1-l2);i++){
        int x,y;
        flag = 1;
        for (x=i,y=0;y<l2;x++,y++){
            if (nth_node(head,x)->data != nth_node(second_head,y)->data){
                flag = 0;
                break;
            }
        }
        if (flag) n++;
    }
    return n;
}

Node* cut_and_paste(Node* head, int index_to_cut, int index_to_paste)
{
    Node* nth_node(Node* head,int n);
    int length(Node* head);
    Node* tail(Node* head);
    Node *pre_paste,*paste,*cut,*pre_cut,*post_cut;
    if (length(head) == 1) return head;
    if (index_to_paste == 0){
        Node *cutnext = nth_node(head,index_to_cut+1);
        Node *cutpre  = nth_node(head,index_to_cut-1);
        Node *cut = nth_node(head,index_to_cut);
        cut->next = head;
        cutpre->next = cutnext;
        return cut;
    }
    pre_paste = nth_node(head,index_to_paste-1);
    paste = nth_node(head,index_to_paste);
    cut = nth_node(head,index_to_cut);
    pre_cut = nth_node(head,index_to_cut-1);
    post_cut = nth_node(head,index_to_cut+1);
    pre_cut->next = post_cut;
    pre_paste->next = cut;
    cut->next = paste;
    return head;
}

Node* sum_symmetric_pairs(Node* head)
{
    Node* nth_node(Node* head,int n);
    int length(Node* head);
    int i;
    int l = length(head);
    int *array = malloc(sizeof(int)*(l/2));
    Node *start,*tracker;
    start = malloc(sizeof(Node));
    start->data = -1;
    start->next = NULL;
    for (i=0;i<(l/2);i++){
        Node *x,*y;
        x = nth_node(head,i);
        y = nth_node(head,l-1-i);
        array[i] = x->data+y->data;
    }
    tracker = start;
    for (i=0;i<(l/2);i++){
        Node *new_node = malloc(sizeof(Node));
        new_node->next = NULL;
        new_node->data = array[i];
        tracker->next = new_node;
        tracker = tracker->next;
    }
    return start->next;
}

Node* tail(Node* head){
    Node *tracker = head;
    while (1){
        if (tracker->next == NULL) break;
        tracker = tracker->next;
    }
    return tracker;
}

Node* nth_node(Node* head,int n){
    Node* tracker = head;
    int i;
    for (i=0;i<n;i++) tracker = tracker->next;
    return tracker;
}

int length(Node* head){
    Node* tracker = head;
    int n = 0;
    while (tracker){
        n++;
        tracker = tracker->next;
    }
    return n;
}