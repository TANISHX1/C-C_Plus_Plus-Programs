// simple real database connection test 
#include <mysql/mysql.h>
#include <iostream>
#include <cstring>

using namespace std;

int main() {
    MYSQL* conn = mysql_init(nullptr);
    if (!conn) {
        cerr << "mysql_init() failed\n";
        return 1;
        }
    const char* host = "localhost";
    const char* user = "root";
    const char* password = "add your own passwd";
    const char* database = "mysql";
    unsigned int port = 3306;
    cout << "Connecting to Mysql Server...\n";

    if (mysql_real_connect(conn, host, user, password, database, port, nullptr, 0) == nullptr) {
        cerr << "connection failed\n";
        cerr << "Error: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return 1;
        }

    cout << "connected Successfully\n";
    cout << "server Version: " << mysql_get_server_info(conn) << endl;
    cout << "Host Info: " << mysql_get_host_info(conn) << endl;

    mysql_close(conn);
    cout << "Connection Closed\n";
    return 0;
} 