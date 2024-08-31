#include <stdio.h>
#include <mysql.h>


//entering the required data for connection enter your own if you are using it
static char* host = "localhost";
static char* user = "root";
static char* pass = "password";
static char* dbname = "student";
unsigned int port = 3306;
static char* unix_socket = NULL;
unsigned int flag = 0;


void show_error(MYSQL* conn);
void make_table(MYSQL* conn);
void delete_record(MYSQL* conn);
void update_record(MYSQL* conn);
void read_records(MYSQL* conn);
void create_record(MYSQL* conn);
void advancedsearch();
void search_name(MYSQL* conn);
void search_id(MYSQL* conn);
void search_grade(MYSQL* conn);
void search_age(MYSQL* conn);


MYSQL* conn;

int main()
{
    conn = mysql_init(NULL);//intitalizing the connection withdatabase
    if (!(mysql_real_connect(conn, host, user, pass, dbname, port, unix_socket, flag)))
    {
        show_error(conn);
        mysql_close(conn);
        exit(1);
    }

    make_table(conn);

	
    int choice;
	show_error(conn);
	printf("*****Welcome to Student Management System*****");
    printf("\n Choose\n 1. Create Record\n 2. Read Records\n 3. Update records\n 4. Delete records\n 5. Advanced search");
	printf("\n Enter your choice: ");
    scanf_s("%d", &choice);

    switch (choice)
    {
        case 1:
            create_record(conn);
            break;

        case 2:
            read_records(conn);
            break;

        case 3:
            update_record(conn);
            break;

        case 4:
            delete_record(conn);
            break;

        case 5:
            advancedsearch();
            break;

        default:
            printf("Invalid Choice.\n");
            break;
    }

    mysql_close(conn);
    return 0;
}

void show_error(MYSQL* conn)
{
        fprintf(stderr, "\nError: %s [%d]\n", mysql_error(conn), mysql_errno(conn));
}

void make_table(MYSQL* conn) {
    const char* query =
        "CREATE TABLE IF NOT EXISTS students ("
        "id INT AUTO_INCREMENT PRIMARY KEY, "
        "name VARCHAR(100), "
        "age INT, "
        "grade CHAR(2))";

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Table creation failed: %s\n", mysql_error(conn));
    }
    else {
        printf("Table created or already exists.\n");
    }
}


void create_record(MYSQL* conn) {
    char name[100];
    int age;
    char grade[3];

    printf("Enter student name: ");
    scanf_s("%s", name, (unsigned int)sizeof(name));

    printf("Enter student age: ");
    scanf_s("%d", &age);

    printf("Enter student grade: ");
    scanf_s("%s", grade, (unsigned int)sizeof(grade));

    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO students (name, age, grade) VALUES ('%s', %d, '%s')", name, age, grade);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Create failed: %s\n", mysql_error(conn));
    }
    else {
        printf("Record created successfully.\n");
    }
}

void read_records(MYSQL* conn) {
    const char* query = "SELECT * FROM students";
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Read failed: %s\n", mysql_error(conn));
    }
    else {
        MYSQL_RES* result = mysql_store_result(conn);
        if (result == NULL) {
            fprintf(stderr, "Failed to store result: %s\n", mysql_error(conn));
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            printf("ID: %s, Name: %s, Age: %s, Grade: %s\n", row[0], row[1], row[2], row[3]);
        }

        mysql_free_result(result);
    }
}

void update_record(MYSQL* conn) {
    int id;
    char name[100];
    int age;
    char grade[3];

    printf("Enter the id of student to update");
    scanf_s("%d", &id);

    printf("Enter updated student name: ");
    scanf_s("%s", name, (unsigned int)sizeof(name));

    printf("Enter updated student age: ");
    scanf_s("%d", &age);

    printf("Enter updated student grade: ");
    scanf_s("%s",grade, (unsigned int)sizeof(grade));

    char query[256];
    snprintf(query, sizeof(query), "UPDATE students SET grade = '%s' ,name = '%s' ,age = %d WHERE id = %d ", grade, name, age, id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Update failed: %s\n", mysql_error(conn));
    }
    else {
        printf("Record updated successfully.\n");
    }
}

void delete_record(MYSQL* conn) {
    int id;

    printf("Enter id to delete");
    scanf_s("%d", &id);

    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM students WHERE id = %d", id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Delete failed: %s\n", mysql_error(conn));
    }
    else {
        printf("Record deleted successfully.\n");
    }
}

void advancedsearch()
{
    printf("\n1). Search using ID : \n");
    printf("2). Search using Student Name : \n");
    printf("3). Search using Student Age : \n");
    printf("4). Search using Grade : \n");
    printf("5). Exit\n");
    printf("\nEnter your Choice : ");
    int choose;
    scanf_s("%d", &choose);

    switch (choose)
    {
    case 1:
        search_id(conn);
        break;

    case 2:
        search_name(conn);
        break;

    case 3:
        search_age(conn);
        break;

    case 4:
        search_grade(conn);
        break;
    case 5:
        exit(0);
    default:
        printf("Enter Correct Choice");
        break;
    }
}

void search_name(MYSQL* conn) {
    printf("\nEnter Student Name: ");
    char name[100];
    scanf_s("%s", name, (unsigned int)sizeof(name));

    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM students WHERE name = '%s'", name);

    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Search failed: %s\n", mysql_error(conn));
        return;
    }

   
    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Could not retrieve results: %s\n", mysql_error(conn));
        return;
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields; i++) {
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }

    mysql_free_result(result);
    printf("Search completed successfully.\n");
}



void search_age(MYSQL* conn) {
    printf("\nEnter Age: ");
    int age;
    scanf_s("%d", &age);

    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM students WHERE age = %d", age);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Search failed: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Could not retrieve results: %s\n", mysql_error(conn));
        return;
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields; i++) {
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }

    mysql_free_result(result);
    printf("Search completed successfully.\n");
}


void search_id(MYSQL* conn) {
    printf("\nEnter ID: ");
    int id;
    scanf_s("%d", &id);

    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM students WHERE id = %d", id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Search failed: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Could not retrieve results: %s\n", mysql_error(conn));
        return;
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields; i++) {
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }

    mysql_free_result(result);
    printf("Search completed successfully.\n");
}


void search_grade(MYSQL* conn) {
    printf("\nEnter Grade: ");
    char grade[3];
    scanf_s("%2s", grade, (unsigned int)sizeof(grade)); // Ensure no overflow

    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM students WHERE grade = '%s'", grade);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Search failed: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Could not retrieve results: %s\n", mysql_error(conn));
        return;
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields; i++) {
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }

    mysql_free_result(result);
    printf("Search completed successfully.\n");
}
