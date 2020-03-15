import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from flask import Flask, jsonify, render_template
import sqlite3 as sql
import numpy as np
import datetime
import os

app = Flask(__name__)

def create_connection(db_file):
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


@app.route('/orange/api/v1.0/data', methods=['GET'])
def get_data():
    conn = None
    try:
        conn = sql.connect("moisture.db")
    except sql.Error as e:
        print(e)
 

    cur = conn.cursor()
    cur.execute("SELECT * FROM moisture ORDER BY id LIMIT 500")

    data = cur.fetchall()
    return jsonify({'data': data})

@app.route('/plot')
def chartTest():

    conn = None
    try:
        conn = sql.connect("/home/callum/Dev/plantproj/moisture.db")
    except sql.Error as e:
        print(e)
    plt.clf()
    cur = conn.cursor()
    cur.execute("SELECT * FROM moisture ORDER BY id DESC LIMIT 500")

    data = cur.fetchall()
    dates = []
    levels = []
    #print(data)

    for idx, date, moisture in data:
        date = datetime.datetime.strptime(date , "%Y-%m-%d %H:%M:%S.%f")
        dates.append(date)
        levels.append(moisture)
    
    conn.close()
    plt.plot_date(dates, levels)
       
    plt.savefig('/home/callum/Dev/plantproj/static/images/new_plot.jpg')
    return render_template('plot.html', name = 'new_plot', url ='static/images/new_plot.jpg')




if __name__ == '__main__':
    print(os.getcwd())
    app.run("0.0.0.0" , 8080)



	
