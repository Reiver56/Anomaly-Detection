import matplotlib.pyplot as pyplot
# voglio prendere le medie e le covarianze dal database postgresql
import psycopg2
import numpy as np
import pandas as pd

# Create a scatter plot 

# Create a scatter plot
def scatter_plot_average():
    # Connect to the database
    conn = psycopg2.connect(
        host="localhost",
        database="anomaly_detection",
        user="postgres",
        password=""
    )
    # Create a cursor object
    cur = conn.cursor()
    #nella tabella averages ci sono anche i tempi e i sensori che voglio integrare nel grafico
    # A query to get the average of the values
    
    cur.execute("SELECT temp_end, sensor, media FROM averages")
    # Fetch the data
    rows = cur.fetchall()
    # Create a dataframe
    df = pd.DataFrame(rows, columns=['temp_end', 'sensor', 'media'])
    # Create a scatter plot
    fig, ax = pyplot.subplots()
    # Create a scatter plot for each sensor
    for sensor in df['sensor'].unique():
        # Get the data for the sensor
        data = df[df['sensor'] == sensor]
        # Create a scatter plot
        ax.scatter(data['temp_end'], data['media'], label=sensor)
    # Set the title
    ax.set_title('Average values for each sensor')
    # Set the x-axis label
    ax.set_xlabel('Time')
    # Set the y-axis label
    ax.set_ylabel('Average value')
    # Set the legend
    ax.legend()
    # Show the plot
    pyplot.show()
    # Close the cursor
    cur.close()
    # Close the connection
    conn.close()

# Create a scatter plot for covariances
def scatter_plot_covariance():
    # Connect to the database
    conn = psycopg2.connect(
        host="localhost",
        database="anomaly_detection",
        user="postgres",
        password=""
    )
    # Create a cursor object
    cur = conn.cursor()
    # A query to get the covariance of the values
    cur.execute("SELECT temp_end, sensor1, sensor2, covariance FROM covariances")
    # i due sensori relativi alla covarianza devono essere presi insieme nella leggenda del grafico
    
    # Fetch the data
    rows = cur.fetchall()
    # Create a dataframe
    df = pd.DataFrame(rows, columns=['temp_end', 'sensor1', 'sensor2', 'covariance'])
    # Create a scatter plot
    fig, ax = pyplot.subplots()
    # Create a scatter plot for each sensors, considerando nella leggenda i la coppia di sensori relativi alla covarianza
    
    
    for sensor1, sensor2 in df[['sensor1', 'sensor2']].drop_duplicates().values:
        # Get the data for the sensor
        data = df[(df['sensor1'] == sensor1) & (df['sensor2'] == sensor2)]
        # Create a scatter plot
        ax.scatter(data['temp_end'], data['covariance'], label=(sensor1, sensor2))
    # Set the title
    ax.set_title('Covariance values for each sensor')
    # Set the x-axis label
    ax.set_xlabel('Time')
    # Set the y-axis label
    ax.set_ylabel('Covariance value')
    # Set the legend
    ax.legend()
    # Show the plot
    pyplot.show()
    # Close the cursor
    cur.close()
    # Close the connection
    conn.close()

     
    








# Call the function
scatter_plot_average()
scatter_plot_covariance()

