/* Given structs */
typedef struct Student
{
    int studentID;
} Student;

struct Node
{
    int data;
    struct Node *next;
};
typedef struct Node Node;
/* Given structs */

Student** create_student_array(int* student_id_list, int student_count);

Node* insert_head(Node* head, int new_node_data);

Node* remove_kth_node(Node* head, int index);

int find_occurrences(Node* head, int data);

Node* copy_to_head(Node* head, int index);

Node* sum_consecutive_pairs(Node* head);