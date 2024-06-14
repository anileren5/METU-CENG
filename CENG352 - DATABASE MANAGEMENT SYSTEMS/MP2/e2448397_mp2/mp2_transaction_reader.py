import psycopg2
from config import read_config

query = """
SELECT *
FROM plans;
"""

def print_plans(plans):
    print("#|Name|Max Sessions")
    for plan in plans:
        print(f"{plan[0]}|{plan[1]}|{plan[2]}")

def run_experiment(isolation_level):
    conn = psycopg2.connect(**read_config(filename="database.cfg", section="postgresql"))
    cursor = conn.cursor()
    cursor.execute(f"SET TRANSACTION ISOLATION LEVEL {isolation_level};")
    try:
        cursor.execute(query)
        plans_before = cursor.fetchall()
        print(f"\nPlans before writer commits ({isolation_level}):\n")
        print_plans(plans_before)
        print("\nHit enter after writer commits to read plans again...")
        input()
        
        cursor.execute(query)
        plans_after = cursor.fetchall()
        print(f"Plans after writer commits ({isolation_level}):\n")
        print_plans(plans_after)
    except Exception as error:
        print(f"Error: {error}")
    finally:
        cursor.close()
        conn.close()
        print("\nDatabase connection closed")

if __name__ == "__main__":
    for level in ["READ COMMITTED", "REPEATABLE READ", "SERIALIZABLE"]:
        print(f"\n---------- Running experiment with isolation level: {level} ----------")
        run_experiment(level)
        print("\nExperiment completed. Hit enter to proceed to the next isolation level...")
        input()
