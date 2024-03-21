# voglio fare dei file che contengono i dati delle tabelle di postgresql le tabelle di anomaly_detection: 
# 1. averages
# 2. anomalies_averages
# 3. anomalies_covariances
# 4. covariances

# producendo quindi 4 file csv



import pandas as pd
import psycopg2
from sqlalchemy import create_engine


# connessione al database
conn = psycopg2.connect(
    host="localhost",
    database="anomaly_detection",
    user="postgres",
    password="matteus67"
)

# query per estrarre i dati
query_averages = "SELECT temp_start, temp_end, sensor, media, null_window FROM averages"
query_anomalies_averages = "SELECT temp_start, temp_end, sensor FROM anomalies_averages"
query_anomalies_covariances = "SELECT temp_start, temp_end, sensor1, sensor2 FROM anomalies_covariances"
query_covariances = "SELECT temp_start, temp_end, sensor1, sensor2, covariance, null_windows FROM covariances"

# estrazione dei dati
averages = pd.read_sql_query(query_averages, conn)
anomalies_averages = pd.read_sql_query(query_anomalies_averages, conn)
anomalies_covariances = pd.read_sql_query(query_anomalies_covariances, conn)
covariances = pd.read_sql_query(query_covariances, conn)

# chiusura della connessione
conn.close()

# scrittura dei file csv
averages.to_csv("averages.csv", index=False)
anomalies_averages.to_csv("anomalies_averages.csv", index=False)
anomalies_covariances.to_csv("anomalies_covariances.csv", index=False)
covariances.to_csv("covariances.csv", index=False)

print("Files csv creati con successo")


