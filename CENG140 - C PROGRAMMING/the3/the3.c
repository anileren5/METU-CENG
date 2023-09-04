#include <stdio.h>
#include <stdlib.h>
#include "the3.h"

Apartment* add_apartment(Apartment* head, int index, char* apartment_name, int max_bandwidth){
    /*This function returns the head of changed apartment linked list.*/
    Apartment* nth_apartment(Apartment* head,int n);
    int street_length(Apartment* head);
    Apartment* create_apartment(char* name,int max_bandwith);
    Apartment* street_tail(Apartment* head);
    Apartment *new_apartment = create_apartment(apartment_name,max_bandwidth); /* flat_list = NULL , next = NULL */ 
    if (head == NULL){ /*In case initially there is no apartment in the street.*/
        new_apartment->next = new_apartment;
        return new_apartment;
    }
    
    if (index == 0){ /*In case new apartment should be added at the beginning of the street.*/
        Apartment* tail = street_tail(head);
        Apartment* old_head = head;
        tail->next =  new_apartment;
        new_apartment->next = old_head; /*new_apartment has been new head of the street from now on.*/
        return new_apartment;
    }

    else if (index == street_length(head)){ /*In case new apartment should be added at the end of the street.*/
        Apartment* old_tail = street_tail(head);
        old_tail->next = new_apartment;
        new_apartment->next = head;
        return head;
    }

    else{ /*In case new apartment should be added in between.*/
        Apartment *pre,*post;
        pre = nth_apartment(head,index-1);
        post = nth_apartment(head,index);
        pre->next = new_apartment;
        new_apartment->next = post;
        return head;
    }
}

Apartment* nth_apartment(Apartment* head,int n){
    /* This function returns pointer to nth apartment in the street.(0-indexing)*/
    int i;
    Apartment* tracker = head;
    for (i=0;i<n;i++) tracker = tracker->next;
    return tracker;
}

int street_length(Apartment* head){
    /* This function returns how many apartments there are in the street.*/
    Apartment* tracker = head;
    int n = 1;
    if (tracker == NULL) return 0;
    while (tracker->next != head){
        n++;
        tracker = tracker->next;
    }
    return n;
}

Apartment* create_apartment(char* name,int max_bandwith){
    /* This function creates an apartment with name and max_bandwith, flat_list and next as NULL.*/
    Apartment* new_apartment = malloc(sizeof(Apartment));
    if (new_apartment){
        new_apartment->name = name;
        new_apartment->max_bandwidth = max_bandwith;
        new_apartment->flat_list = NULL;
        new_apartment->next = NULL;
    }
    return new_apartment;
}

Apartment* street_tail(Apartment* head){
    Apartment* tracker = head;
    if (head == NULL) return NULL;
    while (1){
        if (tracker->next == head) break;
        tracker = tracker->next;
    }
    return tracker;
}

void add_flat(Apartment* head, char* apartment_name, int index, int flat_id, int initial_bandwidth){
    Apartment* find_apartment(Apartment* head,char* apartment_name);
    Flat* nth_flat(Apartment* aprt,int index);
    Flat* create_flat(int flat_id,int bandwith);
    int current_bandwith(Apartment* aprt);
    int number_of_flats(Apartment* aprt);

    Apartment *apartment_to_add = find_apartment(head,apartment_name);
    Flat *new_flat;
    int initial_flat_count = number_of_flats(apartment_to_add);
    int empty_bandwith = (apartment_to_add->max_bandwidth) - current_bandwith(apartment_to_add);

    if (empty_bandwith >= initial_bandwidth) new_flat = create_flat(flat_id,initial_bandwidth); /* It is created with next = prev = NULL, is_empty = 0.*/
    else new_flat = create_flat(flat_id,empty_bandwith); /* It is created with next = prev = NULL, is_empty = 0. */

    if (apartment_to_add->flat_list == NULL) { /* In case there is no flat in the apartment. */
        apartment_to_add->flat_list = new_flat;
        return ;
    }

    if (index == 0){ /*In case new flat is added at the beginning of flat_list.*/
        Flat *old_head_flat = apartment_to_add->flat_list;
        apartment_to_add->flat_list = new_flat;
        new_flat->next = old_head_flat;
        old_head_flat->prev = new_flat;
    }

    else if (index == initial_flat_count){ /*In case new flat is added at the end of flat_list.*/
        Flat *old_tail_flat = nth_flat(apartment_to_add,index-1);
        old_tail_flat->next = new_flat;
        new_flat->prev = old_tail_flat;
    }

    else{ /*In case new flat is added in between.*/
        Flat *pre,*post;
        pre = nth_flat(apartment_to_add,index-1);
        post =nth_flat(apartment_to_add,index);
        pre->next = new_flat;
        new_flat->prev = pre;
        new_flat->next = post;
        post->prev = new_flat;
    }
}

Apartment* find_apartment(Apartment* head,char* apartment_name){
    /* This function returns the Apartment with name apartment_name.*/
    int compare_names(char* name1,char* name2);
    Apartment *tracker = head;
    while (1){
        if (compare_names(tracker->name,apartment_name) == 1) break;
        else tracker = tracker->next;
    }
    return tracker;
}

int compare_names(char* name1,char* name2){
    int string_length(char* name);
    int l1,l2;
    l1 = string_length(name1);
    l2 = string_length(name2);
    if (l1 != l2) return 0;
    else{
        int i,flag = 1;
        for (i=0;i<l1;i++){
            if (name1[i] != name2[i]){
                flag = 0;
                break;
            }
        }
        return flag;
    }
}

int string_length(char* name){
    int i,n = 0;
    for (i=0;;i++){
        if (name[i] == '\0') break;
        else n++;
    }
    return n;
}

Flat* nth_flat(Apartment* aprt,int index){
    int i;
    Flat *head_flat = aprt->flat_list;
    Flat *tracker = head_flat;
    for  (i=0;i<index;i++) tracker = tracker->next;
    return tracker;
}

Flat* create_flat(int flat_id,int bandwith){
    Flat *new_flat = malloc(sizeof(Flat));
    if (new_flat){
        new_flat->id = flat_id;
        new_flat->initial_bandwidth = bandwith;
        new_flat->is_empty = 0;
        new_flat->next = NULL;
        new_flat->prev = NULL;
    }
    return new_flat;
}

int current_bandwith(Apartment* aprt){
    int sum = 0;
    Flat *head_flat = aprt->flat_list;
    Flat *tracker = head_flat;
    while (tracker){
        sum += (tracker->initial_bandwidth);
        tracker = tracker->next;
    }
    return sum;
}

int number_of_flats(Apartment* aprt){
    Flat *head_flat = aprt->flat_list;
    Flat *tracker = head_flat;
    int n = 0;
    while (tracker){
        n++;
        tracker = tracker->next;
    }
    return n;
}

int index_of_apartment(Apartment* head,char* apartment_name){
    int compare_names(char* name1,char* name2);
    Apartment *tracker = head;
    int n = 0;
    while(1){
        if (compare_names(apartment_name,tracker->name) == 1) break;
        else{
            n++;
            tracker = tracker->next;
        }
    }
    return n;
}

Apartment* remove_apartment(Apartment* head, char* apartment_name){
    Apartment* find_apartment(Apartment* head,char* apartment_name);
    int index_of_apartment(Apartment* head,char* apartment_name);
    int street_length(Apartment* head);
    void free_apartment(Apartment* aprt);
    Apartment* tail_apartment(Apartment* head);

    Apartment *apartment_to_remove;
    int index;
    if (street_length(head) == 1){ /*In case there is only one apartment in the street.*/
        free_apartment(head);
        free(head);
        return NULL;
    }

    index = index_of_apartment(head,apartment_name);
    apartment_to_remove = find_apartment(head,apartment_name);

    if ((index == 0) && (street_length(head) != 1)){/*In case head of the street is removed.*/
        Apartment* tail = tail_apartment(head);
        Apartment* new_head = head->next;
        free_apartment(head);
        free(head);
        tail->next = new_head;
        return new_head;
    }

    else if (apartment_to_remove == tail_apartment(head)){ /*In case tail of the street is removed.*/
        Apartment* new_tail = nth_apartment(head,index-1);
        free_apartment(apartment_to_remove);
        free(apartment_to_remove);
        new_tail->next = head;
        return head;
    }

    else{ /* If there exists an apartment before and after the removed apartment.*/
        Apartment *pre = nth_apartment(head,index-1);
        Apartment *post = nth_apartment(head,index+1);
        free_apartment(apartment_to_remove);
        free(apartment_to_remove);
        pre->next = post;
        return head;
    }
}

void free_apartment(Apartment* aprt){
    /*This function frees flats of the apartment.*/
    Flat *head_flat = aprt->flat_list;
    Flat *storage;
    while(head_flat){
        storage = head_flat;
        head_flat = head_flat->next;
        free(storage);
    }
}

Apartment* tail_apartment(Apartment* head){
    Apartment *tracker = head;
    while(1){
        if (tracker->next == head) break;
        tracker = tracker->next;
    }
    return tracker;
}

void make_flat_empty(Apartment* head, char* apartment_name, int flat_id){
    Apartment* find_apartment(Apartment* head,char* apartment_name);
    Flat* find_flat(Apartment* aprt,int flat_id);

    Apartment* wanted_apartment = find_apartment(head,apartment_name);
    Flat* wanted_flat = find_flat(wanted_apartment,flat_id);
    wanted_flat->is_empty = 1;
    wanted_flat->initial_bandwidth = 0; 
}

Flat* find_flat(Apartment* aprt,int flat_id){
    Flat* head_flat = aprt->flat_list;
    Flat* tracker = head_flat;
    while (tracker){
        if (tracker->id == flat_id) break;
        else tracker = tracker->next;
    } 
    return tracker;
}

int find_sum_of_max_bandwidths(Apartment* head){
    Apartment* tail_apartment(Apartment* head);
    Apartment* tracker = head;
    Apartment* tail;
    int sum = 0;

    if (head == NULL) return 0;
    tail = tail_apartment(head);
    while (1){
        if (tracker == tail){
            sum += (tracker->max_bandwidth);
            break;
        }
        sum += (tracker->max_bandwidth);
        tracker = tracker->next;
    }
    return sum;
}

Apartment* merge_two_apartments(Apartment* head, char* apartment_name_1, char* apartment_name_2){
    Flat* tail_flat(Apartment* aprt);
    Apartment* find_apartment(Apartment* head,char* apartment_name);
    Apartment* nth_apartment(Apartment* head,int n);
    int index_of_apartment(Apartment* head,char* apartment_name);

    Apartment* apartment1 = find_apartment(head,apartment_name_1);
    Apartment* apartment2 = find_apartment(head,apartment_name_2);

    /* Add max_bandwith value of second apartment to the first apartment. */
    apartment1->max_bandwidth += (apartment2->max_bandwidth);

    if (apartment1->flat_list == NULL && apartment2->flat_list == NULL); /* In case there is no flat in both apartment, there will not be any flat operation. */
    else if (apartment1->flat_list == NULL && apartment2->flat_list != NULL){
        Flat* head_of_second_apartment = apartment2->flat_list;
        apartment1->flat_list = head_of_second_apartment;
        apartment2->flat_list = NULL;
    }
    else if (apartment1->flat_list != NULL && apartment2->flat_list == NULL); /*In case there is no flat in the second apartment, there will not be any flat operation. */
    else{ /*In case there is at least one flat in both apartment. */
        Flat* tail_of_first_apartment = tail_flat(apartment1);
        Flat* head_of_second_apartment = apartment2->flat_list;
        tail_of_first_apartment->next = head_of_second_apartment;
        head_of_second_apartment->prev = tail_of_first_apartment;
        apartment2->flat_list = NULL;
    }
    /* Move of flats is completed. */
    /*******************************/
    
    /* Removal of second apartment */
    if (apartment2 == head){ /* If the apartment to be removed is at the beginning of the street.*/
        Apartment* new_head = apartment2->next;
        Apartment* tail = tail_apartment(head);
        tail->next = new_head;
        free(apartment2);
        return new_head;
    }
    
    else if (apartment2 == tail_apartment(head)){ /* If the apartment to be removed is at the end of the street.*/
        int index_of_tail = index_of_apartment(head,apartment2->name);
        Apartment* new_tail = nth_apartment(head,index_of_tail-1);
        new_tail->next = head;
        free(apartment2);
        return head;
    }

    else{
        int index = index_of_apartment(head,apartment2->name);
        Apartment* previous_apartment = nth_apartment(head,index-1);
        Apartment* next_apartment = nth_apartment(head,index+1);
        previous_apartment->next = next_apartment;
        free(apartment2);
        return head;
    }
}

Flat* tail_flat(Apartment* aprt){
    Flat* tracker = aprt->flat_list;
    if (tracker == NULL) return NULL;
    while (tracker->next != NULL){
        tracker = tracker->next;
    }
    return tracker;
}

Flat* search_flat_in_street(Apartment* head,int flat_id){
    int street_length(Apartment* head);
    Apartment* nth_apartment(Apartment* head,int n);
    int number_of_flats(Apartment* aprt);
    Flat* nth_flat(Apartment* aprt,int index);

    int i,j;
    int l_street = street_length(head);
    
    for (i=0;i<l_street;i++){
        Apartment* current_apartment = nth_apartment(head,i);
        int n_flat = number_of_flats(current_apartment);
        for (j=0;j<n_flat;j++){
            if (nth_flat(current_apartment,j)->id == flat_id){
                return nth_flat(current_apartment,j);
            }
        }
    }

    return NULL;
}

Apartment* find_apartment_of_flat(Apartment* head,int flat_id){
    int street_length(Apartment* head);
    Apartment* nth_apartment(Apartment* head,int n);
    int number_of_flats(Apartment* aprt);
    Flat* nth_flat(Apartment* aprt,int index);

    int i,j;
    int l_street = street_length(head);
    
    for (i=0;i<l_street;i++){
        Apartment* current_apartment = nth_apartment(head,i);
        int n_flat = number_of_flats(current_apartment);
        for (j=0;j<n_flat;j++){
            if (nth_flat(current_apartment,j)->id == flat_id){
                return current_apartment;
            }
        }
    }

    return NULL;
}

void relocate_flats_to_same_apartment(Apartment* head, char* new_apartment_name, int flat_id_to_shift, int* flat_id_list){
    Flat* search_flat_in_street(Apartment* head,int flat_id);
    Apartment* find_apartment_of_flat(Apartment* head,int flat_id);
    int number_of_flats(Apartment* aprt);

    int i,n = 0;
    Apartment* relocation_apartment = find_apartment(head,new_apartment_name);
    Flat* flat_to_be_shifted = search_flat_in_street(head,flat_id_to_shift);
    Flat* shifting_tracker = flat_to_be_shifted;

    for (i=0;flat_id_list[i]!=0;i++) n++;


    for (i=n-1;i>=0;i--){
        Flat* current_flat = search_flat_in_street(head,flat_id_list[i]);
        Apartment* current_apartment = find_apartment_of_flat(head,flat_id_list[i]);
        current_apartment->max_bandwidth -= (current_flat->initial_bandwidth);
        relocation_apartment->max_bandwidth += (current_flat->initial_bandwidth);
        if ((current_flat->prev == NULL) && (number_of_flats(current_apartment) == 1)){
            current_flat->next = NULL;
            current_flat->prev = NULL;
            current_apartment->flat_list = NULL;
        }
        
        
        else if ((current_flat->prev == NULL) && (number_of_flats(current_apartment) != 1)){ /* Head is reallocated.*/
            current_apartment->flat_list = current_flat->next;
            current_flat->next->prev = NULL;
            current_flat->next = NULL;
            current_flat->prev = NULL;
        }

        else if (current_flat->next == NULL){ /* Tail is reallocated.*/
            Flat* new_tail = current_flat->prev;
            current_flat->next = NULL;
            current_flat->prev = NULL;
            new_tail->next = NULL;
        }

        else { /* An intermediate flat is reallocated.*/
            Flat* cur_prev = current_flat->prev;
            Flat* cur_next = current_flat->next;
            current_flat->next = NULL;
            current_flat->prev = NULL;
            cur_prev->next = cur_next;
            cur_next->prev = cur_prev;
        }

        if (shifting_tracker->prev == NULL){ /* Head is shifted. */
            relocation_apartment->flat_list = current_flat;
            current_flat->prev = NULL;
            current_flat->next = shifting_tracker;
            shifting_tracker->prev = current_flat;
        }

        else{
            Flat* previous_of_shifting_tracker = shifting_tracker->prev;
            previous_of_shifting_tracker->next = current_flat;
            current_flat->prev = previous_of_shifting_tracker;
            current_flat->next = shifting_tracker;
            shifting_tracker->prev = current_flat;
        }

        shifting_tracker = current_flat;
    }
}