import mysql.connector
from PyQt5.QtCore import QThread

class DBConnector(QThread):

    def __init__(self):
        self.host = "localhost"
        self.port = "3306"
        self.user = "segway"
        self.password = "1"
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

    def insert_to_mpu6050(self, time_stamp, acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z):
        if not self.connection:
            print("Not connected to the database. Call connect() method first.")
            return

        cursor = self.connection.cursor()

        try:
            insert_query = "INSERT INTO mpu6050 (time_stamp, acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z) VALUES (%s, %s, %s, %s, %s, %s, %s)"
            data = (time_stamp, acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z)
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

    def insert_to_rfid_user(self, uid, name, sign_in_date):
        if not self.connection:
            print("Not connected to the database. Call connect() method first.")
            return

        cursor = self.connection.cursor()

        try:
            insert_query = "INSERT INTO user (uid, name, sign_in_date) VALUES (%s, %s, %s)"
            data = (uid, name, sign_in_date)
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

    def fetch_uid_from_user(self, uid):
        if not self.connection:
            print("Not connected to the database. Call connect() method first.")
            return
        
        cursor = self.connection.cursor()

        try:
            select_query = "SELECT name FROM user WHERE uid LIKE %s"
            data = (f'%{uid}%',)
            cursor.execute(select_query, data)
            result = cursor.fetchall()
            return result
        except mysql.connector.Error as err:
            print(f"Error fetching user data: {err}")
        finally:
            cursor.close()



if __name__ == "__main__":
    db = DBConnector()
    db.connect()


    ####################################
    # CREATE TABLES FOR SEGWAY PROJECT #
    ####################################
    # # Specify the path to your SQL file
    # sql_file_path = "segwayDB_create_table.sql"

    # # Execute the SQL script from the file to create tables
    # db.execute_sql_file(sql_file_path)


    ####################################
    #   INSERT VALUES INTO DATABASET   #
    ####################################
    # Insert data into MPU6050 table
    # db.insert_to_mpu6050("2023-10-28 23:53:39", 1, 2, 3, 4, 5, 6)
    # db.insert_to_mpu6050("2023-10-29 23:53:39", 7, 8, 9, 10, 11, 12)

    # # Insert data into Motor table
    # db.insert_to_motor("2023-10-28 23:53:39", "w")
    # db.insert_to_motor("2023-10-28 23:53:42", "d")

    # Insert data into RFID user table
    # 63:61:26:EC (card)
    db.insert_to_rfid_user("63:61:26:EC", "Hongki Cho", "2023-10-29")
    # E3:73:C1:0D (tag)
    db.insert_to_rfid_user("E3:73:C1:0D", "Seoyoung Moon", "2023-10-29")

    # # Insert data into RFID tag log table
    # db.insert_to_rfid_tag_log("2023-10-28 23:53:39", "63:61:26:EC")



    ####################################
    #    FETCH DATA FROM DATABASE      #
    ####################################
    # select_query = "SELECT * FROM rfid_user"
    # results = db.queryResults(select_query)
    # for row in results:
    #     print(row)


    db.disconnect()