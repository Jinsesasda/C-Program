/* DO NOT MODIFY THIS FILE OR YOUR CODE WILL NOT COMPILE ON THE SUBMIT
 * SERVER. */

typedef struct {
  char *first_name;
  char *last_name;
} Student;

void init_student(Student *const student, const char first_name[],
                  const char last_name[]);
Student *create_student(const char first_name[], const char last_name[]);
void print_student(Student student);
unsigned int change_first_name(Student *const student, const char new_name[]);
unsigned int change_last_name(Student *const student, const char new_name[]);
unsigned int add_nickname(Student *const student, const char nickname[]);
int compare(Student student1, Student student2);
unsigned int reset_student(Student *const student);
unsigned int free_student(Student **const student);
