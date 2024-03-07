import sqlite3

def extract_admin_credentials(database_path):
    conn = sqlite3.connect(database_path)
    cursor = conn.cursor()

    query = "SELECT username, password FROM credentials WHERE username = 'hotdogstand'"

    cursor.execute(query)

    result = cursor.fetchone()

    conn.close()

    if result:
        username, password = result
        print(f"Admin Username: {username}")
        print(f"Admin Password: {password}")
    else:
        print("Admin user 'hotdogstand' not found!")

if __name__ == "__main__":
    database_path = "src/hotdog-database/robot_data.db"
    extract_admin_credentials(database_path)
