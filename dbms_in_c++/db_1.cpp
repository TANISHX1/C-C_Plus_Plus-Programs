// Simple program to create a database in Maria DB. 
// RAII (Resource Acquisation in intialization) - connection closes automatically
#include <iostream>
#include <mysql/mysql.h>
#include "decor.h"
using namespace std;

class database {
private:
    // MYSQL*conn it creates a connection handler .which has all info about the db connection
    // like: host,port,state etc

    MYSQL* conn;
public:

    database() {
        //it allocate memory and initialises MYSQL structure
          // it only allocates /prepares the memory (for MYSQL object) required for connection
    // at that time conn has no connection information (or defult values )
        conn = mysql_init(nullptr);
        if (!conn) {
            cerr << "Failed to initalize [mysql_init()]\n";
            }
        }
    ~database() {

        mysql_close(conn);
        }
    bool connect(const char* host, const char* passwd, const char* user) {
        // it establish a TCP/IP or UNIX connection (if you have passed 'localhost',something like that cases) to the server
        // at this state conn has live connection informations
        return mysql_real_connect(conn, host, user, passwd, nullptr, 0, nullptr, 0) != nullptr;
        }

    bool executequery(const string& query) {
        cout <<"Executing: " << query << endl;

        // send SQL commands /Query to the server
        // it transmits the string over the established connection 
        if (mysql_query(conn, query.c_str())) {
            cerr << "[ Error ] : " << mysql_error(conn) << endl;
            return false;
            }
        cout <<FG_GREEN << "Query Executed Successfully!\n"<<RESET;
        return true;
        }

    void display_results() {
        MYSQL_RES* result; // stores the result 
        MYSQL_ROW row; // stores one row of the result
        result = mysql_store_result(conn); //retrieve the result from the server
        /*mysql_store_result(conn) : it fetch 1 row (does not download entire DB).
        --use when DB is large (to save local resources)*/

        if (!result) {
            cerr << "[Error] Failed to store result :" << mysql_error(conn) << endl;
            return;
            }

        // Print header
        unsigned int num_fields = mysql_num_fields(result); //
        MYSQL_FIELD* fields = mysql_fetch_fields(result);
        for (unsigned int i = 0;i < num_fields;i++) {
            cout << fields[i].name << "\t\t";
            }
        cout << endl;

        // Fetch and print all
        /* mysql_fetch_row(result) : it fetch one row at a time
           in every iteration it moves to next row */
        while ((row = mysql_fetch_row(result)) != nullptr) {
            for (unsigned int i = 0;i < num_fields;i++) {
                cout << row[i] << "\t\t";
                }
            cout << endl;
            }
        mysql_free_result(result); //free memory allocated for the result
        }

    string geterror() {
        return mysql_error(conn);
        }

    };

int main() {
    database db;
    if (!db.connect("localhost", "add your own passwd", "root")) {
        cerr << "connection failed !\n";
        return 1;
        }
    cout <<FG_GREEN << "connected to Meria Server!\n"<<RESET;
    string dbname = "testing";
    string query = "CREATE DATABASE IF NOT EXISTS " + dbname;
    if (db.executequery(query)) {
        cout<<FG_GREEN  << "Database ' " << dbname << " ' Created!\n"<<RESET;
        }
    db.executequery("SHOW DATABASES");
    cout <<FG_BMAGENTA<< "\n---------Avaiable Database-----------\n"<<RESET;
    db.display_results();
    return 0;
    }