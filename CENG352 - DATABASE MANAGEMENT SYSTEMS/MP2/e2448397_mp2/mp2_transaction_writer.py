import psycopg2
from config import read_config

def insert_plan(isolation_level, plan):
    conn = psycopg2.connect(**read_config(filename="database.cfg", section="postgresql"))
    cursor = conn.cursor()
    cursor.execute(f"SET TRANSACTION ISOLATION LEVEL {isolation_level};")
    try:
        cursor.execute("""
            INSERT INTO plans(plan_id, name, max_parallel_sessions)
            VALUES(%s, %s, %s);
        """, plan)
        print(f"Inserted {plan[1]} but not committed yet ({isolation_level}). Hit enter to commit...")
        input()
        
        conn.commit()
        print(f"Transaction committed ({isolation_level}). Hit enter to finish...")
        input()
    except Exception as error:
        print(f"Error: {error}")
        conn.rollback()
    finally:
        cursor.close()
        conn.close()
        print("Database connection closed")

if __name__ == "__main__":
    experiments = [
        ("READ COMMITTED", (4, "new_plan_read_committed", 8)),
        ("REPEATABLE READ", (5, "new_plan_repeatable_read", 10)),
        ("SERIALIZABLE", (6, "new_plan_serializable", 12))
    ]
    
    for isolation_level, plan in experiments:
        print(f"\nRunning experiment with isolation level: {isolation_level}")
        insert_plan(isolation_level, plan)
        print("Experiment completed. Hit enter to proceed to the next isolation level...")
        input()
