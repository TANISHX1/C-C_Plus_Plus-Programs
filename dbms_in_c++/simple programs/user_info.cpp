// application that takes user info and saves them in db.
// uses multithreading and Signal Handling 
// Sequential switching between data inserting (Insert Data) to Manual Query (Query Executions)

#include <iostream>
#include <iomanip>
#include <mysql/mysql.h>
#include <signal.h>
#include <pthread.h>
#include "../decor.h"
#include "../basics/info.h"
#include <sstream>  //  this is for ostringstream
#include <unistd.h>
#define COLUMN_WIDTH 25
using namespace std;

class Database {
private:
    MYSQL* conn;
public:
    Database() {
        conn = mysql_init(nullptr);
        if (!conn) {
            printf("[%sError%s]{connection} Failed to Initialized Connection (mysql_init)\n", FG_RED, RESET);
            }
        }

    ~Database() {
        mysql_close(conn);
        printf("\n[%s Connection handler %s] Connection closed Successfully\n", FG_GREEN, RESET);
        }

    bool connect(const char* host, const char* user, const char* passwd, const char* db_name) {
        return mysql_real_connect(conn, host, user, passwd, db_name, 0, nullptr, 0) != nullptr;
        }

    bool execute(const char* Query) {

        if (mysql_query(conn, Query)) {
            printf("[%sError%s]{Query} Query Exection Failed (\n%s)\n", FG_RED, RESET, get_error());
            return false;
            }
        printf("[%sQuery%s] Query Successfuly Executed (\n%s\t)", FG_GREEN, RESET, Query);
        return true;
        }

    void display_result() {
        MYSQL_RES* result = mysql_store_result(conn);
        if (!result) {
            printf("[%s Error %s] {Display} %s\\n", FG_RED, RESET, get_error());
            }

        MYSQL_ROW row;
        unsigned int field_num = mysql_num_fields(result);
        MYSQL_FIELD* fields = mysql_fetch_field(result);
        puts(BOLD);
        for (unsigned int i = 0;i < field_num;i++) {
            cout << left << setw(COLUMN_WIDTH) << fields[i].name;
            }
        puts(RESET);
        while ((row = mysql_fetch_row(result)) != nullptr) {
            for (unsigned int i = 0;i < field_num;i++) {
                if (row[i] == nullptr) {
                    cout << left << setw(COLUMN_WIDTH) << "NULL";
                    }
                cout << left << setw(COLUMN_WIDTH) << row[i];
                }
            cout << endl;
            }
        mysql_free_result(result);
        }

    const char* get_error() {
        return mysql_error(conn);
        }
    };

Database db;

volatile u_int8_t running = true;

void* db_input(void*) {

    mysql_thread_init();
    string name, email;
    char grade;
    short int age;

    while (running) {


        cout << "[ Thread ] Enter your Name:\t" << flush;
        if (!(getline(cin >> ws, name))) break;
        cout << "[ Thread ] Enter your Age:\t\t";cin >> age;
        cout << "[ Thread ] Enter Email (example@gmail.com):\t"; cin >> email;
        cout << "[ Thread ] Enter Grade:\t";cin >> grade;
        /*
        ostringstream is a C++ class from the <sstream>
        header that allows you to build strings using stream operations.
        It's part of the Standard Template Library (STL) and
        provides a convenient way to construct formatted strings.
        */
        // Query making
        ostringstream query;
        query << "INSERT INTO students (name,age,grade,email) VALUES ('"
            << name << "','" << age << "','" << grade << "','" << email << "')";

        db.execute(query.str().c_str());
        //query.str() return string , we need const char* ,that's why we used .c_str()
        }

    mysql_thread_end(); //cleanup thread variables
    return NULL;
    }



int main() {

    // signal handling
    sigset_t set;
    int sig;

    // 1. prepare the signal set and block SIGQUIT
    sigemptyset(&set);
    sigaddset(&set, SIGQUIT);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    // Database connection
    if (!db.connect(host, user, passwd, "testing")) {
        printf("[%s Error %s] Failed to connect\n", FG_RED, RESET);
        return 0;
        }
    cout << "Database Connected Successfuly\n";

    //   input thread creation
    pthread_t input_t;
    pthread_create(&input_t, NULL, db_input, NULL);

    // Main thread will waits for signal
    sigwait(&set, &sig);
    running = false;
    cout << " Signal Caught [Closing input thread ]..\n";
    pthread_cancel(input_t);
    pthread_join(input_t, nullptr);

    puts("\t\tEntring Manual Query Mode");
    /* Why using cin.clear()
    Reason :cin.clear(): When you cancel a thread that is using cin,
            it often sets the failbit or eofbit. Without clear(),
            every future getline will return false immediately without
            waiting for you to type.
    */
    cin.clear();
    running = true;
    string result_query;
    while (running) {
        cout << "\n Enter Query:\t";
        if(!(getline(cin>>ws,result_query)))break; // this is used to remove the leading whitespaces in starting
        if (!db.execute(result_query.c_str())) {
            continue;
            }
        db.display_result();
        }
    exit(0);
    }