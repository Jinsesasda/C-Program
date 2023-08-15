#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "student.h"

void init_student(Student *const student, const char first_name[], const char last_name[]) {
  if (student != NULL && first_name != NULL && last_name != NULL) {
    student -> first_name = malloc(strlen(first_name) + 1);
    student -> last_name = malloc(strlen(last_name) + 1);
    strcpy(student -> first_name, first_name);
    strcpy(student -> last_name, last_name);
  }
}

Student *create_student(const char first_name[], const char last_name[]) {
  Student *student = malloc(sizeof(Student));
  if (first_name == NULL || last_name == NULL)
    return NULL;
  student -> first_name = malloc(strlen(first_name) + 1);
  student -> last_name = malloc(strlen(last_name) + 1);
  strcpy(student -> first_name, first_name);
  strcpy(student -> last_name, last_name);
  return student;
}

void print_student(Student student) {
  if (student.last_name != NULL && student.first_name != NULL)
    printf("%s, %s", student.last_name, student.first_name);
  if (student.last_name != NULL && student.first_name == NULL)
    printf("%s", student.last_name);
  if (student.first_name != NULL && student.last_name == NULL)
    printf(", %s", student.first_name);
}

unsigned int change_first_name(Student *const student, const char new_name[]) {
  if (student == NULL || new_name == NULL)
    return 0;
  free(student -> first_name);
  student -> first_name = malloc(strlen(new_name) + 1);
  strcpy(student -> first_name, new_name);
  return 1;
}

unsigned int change_last_name(Student *const student, const char new_name[]) {
  if (student == NULL || new_name == NULL)
    return 0;
  free(student -> last_name);
  student -> last_name = malloc(strlen(new_name) + 1);
  strcpy(student -> last_name, new_name);
  return 1;
}

unsigned int add_nickname(Student *const student, const char nickname[]) {
  /* +4 to the size for the (), space, and null elements in the char array */
  unsigned int new_size = strlen(student -> first_name) + strlen(nickname) + 4;
  if (strchr(student -> first_name, '(') != NULL || nickname == NULL
      || student == NULL)
    return 0;
  student -> first_name = realloc(student -> first_name, new_size);
  strcat(student -> first_name, " (");
  strcat(student -> first_name, nickname);
  strcat(student -> first_name, ")");
  return 1;
}

int compare(Student student1, Student student2) {
  if (strcmp(student1.last_name, student2.last_name) > 0)
    return 1;
  if (strcmp(student1.last_name, student2.last_name) < 0)
    return -1;
  if (strcmp(student1.last_name, student2.last_name) == 0) {
    if (strcmp(student1.first_name, student2.first_name) > 0)
      return 1;
    if (strcmp(student1.first_name, student2.first_name) < 0)
      return -1;
  }
  return 0;
}

unsigned int reset_student(Student *const student) {
  free(student -> first_name);
  free(student -> last_name);
  student -> first_name = NULL;
  student -> last_name = NULL;
  return 1;
}

unsigned int free_student(Student **const student) {
  if (student == NULL || (*student) == NULL || (*student) -> first_name == NULL
      || (*student) -> last_name == NULL)
    return 0;
  free((*student) -> first_name);
  free((*student) -> last_name);
  free((*student));
  (*student) = NULL;
  return 1;
}
