// code explains the workiing of DB. 
// Functionality : Describes the Schema of a Database

#include <iostream>
#include <mysql/mysql.h>
#include "decor.h"
#include <iomanip>
#define COLUMN_WIDTH 20
using namespace std;

class database {
  private:
    MYSQL* conn;

public:
    database() {
        conn = mysql_init(nullptr);
        if (!conn) {
            printf("%s mysql_init() Failed !%s\n", FG_RED, RESET);
            }
        }
    ~database() {
        mysql_close(conn);
        }
    // establish connection with database
    bool connect(const char* host, const char* user, const char* passwd, const char* dbname) {
        return mysql_real_connect(conn, host, user, passwd, dbname, 0, nullptr, 0) != nullptr;
        }

    // execute queries
    bool exe_query(const char* query) {
        cout << "Executing: " << query << endl;
        if (mysql_query(conn, query)) {
            printf("[%sError%s] Query execution Failed \n", FG_RED, RESET);
            cout<< mysql_error(conn);
            return false;
            }
        cout << FG_GREEN << "Query Executed Successfully\n" << RESET;
        return true;
        }

    void geterror() {
      
        if (const char* error = mysql_error(conn))
            printf("[%sError] : %s%s", FG_RED, error, RESET);
        }

    void display_results() {
        MYSQL_RES* result = mysql_store_result(conn);
        if (!result) {
            printf("[%sError%s] Failed to store Result (%s) ", FG_RED, RESET, mysql_error(conn));
            return;
            }
        MYSQL_ROW row;
        /*--Type Definition: MYSQL_ROW is typically defined as a pointer to a pointer to a character: char **.
          --Array of Strings: It acts as an array where each element (row[0], row[1], etc.) is a char* pointer
             to the value of a single column in that row.
          --NULL Values: If a particular column in the SQL result is NULL, the corresponding char* pointer
             in the MYSQL_ROW array will be a NULL pointer.
          */
        unsigned int num_fields = mysql_num_fields(result);
        MYSQL_FIELD* fields = mysql_fetch_fields(result);
        puts(BOLD);
        for (unsigned int i = 0;i < num_fields;i++) {
            cout <<left<<setw(COLUMN_WIDTH)<< fields[i].name ;
            }
        puts(RESET);
        cout << endl;

        while ((row = mysql_fetch_row(result)) != nullptr) {
            for (unsigned int i = 0; i < num_fields;i++) {
                if (row[i] == nullptr) {
                    cout <<left<<setw(COLUMN_WIDTH)<< "NULL";
                    continue;
                    }
                cout<<left<<setw(COLUMN_WIDTH) << row[i];
                }
            cout << endl;
            }
        mysql_free_result(result);
        }

    };

int main() {
    database db;

    if (!db.connect("localhost", "root", "add your own passwd", "add your own dbname")) {
        cerr << FG_RED << "[Error]" << RESET << "Connected Failed!" << endl;
        db.geterror();
        return 1;
        }
    db.exe_query("DESCRIBE players");
    db.display_results();
    return 0;
    }