#include <iostream>
#include "read_csv.cpp"
#include "set-stream.cpp"
#include <vector>
#include <string>
#include <cstdlib> // per atoi
#include <hiredis/hiredis.h>
#include <chrono>



using namespace std::chrono;
using namespace std;
const string filename = "./test.csv";
const string targetSensor = "SAC7";

int main() {
    
    auto start = high_resolution_clock::now();
    int tempStart = 0;
    int tempWindow = 10; // questo valore può essere configurato e corrisponde alla grandezza della finestra

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
    int va = system("redis-cli FLUSHDB"); // svuto interamente il database di redis per creare un nuovo test

    // mando in una stream di redis i nomi la variabile tempWindow
    string streamKey = "stream_size";
    string command = "XADD " + streamKey + " * tempWindow " + to_string(tempWindow) + " targetSensor " + targetSensor;
    redisReply *reply = (redisReply *)redisCommand(c, command.c_str());
    if (reply == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    freeReplyObject(reply);

    //cout << "Connected to redis" << endl;
    // fine connessione a redis
    
    vector<vector<string>> data = read_csv(filename);
    vector<vector<string>> dataWindow;
    // inserisco in dataWindow i dati con targetSensor == "SAC7"
    for (int i = 0; i < data.size(); i++){
        if (data[i][1] == targetSensor){
            dataWindow.push_back(data[i]);
            
        }
    }
    

    // faccio un array di stringhe dove mettere tutte le stream (cioè i nomi delle stream di redis)
    vector<string> streamNames;

    cout << dataWindow.size() << endl;
    // inserisco in dataWindow le righe che al data[n][0] == tempWindow &&  data[n][1] == targetSensor
    
    while(tempWindow < dataWindow.size()) {
        // controllo i valori della finestra e li inserisco in dataWindow -> per mettere ogni finestra in uno stream di 
        string streamName = "mystream" + to_string(tempStart);
        setStream(dataWindow, streamName, tempStart, tempWindow, c);
        streamNames.push_back(streamName);
        //shift della finestra
        tempStart ++; 
        tempWindow ++;    
        //--------------------   
        }

    // mando a in una stream di redis i nomi degli stream presenti in streamNames 
    for (int i = 0; i < streamNames.size(); i++) {
        string streamKey = "stream_names";
        string command = "XADD " + streamKey + " * streamName " + streamNames[i];
        redisReply *reply = (redisReply *)redisCommand(c, command.c_str());
        if (reply == NULL) {
            cout << "Errore nell'esecuzione del comando" << endl;
            exit(1);
        }
        freeReplyObject(reply);
    }
    
   


    for (int i = 0; i < streamNames.size(); i++) {
        cout << streamNames[i] << endl;
    }

    // chiudo la connessione a redis
    redisFree(c);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Tempo di esecuzione: " << duration.count() << " secondi" << endl;
    return 0;
}
