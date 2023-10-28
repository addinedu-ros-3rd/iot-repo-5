import mysql.connector

class ConnectDB:
    def __init__(self):
        self.host = "****"
        self.port = "3306"
        self.user = "****"
        self.password = "****"
        self.database = "segway"
        self.connection = None

    def connect(self):
        try:
            self.connection = mysql.connector.connect(
                host=self.host,
                port=self.port,
                user=self.user,
                password=self.password,
                database=self.database
            )
            print("=== [ Connected to the database ] ===")

        except mysql.connector.Error as err:
            print(f"Error: {err}")
            self.connection = None

    def disconnect(self):
        if self.connection:
            self.connection.close()
            print("=== [ Disconnected from the database ] ===")

    def execute_sql_file(self, sql_file):
        if not self.connection:
            print("Not connected to the database. Call connect() method first.")
            return

        cursor = self.connection.cursor()

        try:
            with open(sql_file, 'r') as file:
                sql_script = file.read()
                cursor.execute(sql_script, multi=True)

                for result in cursor.stored_results():
                    result.fetchall()

                self.connection.commit()

                print("SQL script execution successful")

        except mysql.connector.Error as err:
            print(f"Error executing SQL script: {err}")

        finally:
            cursor.close()

    def queryExecute(self, query, data=None):
        if not self.connection:
            print("Not connected to the database. Call connect() method first.")
            return

        cursor = self.connection.cursor()
        try:
            if data:
                cursor.execute(query, data)
            else:
                cursor.execute(query)
            self.connection.commit()
            print("Query execution successful")

        except mysql.connector.Error as err:
            print(f"Error executing query: {err}")

        finally:
            cursor.close()

    def queryResults(self, query, data=None):
        if not self.connection:
            print("Not connected to the database. Call connect() method first.")
            return

        cursor = self.connection.cursor()
        try:
            if data:
                cursor.execute(query, data)
            else:
                cursor.execute(query)
            result = cursor.fetchall()
            return result

        except mysql.connector.Error as err:
            print(f"Error executing query: {err}")

        finally:
            cursor.close()


if __name__ == "__main__":
    db = ConnectDB()
    db.connect()


    ####################################
    # CREATE TABLES FOR SEGWAY PROJECT #
    ####################################
    # Specify the path to your SQL file
    # sql_file_path = "segwayDB.sql"

    # # Execute the SQL script from the file to create tables
    # db.execute_sql_file(sql_file_path)


    ####################################
    #    INSERT VALUES TO DATABASE     #
    ####################################
    query = "INSERT INTO rfid_user (id, uid, user_name) VALUES (%s, %s, %s)"
    data = (1, "abcde12345fghij67890", "rae moon")
    db.queryExecute(query, data)


    ####################################
    #    FETCH DATA FROM DATABASE      #
    ####################################
    select_query = "SELECT * FROM rfid_user"
    results = db.queryResults(select_query)
    for row in results:
        print(row)


    db.disconnect()
