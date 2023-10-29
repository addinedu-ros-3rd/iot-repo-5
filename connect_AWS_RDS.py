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

    def insert_to_mpu6050(self, time_stamp, gyro_yaw, gyro_pitch, gyro_roll):
        if not self.connection:
            print("Not connected to the database. Call connect() method first.")
            return

        cursor = self.connection.cursor()

        try:
            insert_query = "INSERT INTO mpu6050 (time_stamp, gyro_yaw, gyro_pitch, gyro_roll) VALUES (%s, %s, %s, %s)"
            data = (time_stamp, gyro_yaw, gyro_pitch, gyro_roll)
            cursor.execute(insert_query, data)
            self.connection.commit()
            print("MPU6050 data inserted successfully")
        except mysql.connector.Error as err:
            print(f"Error inserting MPU6050 data: {err}")
        finally:
            cursor.close()

    def insert_to_motor(self, time_stamp, direction):
        if not self.connection:
            print("Not connected to the database. Call connect() method first.")
            return

        cursor = self.connection.cursor()

        try:
            insert_query = "INSERT INTO motor (time_stamp, direction) VALUES (%s, %s)"
            data = (time_stamp, direction)
            cursor.execute(insert_query, data)
            self.connection.commit()
            print("Motor data inserted successfully")
        except mysql.connector.Error as err:
            print(f"Error inserting motor data: {err}")
        finally:
            cursor.close()

    def insert_to_rfid_user(self, uid, user_name):
        if not self.connection:
            print("Not connected to the database. Call connect() method first.")
            return

        cursor = self.connection.cursor()

        try:
            insert_query = "INSERT INTO rfid_user (uid, user_name) VALUES (%s, %s)"
            data = (uid, user_name)
            cursor.execute(insert_query, data)
            self.connection.commit()
            print("RFID user data inserted successfully")
        except mysql.connector.Error as err:
            print(f"Error inserting RFID user data: {err}")
        finally:
            cursor.close()

    def insert_to_rfid_tag_log(self, time_stamp, uid):
        if not self.connection:
            print("Not connected to the database. Call connect() method first.")
            return

        cursor = self.connection.cursor()

        try:
            insert_query = "INSERT INTO rfid_tag_log (time_stamp, uid) VALUES (%s, %s)"
            data = (time_stamp, uid)
            cursor.execute(insert_query, data)
            self.connection.commit()
            print("RFID tag log data inserted successfully")
        except mysql.connector.Error as err:
            print(f"Error inserting RFID tag log data: {err}")
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
    #   INSERT VALUES INTO DATABASET   #
    ####################################
    # Insert data into MPU6050 table
    db.insert_to_mpu6050("2023-10-28 23:53:39", 1.00012, 230.00034, 34560.00056)

    # Insert data into Motor table
    db.insert_to_motor("023-10-28 23:53:39", "w")

    # Insert data into RFID user table
    db.insert_to_rfid_user("48656C6C6F20576F726C64;", "rae moon")

    # Insert data into RFID tag log table
    db.insert_to_rfid_tag_log("23-10-28 23:53:39", "48656C6C6F20576F726C64;")


    # # Insert data into MPU6050 table
    # db.insert_to_mpu6050("timestamp_value", gyro_yaw_value, gyro_pitch_value, gyro_roll_value)

    # # Insert data into Motor table
    # db.insert_to_motor("timestamp_value", "direction_value")

    # # Insert data into RFID user table
    # db.insert_to_rfid_user("uid_value", "user_name_value")

    # # Insert data into RFID tag log table
    # db.insert_to_rfid_tag_log("timestamp_value", "uid_value")



    ####################################
    #   INSERT VALUES INTO DATABASET   #
    ####################################
    # query = "INSERT INTO rfid_user (id, uid, user_name) VALUES (%s, %s, %s)"
    # data = (1, "abcde12345fghij67890", "rae moon")
    # db.queryExecute(query, data)


    ####################################
    #    FETCH DATA FROM DATABASE      #
    ####################################
    # select_query = "SELECT * FROM rfid_user"
    # results = db.queryResults(select_query)
    # for row in results:
    #     print(row)


    db.disconnect()
