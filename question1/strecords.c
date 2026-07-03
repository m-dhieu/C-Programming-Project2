#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    float grade;
} student;  // student struct

typedef void (*add)(student **, int *);
typedef void (*display)(student *, int);
typedef void (*sort)(student *, int);
typedef float (*avg)(student *, int);
typedef student *(*top)(student *, int);
typedef int (*search)(student *, int, const char *);
typedef float (*custom)(student *, int);

float grade_var(student *arr, int n);

void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}  // clear leftover newline

void read_line(char *buf, int max_len) {
    if (fgets(buf, max_len, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    int len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    }
}  // read line & strip newline

void add_students(student **arr, int *n) {
    int id;
    char name[50];
    float grade;

    printf("Enter ID: ");
    scanf("%d", &id);
    clear_input();

    printf("Enter name: ");
    read_line(name, sizeof(name));

    printf("Enter grade: ");
    scanf("%f", &grade);
    clear_input();

    (*n)++;
    student *tmp = realloc(*arr, (*n) * sizeof(student));
    if (!tmp) {
        printf("Mem allocation failed.\n");
        (*n)--;
        return;
    }
    *arr = tmp;  // grow array

    (*arr)[(*n) - 1].id = id;
    strcpy((*arr)[(*n) - 1].name, name);
    (*arr)[(*n) - 1].grade = grade;

    printf("Student added successfully.\n");
}

void display_students(student *arr, int n) {
    if (n == 0) {
        printf("No students to display.\n");
        return;
    }

    printf("\nID\tName\t\tGrade\n");
    student *p = arr;
    for (int i = 0; i < n; i++) {
        printf("%d\t%s\t\t%.2f\n", p->id, p->name, p->grade);
        p++;  // pointer traversal
    }
}

int comp_grade(student a, student b) {
    if (a.grade < b.grade) return -1;
    if (a.grade > b.grade) return 1;
    return 0;
}

void sort_students(student *arr, int n) {
    if (n <= 1) return;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (comp_grade(arr[j], arr[j+1]) > 0) {
                student tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

int comp_name(student a, student b) {
    return strcmp(a.name, b.name);
}  // string comparison

void sort_name(student *arr, int n) {
    if (n <= 1) return;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (comp_name(arr[j], arr[j+1]) > 0) {
                student tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

float calculate_average(student *arr, int n) {
    if (n == 0) return 0.0;
    float sum = 0.0;
    student *p = arr;
    for (int i = 0; i < n; i++) {
        sum += p->grade;
        p++;
    }
    return sum / n;
}

student *find_top_student(student *arr, int n) {
    if (n == 0) return NULL;
    student *p = arr;
    student *tp = p;
    for (int i = 1; i < n; i++) {
        p++;
        if (p->grade > tp->grade) {
            tp = p;
        }
    }
    return tp;
}

int search_name(student *arr, int n, const char *name) {
    student *p = arr;
    for (int i = 0; i < n; i++) {
        if (strcmp(p->name, name) == 0) {
            return i;
        }
        p++;
    }
    return -1;
}

float grade_var(student *arr, int n) {
    if (n == 0) return 0.0;
    float avg = calculate_average(arr, n);
    float sum_sq = 0.0;
    student *p = arr;
    for (int i = 0; i < n; i++) {
        float diff = p->grade - avg;
        sum_sq += diff * diff;
        p++;
    }
    return sum_sq / n;
}

void init_db(student **arr, int *n) {
    student temp[] = {
        {1001, "Vincent Nkem", 78.5},
        {1002, "Modupe Akanni", 85.0},
        {1003, "Habeeb Dindi", 92.3},
        {1004, "Rowan Hongo", 67.0},
        {1005, "Neville Iregi", 88.7},
        {1006, "Paul Masamvu", 73.4},
        {1007, "Paul Muwanguzi", 90.1},
        {1008, "James Giathi", 65.8},
        {1009, "Christian N.", 81.2},
        {1010, "Albert N.", 77.9},
        {1011, "Innocent N.", 84.6},
        {1012, "Pascal Nzigo", 89.0}
    };

    int N = sizeof(temp) / sizeof(temp[0]);  // db size

    *arr = malloc(N * sizeof(student));
    if (!*arr) {
        printf("Mem allocation failed.\n");
        *n = 0;
        return;
    }

    for (int i = 0; i < N; i++) {
        (*arr)[i].id = temp[i].id;
        strcpy((*arr)[i].name, temp[i].name);
        (*arr)[i].grade = temp[i].grade;
    }

    *n = N;
}  // initialize students

int main(void) {
    student *students = NULL;
    int n = 0;

    int choice;

    add add_op = add_students;
    display display_op = display_students;
    sort sort_grade_op = sort_students;
    sort sort_name_op = sort_name;
    avg avg_op = calculate_average;
    top top_op = find_top_student;
    search search_op = search_name;
    custom custom_op = grade_var;

    init_db(&students, &n);  // load dataset

    printf("Student ID: ALU1000\n");
    printf("Analysis Function: Grade Variance\n");
    printf("Initial dataset loaded with %d students.\n", n);

    while (1) {
        printf("\n1) Add student\n");
        printf("2) Display students\n");
        printf("3) Sort students by grade\n");
        printf("4) Sort students by name\n");
        printf("5) Calculate average grade\n");
        printf("6) Find top student\n");
        printf("7) Search student by name\n");
        printf("8) Analyze grade variance\n");
        printf("9) Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        clear_input();

        if (choice == 1) {
            add_op(&students, &n);
        } else if (choice == 2) {
            display_op(students, n);
        } else if (choice == 3) {
            sort_grade_op(students, n);
            printf("Sorted successfully by grade.\n");
        } else if (choice == 4) {
            sort_name_op(students, n);
            printf("Sorted successfully by name.\n");
        } else if (choice == 5) {
            float av = avg_op(students, n);
            printf("Average grade: %.2f\n", av);
        } else if (choice == 6) {
            student *tp = top_op(students, n);
            if (tp) {
                printf("Top student: ID=%d Name=%s Grade=%.2f\n",
                       tp->id, tp->name, tp->grade);
            } else {
                printf("No students yet.\n");
            }
        } else if (choice == 7) {
            char name[50];
            printf("Enter student name to search: ");
            read_line(name, sizeof(name));
            int idx = search_op(students, n, name);
            if (idx >= 0) {
                student *s = &students[idx];
                printf("Found: ID=%d Name=%s Grade=%.2f\n",
                       s->id, s->name, s->grade);
            } else {
                printf("Student not found.\n");
            }
        } else if (choice == 8) {
            float var = custom_op(students, n);
            printf("Grade variance: %.4f\n", var);
        } else if (choice == 9) {
            free(students);
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}

