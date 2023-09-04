/* Given structs */

typedef struct Teacher
{
  int teacherID;
} Teacher;

typedef struct Student
{
  int studentID;
  Teacher* best_teacher;
} Student;

struct Node
{
  int data;
  struct Node *next;
};
typedef struct Node Node;

/* Given structs */


/* Helper functions */

Node* create_list(int* data, int data_len);

void print_list(Node* head);

/* Helper functions */


Student** create_student_array_with_teachers(int* student_id_list, int* teacher_id_list, int student_and_teacher_count);

Node* insert_tail(Node* head, int new_node_data);

Node* remove_kth_node_reversely(Node* head, int index);

int find_occurrences_of_second_list(Node* head, Node* second_head);

Node* cut_and_paste(Node* head, int index_to_cut, int index_to_paste);

Node* sum_symmetric_pairs(Node* head);