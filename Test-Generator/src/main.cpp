#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // per atoi
#include <unistd.h> // permette di mettere in sleep il programma
#include <hiredis/hiredis.h>
#include "read_csv.cpp"
#include "set-stream.cpp"

using namespace std;

const string filename = "../../sensorData.csv";

int main() {
    
    int tempStart = 0; // estremo iniziale della finestra temporale
    long unsigned int tempWindow = 100; // questo valore può essere configurato e corrisponde alla grandezza della finestra
    tempWindow = tempWindow - 1;

    // mettere i sensori target all'interno del vettore
    // dovranno essere messi a due  a due, questo perché per il calcolo delle covarianze
    // questo prenderà targetSensors[i] e targetSensors[i+1] e calcolerà la covarianza tra i due
    vector<string> targetSensors = {"579", "558", "265", "2", "298", "19", "798", "787", "1", "627"};
    
    
    int va = system("redis-cli FLUSHDB");
    // connessione a redis
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err) {
        if (c) {
            cout << "Errore di connessione: " << c->errstr << endl;
        } else {
            cout << "Can't allocate redis context" << endl;
        }
        exit(1);
    }
    isReady(c,"no");
    isReady2(c, "no");    

    //---------------------------------------------
    //stream redis con la grandezza della finestra
    string streamKey = "stream_window";
    string command = "XADD " + streamKey + " * window " + to_string(tempWindow);
    redisReply *reply = (redisReply *)redisCommand(c, command.c_str());
    if (reply == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    
    freeReplyObject(reply);
    streamKey = "sensor_names";
    // prendo i target sensor e li metto in redis
    for (long unsigned int i = 0; i < targetSensors.size(); i++) {
        string command = "XADD " + streamKey + " * sensorID " + targetSensors[i];
        redisReply *reply = (redisReply *)redisCommand(c, command.c_str());
        if (reply == NULL) {
            cout << "Errore nell'esecuzione del comando" << endl;
            exit(1);
        }
        freeReplyObject(reply);
    }
    //---------------------------------------------
    vector<vector<string>> data = read_csv(filename);
    vector<vector<string>> dataWindow;
    // inserisco in dataWindow tutte le rilevazioni dei sensori target
    for (long unsigned int i = 0; i < data.size(); i++){
        // verifico che il sensore sia uno dei targetSensor
        if (find(targetSensors.begin(), targetSensors.end(), data[i][1]) != targetSensors.end()){
            dataWindow.push_back(data[i]);
        }
    }

    vector<string> streamNames; // prende il nome delle stream di redis una per ogni sensore
    for (long unsigned int i = 0; i < targetSensors.size(); i++){
        streamNames.push_back("sensor" + targetSensors[i]);
    }


    //inserisco in redis i nomi delle stream-------------------------------------
    for (long unsigned int i = 0; i < streamNames.size(); i++) {
        string streamKey = "stream_names";
        string command = "XADD " + streamKey + " * streamName " + streamNames[i];
        redisReply *reply = (redisReply *)redisCommand(c, command.c_str());
        if (reply == NULL) {
            cout << "Errore nell'esecuzione del comando" << endl;
            exit(1);
        }
        freeReplyObject(reply);
    }
    //---------------------------------------------------------------------------

    
    
    while(tempWindow < dataWindow.size()) {
        
        //cout << tempStart << endl;
        for (int j = 0; j < streamNames.size(); j++){
            setStream(dataWindow, streamNames[j], tempStart, tempWindow, targetSensors[j] ,c);
        }

        // mando in un'altra stream il temp start e il temp end
        string streamKey = "time_window";
        string command = "XADD " + streamKey + " * timestamp " + to_string(tempStart) + " value " + to_string(tempWindow);
        redisReply *reply = (redisReply *)redisCommand(c, command.c_str());
        if (reply == NULL) {
            cout << "Errore nell'esecuzione del comando" << endl;
            exit(1);
        }
        freeReplyObject(reply);
        isReady(c,"yes");
        //isReady(c,"no");
        sleep(1);
        //cout << "svuoto le stream" << endl;
        for (int j = 0; j < streamNames.size(); j++){
            // voglio svuotare queste stream
            string command = "XTRIM " + streamNames[j] + " MAXLEN 0";
            redisReply *reply = (redisReply *)redisCommand(c, command.c_str());
            if (reply == NULL) {
                cout << "Errore nell'esecuzione del comando" << endl;
                exit(1);
            }
            freeReplyObject(reply);
            // svuoto anche la stream time_window
            command = "XTRIM " + streamKey + " MAXLEN 0";
            reply = (redisReply *)redisCommand(c, command.c_str());
            if (reply == NULL) {
                cout << "Errore nell'esecuzione del comando" << endl;
                exit(1);
            }
            freeReplyObject(reply);
        }
        sleep(1);
        //cout << " passo all'altra finestra " << endl;
        tempStart ++; tempWindow ++;    
    }
    return va;
}
