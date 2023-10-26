import mysql.connector

def aws_to_df():
    conn = mysql.connector.connect(
        host = "localhost",
        port = 3306,
        user = "segway",
        password = "1",
        database = "proj_iot"
    )

    cur = conn.cursor(buffered=True)
    
    return cur