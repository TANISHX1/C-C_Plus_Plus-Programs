#include <mysql/mysql.h>
#include <iostream>
using namespace std;

class MySQLConnection {
private:
    MYSQL* conn;
    bool connected;
public:
    // constructor
    MySQLConnection() : conn(nullptr), connected(false) {
        conn = mysql_init(nullptr);
        if (!conn) {
            cerr << "mysql_init() failed\n";
            }
        }

    // Deconstrutor
    ~MySQLConnection() {
        if (conn != nullptr) {
            mysql_close(conn);
            cout << "Connection is automatically closed..\n";
            }
        }

    // connect to database
    bool connect(const string& host, const string& user, const string& password,
        const string& database = "")
        {
        if (conn == nullptr) {
            return false;
            }
        if (mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str()
            , database.empty() ? nullptr : database.c_str(), 0, nullptr, 0) == nullptr) {
            cerr << "Connection error: " << mysql_error(conn) << endl;
            return false;
            }
        connected = true;
        cout << "Connected to :" << host << endl;
        return true;
        }
    // check connection
    bool isConnected() const {
        return connected;
        }
    // get connection
    MYSQL* getConnection() {
        return conn;
        }
    // Get errors
    string getError() const {
        return conn ? mysql_error(conn) : "No Connection";
        }
    };

int main() {
    MySQLConnection db;

    if (!db.connect("localhost", "root", "add your own passwd", "game")) {
        cerr << "Failed to connect\n";
        return 1;
        }
    cout << "Connection successful\n";
    cout << "exiting...\n";
    return 0; 
    }