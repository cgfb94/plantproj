import socket
import threading 
import datetime
import os

import sqlite3 as sql

HOST_SOC = "0.0.0.0"
SOC_PORT = 8090

def add_data(conn, data):
    """
    Create a new project into the projects table
    :param conn:
    :param project:
    :return: project id
    """
    sql = ''' INSERT INTO moisture(time,moisture)
              VALUES(?,?) '''
    cur = conn.cursor()
    cur.execute(sql, data)
    return cur.lastrowid

def socket_server(db_connection):

    sep = '\n'

    while(True):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((HOST_SOC, SOC_PORT))
            s.listen()
            conn, addr = s.accept()
            with conn:
                print('Connected by', addr)
                while True:
                    ts = datetime.datetime.now() 
                    ts = str(ts)
                    data = conn.recv(8)
                    num = int (data.decode("utf-8"))
                    print(num)
                    if ( num < 900 ):
                        add_data(db_connection, (ts, num))
                        db_connection.commit()

                    if not data:
                        s.close()
                        s.bind((HOST_SOC, SOC_PORT))
                        s.listen()
                        conn, addr = s.accept()



def database_init(db_file):
    """ create a database connection to the SQLite database
        specified by the db_file
    :param db_file: database file
    :return: Connection object or None
    """
    conn = None
    try:
        conn = sql.connect(db_file)
    except sql.Error as e:
        print(e)

 
    return conn

def create_table(conn, create_table_sql):
    """ create a table from the create_table_sql statement
    :param conn: Connection object
    :param create_table_sql: a CREATE TABLE statement
    :return:
    """
    try:
        c = conn.cursor()
        c.execute(create_table_sql)
    except Error as e:
        print(e)

if __name__ == '__main__':
    print(os.getcwd())
    conn = database_init("/home/callum/Dev/plantproj/moisture.db")

    sql_create_moisture_table = """ CREATE TABLE IF NOT EXISTS moisture (
                                        id integer PRIMARY KEY AUTOINCREMENT,
                                        time text,
                                        moisture int
                                     
                                    ); """

    with conn:
        create_table(conn, sql_create_moisture_table)
        socket_server(conn)
