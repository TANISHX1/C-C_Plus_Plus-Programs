#include<iostream>
#include "decor.h"
#include <mysql/mysql.h>
using namespace std;
#define COLUMN_WIDTH 20
#include <iomanip>

class Database {
private:
    MYSQL* conn;
public:
    Database() {
        conn = mysql_init(nullptr);
        if (!conn) {
            cerr << "DataBase Object (Conn) Not Initialized [Initilization failed] \n";

            }
        }
    ~Database() {
        mysql_close(conn);
        }
    bool connect(const char* host, const char* user, const char* passwd, const char* db_name) {
        return mysql_real_connect(conn, host, user, passwd, db_name, 0, nullptr, 0) != nullptr;
        }

    bool Execute(const char* Query) {
        if (mysql_query(conn, Query)) {
            cerr << FG_RED << "[Error] : Query Exection Failed (" << get_error() << ")\n";
            return false;
            }
        printf("[%sQuery%s] Exection Successfull.. ( %s )\n", FG_GREEN, RESET,Query);
        return true;
        }

    const char* get_error() {
        return mysql_error(conn);
        }

    void display_result() {
            MYSQL_RES* result = mysql_store_result(conn);
            if (!result) {
                printf("[%sError%s] Failed to Load Result (%s)", FG_RED, RESET, get_error());
                exit (1);
                }
            MYSQL_ROW row;
            unsigned int num_fields = mysql_num_fields(result);
            MYSQL_FIELD* fields = mysql_fetch_field(result);
            puts(BOLD);
            for (unsigned int i = 0;i < num_fields;i++) {
                cout << left << setw(COLUMN_WIDTH) << fields[i].name ;
                }
            puts(RESET);
            cout << endl;

            while ((row = mysql_fetch_row(result)) != nullptr) {
                for (unsigned int i = 0;i < num_fields;i++) {
                    if (row[i] == nullptr) {
                        cout << left << setw(COLUMN_WIDTH) << "NULL";
                        continue;
                        }
                    cout << left << setw(COLUMN_WIDTH) << row[i];
                    }
                cout << endl;
                }
            mysql_free_result(result);
        }
    };

int main() {
    Database db;

    if (!db.connect("localhost", "root", "Tanishp4224j", "testing")) {
        printf("[%sError%s] Conneciton Failed\n", FG_RED, RESET);
        return 1;
        }
    else {
        cout << "connected to Database\n";
        }
    const char* Query = R"(
    CREATE TABLE IF NOT EXISTS students(
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    age INT NOT NULL,
    grade CHAR(1),
    email VARCHAR(100) UNIQUE,
    enrolled_date DATE DEFAULT CURRENT_DATE)
    )";

    cout << "Creating student table...\n";
    fflush(stdout);
    if (db.Execute(Query)) {
        cout << "Table Create successfully\n";
        }
    db.Execute("DESCRIBE students");
    db.display_result();
    return 0;
    }
    