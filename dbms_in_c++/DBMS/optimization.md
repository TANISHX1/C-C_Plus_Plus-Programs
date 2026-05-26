

### Indexing 
indexing is a data structure architecture use to speed up the retrieval of a record . without indexing ,system will perform a full scan ,it will perform I/O operation and takes the pages to the memory and then search the record . starting form first page to last page .
But indexing helps to improve this . it builds a B+ tree for a certain column and then only loads necessary pages to the memory which improves the performance 


## 1. Core Mechanism



#### Which Columns Should You Index?

This is the most common point of confusion. You shouldn't index everything. Here is a definitive guide on how to choose which columns get an index:

**1. Columns used frequently in `WHERE` clauses** If your users are constantly searching for movies by their title, or users by their email address, those columns (`title`, `email`) are prime candidates for an index.

**2. Columns used in `JOIN` conditions (Foreign Keys)** In the CS50 IMDb database, the `stars` table links people to movies using `person_id` and `movie_id`. Because you will frequently `JOIN` tables using these IDs, indexing foreign keys is highly recommended. _(Note: You do not need to manually index Primary Keys like `id`. SQLite and most databases do this automatically)._

**3. Columns used in `ORDER BY` or `GROUP BY`** Because a B-Tree index is naturally sorted, indexing a column that you frequently sort by can save the database from having to perform heavy, memory-intensive sorting operations on the fly.

**4. Multi-Column searches (Covering Indexes)** As highlighted in the CS50 lecture, sometimes one index isn't enough. If your query looks up a `person_id` but _also_ needs to retrieve the `movie_id`, you can index both at the exact same time:

```sql
CREATE INDEX person_index ON stars (person_id, movie_id);
```

This is called a Covering Index. The database finds everything it needs inside the index itself without ever having to take the extra step of looking at the original table!

#### Why Not Index Every Column? (The Trade-Offs)

If indexes make queries lightning fast, why don't we just index every single column in the database? The CS50 notes highlight two massive trade-offs:

- The Space Trade-off: Indexes are literal copies of your data. If you index every column, your database file size will explode. (This is why the CS50 notes mention the VACUUM command—to clean up database space after you drop old indexes).

- The Time Trade-off (Writes): Indexes make reading (SELECT) incredibly fast, but they make writing (INSERT, UPDATE, DELETE) slower. Every time you add a new movie to the database, you aren't just adding a row to a table; the database has to re-calculate and re-balance the B-Tree for every single index attached to that table.

##### Advanced Tip: Partial Indexes

If you have a massive database, you can save space by creating a Partial Index. For example, if you know your users usually only search for newly released movies, you can index only those rows:

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