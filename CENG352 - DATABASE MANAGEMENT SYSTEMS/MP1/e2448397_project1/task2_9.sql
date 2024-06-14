-- First, I created the new user with the following command:

    CREATE USER newuser WITH PASSWORD 'newpassword';

-- Then, I connected to the database with the new user:

-- Then, I run the following query to find all cancelled orders:
    
    SELECT *
    FROM orders O
    WHERE O.status = 'CANCELLED';

-- After the query was executed, I got the following error:

    SQL Error [42501]: ERROR: permission denied for table orders

-- Then, I run the following command to give permission to query orders table to the newuser:
    
    GRANT SELECT ON orders TO newuser;

-- Then, I run the following query again:

    SELECT *
    FROM orders O
    WHERE O.status = 'CANCELLED';

-- Finally, query was successfully executed and I got the result.
