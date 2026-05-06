### AFFINITY
it means if a column or field is of Data Type  TEXT and we enters a data of type INT ,then it will convert it to  TEXT (INT --> TEXT ) . 

> [!NOTE] NOTE
> when we doesn't write the data type of a field then sqlite give numeric type of affinity to the that field (column)  


### Column Constraint
to check that 

-  **CHECK** : to check that given value in a column is less then , greater then or equal to a specific value 
- **DEFAULT** : if a user doesn't gives a value then put that filed in the column (like a student forget to fill the school name then by default it puts the school name  )
- **NOT NULL** : user can not insert NULL or empty value to a column 
- **UNIQUE** : every row in a column must have unique value (like :id column )

> [!NOTE]
  when we a column a primary key column , some set of constraints are already applied to that column with primary key constraint. (like if we applied the primary key constraint in "id" then we doesn't have to apply NOT NULL,UNIQUE and other constraints )



### ALTER TABLE  
when we have to update a table (like rename a column , drop a column , add a column )



> [!Note] Need more Efforts to get Perfection
> [[Things to learn ( additionally which is not deeply taught )]]

### [[Triggers]]