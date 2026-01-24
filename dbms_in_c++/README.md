# DATABASE MANAGEMENT SYSTEM WITH C/C++

## [Basics](basics)
- **Database Connection**: Establish connection using `mysql_real_connect()`
- **Database Creation**: Execute `CREATE DATABASE` query
- **Modification**: Use `ALTER TABLE` statements
- **Deletion**: Implement `DROP TABLE` or `DELETE` operations
- **Query Execution**: Perform CRUD operations with `mysql_query()`

## [SIMPLE PROGRAMS](simple%20programs)
multiple simple programs which are using mysql c library  api ( uses libmysqlclient, which is the native C connector for MySQL.) 

- ### [**user_info.cpp**](simple%20programs/user_info.cpp)
     A multithreaded C++ application that stores user information in a MySQL database. Features include:
    - **Multithreading**: Separate thread for user input operations
    - **Signal Handling**: Gracefully switches between input modes using `SIGQUIT` (Ctrl+\\)
    - **Manual Query Execution**: Allows direct SQL queries after interrupting input mode

   #### Example :
  <img width="1670" height="200" alt="Screenshot From 2026-01-25 01-39-58" src="https://github.com/user-attachments/assets/9fdc862e-0fdc-4d85-b7f3-83fddf5cc329" />
  <img width="1403" height="358" alt="Screenshot From 2026-01-25 01-40-47" src="https://github.com/user-attachments/assets/83a075da-22c9-49ff-96b5-d78801af9610" />
