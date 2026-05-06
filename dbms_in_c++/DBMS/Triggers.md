**SQLite Triggers: NEW vs. OLD**

In SQLite, `NEW` and `OLD` are temporary table aliases used to access row data during an action.

- **`NEW`**:  Represents the data **coming in** (the data being inserted or the "new" version of an update).(used in `INSERT` and `UPDATE`).

- **`OLD`**: Represents the data **already there** (the data being deleted or the "old" version before an update). (used in `UPDATE` and `DELETE`).

---

**1. INSERT Trigger (Fixing Data)**

Used to auto-format or round data immediately after it is added.  
_Uses **NEW** only._

```sql
CREATE TRIGGER auto_round_insert AFTER INSERT ON Products
BEGIN
    UPDATE Products 
    SET Price = ROUND(NEW.Price, 2) 
    WHERE rowid = NEW.rowid;
END;
```


**2. UPDATE Trigger (Comparing Values)**

Used to compare the "before" and "after" values.  
_Uses **NEW** and **OLD**._


```sql
CREATE TRIGGER track_price_change AFTER UPDATE OF Price ON Products
WHEN NEW.Price != OLD.Price  -- Only run if the price actually changed
BEGIN
    INSERT INTO Price_History(old_price, new_price) 
    VALUES (OLD.Price, NEW.Price);
END;
```


**3. DELETE Trigger (Cleanup)**

Used to clean up related data before a row is gone.  
Uses **OLD** only.

```sql
CREATE TRIGGER clean_up_files BEFORE DELETE ON Users
BEGIN
    DELETE FROM User_Settings WHERE user_id = OLD.id;
END;
```



> [!NOTE] Caution
> trigger not help always . if you are applying trigger in a situation where massive / huge number of rows are imported then performance drops
