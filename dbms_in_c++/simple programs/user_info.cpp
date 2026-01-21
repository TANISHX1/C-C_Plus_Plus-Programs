#include <iostream>
#include <iomanip>
#include <mysql/mysql.h>
#include "decor.h"
#include "basics/info.h"
#include <sstream>  //  this is for ostringstream
#define COLUMN_WIDTH 25
using namespace std;

class Database {
private:
    MYSQL* conn;
public:
    Database() {
        conn = mysql_init(nullptr);
        if (!conn) {
            printf("[%sError%s]{connection} Failed to Initialized Connection (mysql_init)\n",FG_RED,RESET);
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
            printf("[%sError%s]{Query} Query Exection Failed (%s)\n", FG_RED, RESET, get_error());
            return false;
            }
            printf("[%sQuery%s] Query Successfuly Executed (\t%s\t)", FG_GREEN, RESET, Query);
            return true;
        }

    void display_result() {
        MYSQL_RES* result = mysql_use_result(conn);
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
                    cout <<left<<setw(COLUMN_WIDTH)<< "NULL";
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


int main() {
    Database db;
    if (!db.connect(host, user, passwd, "testing")) {
        printf("[%s Error %s] Failed to connect\n", FG_RED, RESET);
        return 0;
        }
    cout << "Database Connected Successfuly\n";
    string name, email;
    char grade;
    short int age;
    cout << "Enter your Name:\t";
    getline(cin, name);
    cout << "Enter your Age:\t";
    cin >> age;
    cin.ignore();
    cout << "Enter Email (example@gmail.com):\t";
    getline(cin, email);
    cout << "Enter Grade:\t";
    cin >> grade;
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
    
    if (!db.execute(query.str().c_str())) {
        //query.str() return string , we need const char* ,that's why we used .c_str()
        cout << "Student added\n";
        }

    }