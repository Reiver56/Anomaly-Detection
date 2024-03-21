#pragma once // #include <include/database-struct.hpp>

#include <pqxx/pqxx>
#include <iostream>
#include <stdlib.h>
#include "database-classes.hpp"
#include <vector>
#include <cstdlib> // per getenv
#include <iostream>
#include <string>
#include <fstream>

inline int getMaxTempEndAverages(pqxx::connection& C){
    pqxx::work W(C);
    pqxx::result R = W.exec("SELECT MAX(temp_end) FROM averages;");
    int max = R[0][0].as<int>();
    W.commit();
    return max;
}

inline int getMaxTempEndCovariances(pqxx::connection& C){
    pqxx::work W(C);
    pqxx::result R = W.exec("SELECT MAX(temp_end) FROM covariances;");
    int max = R[0][0].as<int>();
    W.commit();
    return max;
}

// metodo per la creazione del database anomaly_detection
inline bool databaseExists(pqxx::connection& C){
    std::string sql;
    pqxx::work W(C);
    sql = "SELECT 1 FROM pg_database WHERE datname = 'anomaly_detection';";
    pqxx::result R = W.exec(sql);
    W.commit();
    if (R.size() == 0){
        return false;
    }
    return true;
}

// metodo per la creazione del database anomaly_detection
inline void createDatabase(pqxx::connection& C){
    pqxx::nontransaction N(C);
    std::cout << "Creazione del database in corso..." << std::endl;
    std::string sql = "CREATE DATABASE anomaly_detection;";
    N.exec(sql);
    N.commit();
    std::cout << "Database creato con successo" << std::endl;
}


inline void truncateAverages(pqxx::connection& C){
    pqxx::work W(C);
    std::string sql = "TRUNCATE TABLE averages;";
    W.exec(sql);
    W.commit();
}

inline void insertAverage(Average average,pqxx::connection& C){

    pqxx::work W(C);
    std::string sql = "INSERT INTO averages (temp_start, temp_end, sensor, stream, media, null_window) VALUES ($1, $2, $3, $4, $5, $6);";
    //std::cout << average.getTempStart() << " " << average.getTempEnd() << " " << average.getSensor() << " " << average.getStream() << " " << average.getMedia() << " " << average.getNullWindow() << std::endl;
    W.exec_params(sql, average.getTempStart(),average.getTempEnd(), average.getSensor() ,average.getStream(),average.getMedia(),average.getNullWindow());
    W.commit();
}

inline void insertCovariance(Covariance covariance, pqxx::connection& C){

    pqxx::work W(C);
    std::string sql = "INSERT INTO covariances (temp_start, temp_end, sensor1, sensor2, covariance, null_windows) VALUES ($1, $2, $3, $4, $5, $6);";
    W.exec_params(sql, covariance.getTempStart(),covariance.getTempEnd(), covariance.getSensor1() ,covariance.getSensor2(),covariance.getCovariance(),covariance.getNullWindow());
    W.commit();

}

inline void insertAnomalyAverage(Average_anomaly anomaly, pqxx::connection& C){
    pqxx::work W(C);
    std::string sql = "INSERT INTO anomalies_averages (temp_start, temp_end, sensor) VALUES ($1, $2, $3);";
    W.exec_params(sql, anomaly.getTempStart(),anomaly.getTempEnd(), anomaly.getSensor());
    W.commit();

    int gratestTime = getMaxTempEndAverages(C);

    std::cout << "interlapsed time : " << gratestTime - anomaly.getTempEnd() << std::endl;
}

inline void insertAnomalyCovariance(Covariance_anomaly covariance, pqxx::connection& C){
    pqxx::work W(C);
    std::string sql = "INSERT INTO anomalies_covariances (temp_start, temp_end, sensor1, sensor2) VALUES ($1, $2, $3, $4);";
    W.exec_params(sql, covariance.getTempStart(),covariance.getTempEnd(), covariance.getSensor1(),covariance.getSensor2());
    W.commit();

    int greatestTime = getMaxTempEndCovariances(C);

    std::cout << "interlapsed time : " << greatestTime - covariance.getTempEnd() << std::endl;

}

inline void selectAverages(int temp_start,int temp_end, pqxx::connection& C,std::vector<double>& averanges){

    pqxx::work W(C);
        // voglio fare query della tabella averages con tempS che corrisponde a times[0] e tempE a times[1] 
        pqxx::result R = W.exec("SELECT media FROM averages WHERE null_window = false AND temp_start = "+ std::to_string(temp_start) + " AND temp_end = "+std::to_string(temp_end)+";");

        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            averanges.push_back(c[0].as<double>());
        }
        W.commit();
}

inline void selectValidAverages(pqxx::connection& C,std::vector<double>& averages){
    // prendo le medie dal database e le metto in un vettore
        pqxx::work W(C);
        // voglio prendere solo le medie che hanno false nel campo null_windows
        pqxx::result R = W.exec("SELECT media FROM averages WHERE null_window = false");
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            averages.push_back(c[0].as<double>());
        }
        W.commit();

}

inline void selectValidCovariances(pqxx::connection& C,std::vector<double>& covariances){
    // prendo le medie dal database e le metto in un vettore
        pqxx::work W(C);
        // voglio prendere solo le medie che hanno false nel campo null_windows
        pqxx::result R = W.exec("SELECT covariance FROM covariances WHERE null_windows = false");
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            covariances.push_back(c[0].as<double>());
        }
        W.commit();

}

inline void selectSensorFromAverages(pqxx::connection& C,std::vector<int>& sensors){
    pqxx::work W(C);
    pqxx::result R = W.exec("SELECT sensor FROM averages WHERE null_window = false");
    for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
        sensors.push_back(c[0].as<int>());
    }
    W.commit();
}

inline void selectSensorsFromCovariances(pqxx::connection& C,std::vector<std::vector<int>>& sensors){
    pqxx::work W(C);
    pqxx::result R = W.exec("SELECT sensor1, sensor2 FROM covariances WHERE null_windows = false");
    for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
        sensors.push_back({c[0].as<int>(),c[1].as<int>()});
    }
    W.commit();
}

inline void getTempsAverages2(pqxx::connection& C , std::vector<std::vector<int>>& temps){
     pqxx::work W(C);
        pqxx::result R = W.exec("SELECT temp_start, temp_end FROM averages WHERE null_window = false");
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            temps.push_back({c[0].as<int>(),c[1].as<int>()});
        }
        W.commit();
}

inline void getTempsAverages(pqxx::connection& C,std::vector<std::vector<int>>& temps){
     pqxx::work W(C);
        pqxx::result R = W.exec("SELECT temps, tempe FROM averanges WHERE null_window = false");
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            temps.push_back({c[0].as<int>(),c[1].as<int>()});
        }
        W.commit();
}

inline void truncateCovariances(pqxx::connection& C){
     pqxx::work W(C);
        W.exec("TRUNCATE covariances;");
        W.commit();
}


/*
std::vector<StreamCouple> selectStreamCouples(pqxx::connection& C){
   
}
*/
    

inline void createTables(pqxx::connection& C){
    pqxx::work W2(C);
    std::string sql2 = "CREATE TABLE IF NOT EXISTS averages ("
                      "id SERIAL PRIMARY KEY     NOT NULL,"
                      "temp_start INT     NOT NULL,"
                      "temp_end INT     NOT NULL,"
                      "sensor INT     NOT NULL,"
                      "stream VARCHAR     NOT NULL,"
                      "media DOUBLE PRECISION NOT NULL,"
                      "null_window BOOLEAN NOT NULL);";
    W2.exec(sql2);
    W2.commit();
    pqxx::work W4(C);
    std::string sql4 = "CREATE TABLE IF NOT EXISTS covariances ("
                      "id SERIAL PRIMARY KEY     NOT NULL,"
                      "temp_start INT     NOT NULL,"
                      "temp_end INT     NOT NULL,"
                      "sensor1 INT     NOT NULL,"
                      "sensor2 INT     NOT NULL,"
                      "covariance DOUBLE PRECISION NOT NULL,"
                      "null_windows BOOLEAN NOT NULL);";
    W4.exec(sql4);
    W4.commit();
    std::cout << "Tabella covariances creata con successo" << std::endl;

    pqxx::work W5(C);
    std::string sql5 = "CREATE TABLE IF NOT EXISTS stream_couple("
                      "id SERIAL PRIMARY KEY     NOT NULL,"
                      "stream0 VARCHAR     NOT NULL,"
                      "stream1 VARCHAR     NOT NULL);";
    W5.exec(sql5);
    W5.commit();
    std::cout << "Tabella stream_couple creata con successo" << std::endl;
    // creo una tabella per le anomalie delle medie con id(serial), temp_start(int), temp_end(int), sensor(int)
    pqxx::work W6(C);
    std::string sql6 =  "CREATE TABLE IF NOT EXISTS anomalies_averages ("
                       "id SERIAL PRIMARY KEY     NOT NULL,"
                       "temp_start INT     NOT NULL,"
                       "temp_end INT     NOT NULL,"
                       "sensor INT     NOT NULL);";
    W6.exec(sql6);
    W6.commit();
    std::cout << "Tabella anomalies_averages creata con successo" << std::endl;
    // creo una tabella per le anomalie delle covariance con id(serial), temp_start(int), temp_end(int), sensor1(int), sensor2(int)
    pqxx::work W7(C);
    std::string sql7 =  "CREATE TABLE IF NOT EXISTS anomalies_covariances ("
                       "id SERIAL PRIMARY KEY     NOT NULL,"
                       "temp_start INT     NOT NULL,"
                       "temp_end INT     NOT NULL,"
                       "sensor1 INT     NOT NULL,"
                       "sensor2 INT     NOT NULL);";
    W7.exec(sql7);
    W7.commit();
    std::cout << "Tabella anomalies_covariances creata con successo" << std::endl;
}

inline int getLengthRilevazioni(pqxx::connection& C){
    pqxx::work W(C);
    std::string sql = "SELECT COUNT(*) FROM rilevazioni;";
    pqxx::result R = W.exec(sql);
    int length = R[0][0].as<int>();
    W.commit();
    return length;
}







