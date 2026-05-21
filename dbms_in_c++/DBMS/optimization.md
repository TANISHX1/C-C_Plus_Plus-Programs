

### Indexing 
indexing is a data structure architecture use to speed up the retrieval of a record . without indexing ,system will perform a full scan ,it will perform I/O operation and takes the pages to the memory and then search the record . starting form first page to last page .
But indexing helps to improve this . it builds a B+ tree for a certain column and then only loads necessary pages to the memory which improves the performance 

## 1. Core Mechanism



### Vacuum
it is used to free the unused or garbage memory ,which is hold by the database . it checks the bytes which are no longer is in use of database and de-allocates that bytes and give it back to OS. 
- if there are more bytes to de-allocates ,then it will take more time to complete  
- used to reduce the unwanted size of the database 

## Concurrency
In DBMS , concurrency plays a important role in running multiple queries at same time (like in websites , payments , shopping applications etc..)
### Transaction
A unit of work in a database  that executes in a sequence of DB operations (read ,write , delete ) as an all -or noting block (atomic operation)
either every single operation in a transaction succeeds and saved permanently or rollback to original state ,when any operation in transaction fails to guarantees the data integrity 
### Properties
- **Atomicity** : smallest transaction or operation that can not be breakdown to more smaller transactions or operations 
- **Consistency** : Ensures that no DB constraint well violate
- **isolation** : no transaction will interfere other transaction
- **Durability**