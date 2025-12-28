#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_NAME "tasks.txt"
#define MAX 200

struct Task {
    int id;
    char title[150];
    char date[20];
    int isCompleted;
};

//  DATE VALIDATION FUNCTION
int isValidDate(const char *date) {
    if (!date) return 0;
    if (strlen(date) != 10) return 0;          // exactly DD-MM-YYYY
    if (date[2] != '-' || date[5] != '-') return 0;

    int d, m, y;
    if (sscanf(date, "%2d-%2d-%4d", &d, &m, &y) != 3) return 0;

    if (y < 1900 || y > 2100) return 0;
    if (m < 1 || m > 12) return 0;

    int mdays;
    switch (m) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            mdays = 31; break;
        case 4: case 6: case 9: case 11:
            mdays = 30; break;
        case 2: {
            int leap = ( (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0) );
            mdays = leap ? 29 : 28;
            break;
        }
        default:
            return 0;
    }

    if (d < 1 || d > mdays) return 0;

    return 1;
}

#include <time.h>

/* Checks if the entered date is today or in the future */
int isFutureOrToday(const char *date) {
    int d, m, y;
    if (sscanf(date, "%2d-%2d-%4d", &d, &m, &y) != 3) return 0;

    // Get current date
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    int cd = t->tm_mday;
    int cm = t->tm_mon + 1;
    int cy = t->tm_year + 1900;

    // Check year
    if (y > cy) return 1;
    if (y < cy) return 0;

    // Same year, check month
    if (m > cm) return 1;
    if (m < cm) return 0;

    // Same month, check day
    if (d >= cd) return 1;

    return 0;
}

int isOverdue(const char *date) {
    int d, m, y;
    if (sscanf(date, "%2d-%2d-%4d", &d, &m, &y) != 3) return 0;

    // Get current date
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    int cd = t->tm_mday;
    int cm = t->tm_mon + 1;
    int cy = t->tm_year + 1900;

    // Past year
    if (y < cy) return 1;
    if (y > cy) return 0;

    // Same year, past month
    if (m < cm) return 1;
    if (m > cm) return 0;

    // Same month, past day
    if (d < cd) return 1;

    return 0;
}





void loadTasks(struct Task tasks[], int *count);
void saveTasks(struct Task tasks[], int count);
void addTask(struct Task tasks[], int *count);
void viewTasks(struct Task tasks[], int count);
void markCompleted(struct Task tasks[], int count);
void deleteTask(struct Task tasks[], int *count);
void editTask(struct Task tasks[], int count);
void showSummary(struct Task tasks[], int count);

int main() {
    struct Task tasks[MAX];
    int count = 0;
    int choice;

    loadTasks(tasks, &count);

    while (1) {
        printf("\n=========== PRODUCTIVITY TRACKER ===========\n");
        printf("1. Add Task\n");
        printf("2. View All Tasks\n");
        printf("3. Edit Task\n");
        printf("4. Delete Task\n");
        printf("5. Mark Task as Completed\n");
        printf("6. Daily Summary\n");
        printf("7. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addTask(tasks, &count); break;
            case 2: viewTasks(tasks, count); break;
            case 3: editTask(tasks, count); break;
            case 4: deleteTask(tasks, &count); break;
            case 5: markCompleted(tasks, count); break;
            case 6: showSummary(tasks, count); break;
            case 7:
                saveTasks(tasks, count);
                printf("Data saved. Exiting...\n");
                return 0;
            default:
                printf("Invalid option!\n");
        }
    }
}



void loadTasks(struct Task tasks[], int *count) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return;

    while (fscanf(fp, "%d,%[^,],%[^,],%d\n",
                  &tasks[*count].id,
                  tasks[*count].title,
                  tasks[*count].date,
                  &tasks[*count].isCompleted) != EOF) {
        (*count)++;
    }
    fclose(fp);
}

void saveTasks(struct Task tasks[], int count) {
    FILE *fp = fopen(FILE_NAME, "w");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%s,%d\n",
                tasks[i].id,
                tasks[i].title,
                tasks[i].date,
                tasks[i].isCompleted);
    }
    fclose(fp);
}

void addTask(struct Task tasks[], int *count) {
    struct Task t;
    t.id = *count + 1;

    printf("Enter task title: ");
    fgets(t.title, sizeof(t.title), stdin);
    t.title[strcspn(t.title, "\n")] = 0;

    
    do {
    printf("Enter date (DD-MM-YYYY): ");
    fgets(t.date, sizeof(t.date), stdin);
    t.date[strcspn(t.date, "\n")] = 0;

    if (!isValidDate(t.date) || !isFutureOrToday(t.date))
        printf("Invalid or past date! Try again.\n");

} while (!isValidDate(t.date) || !isFutureOrToday(t.date));


    t.isCompleted = 0;
    tasks[*count] = t;
    (*count)++;

    printf("Task added successfully!\n");
    saveTasks(tasks, *count);   

}


void viewTasks(struct Task tasks[], int count) {
    if (count == 0) {
        printf("No tasks found.\n");
        return;
    }

    printf("\n--------- TASK LIST ---------\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d\nTitle: %s\nDate: %s",
               tasks[i].id,
               tasks[i].title,
               tasks[i].date);

        // Show overdue warning (only if not completed)
        if (!tasks[i].isCompleted && isOverdue(tasks[i].date))
            printf(" OVERDUE!");

        // Print status AFTER overdue check
        printf("\nStatus: %s\n\n",
               tasks[i].isCompleted ? "Completed" : "Pending");
    }
}


void editTask(struct Task tasks[], int count) {
    int id;
    printf("Enter task ID to edit: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {
            printf("New title: ");
            fgets(tasks[i].title, sizeof(tasks[i].title), stdin);
            tasks[i].title[strcspn(tasks[i].title, "\n")] = 0;

            printf("New date (DD-MM-YYYY): ");
do {
    fgets(tasks[i].date, sizeof(tasks[i].date), stdin);
    tasks[i].date[strcspn(tasks[i].date, "\n")] = 0;

    if (!isValidDate(tasks[i].date) || !isFutureOrToday(tasks[i].date))
        printf(" Invalid! Date must be today or future. Try again: ");

} while (!isValidDate(tasks[i].date) || !isFutureOrToday(tasks[i].date));


            printf("Task updated!\n");
            saveTasks(tasks, count);   

            return;
        }
    }
    
}

void deleteTask(struct Task tasks[], int *count) {
    int id;
    printf("Enter task ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < *count; i++) {
        if (tasks[i].id == id) {
            for (int j = i; j < *count - 1; j++)
                tasks[j] = tasks[j + 1];
            (*count)--;

            printf("Task deleted successfully!\n");
            saveTasks(tasks, *count);   

            return;
        }
    }
    printf("Task not found.\n");
}

void markCompleted(struct Task tasks[], int count) {
    int id;
    printf("Enter task ID to mark completed: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {
            tasks[i].isCompleted = 1;
            printf("Task marked as completed!\n");
            saveTasks(tasks, count);   

            return;
        }
    }
    printf("Task not found.\n");
}

void showSummary(struct Task tasks[], int count) {
    int completed = 0, pending = 0;

    for (int i = 0; i < count; i++) {
        if (tasks[i].isCompleted)
            completed++;
        else
            pending++;
    }

    printf("\n------ DAILY SUMMARY ------\n");
    printf("Total Tasks: %d\n", count);
    printf("Completed : %d\n", completed);
    printf("Pending   : %d\n", pending);
}


