#pragma once // #include <include/database-struct.hpp>
// modificata streamCouple perche' ci serve per settare la coppia di stream per poi calcolare la covarianza
//N.B. : le stream couple vengono settate all'inizio (in Test-Generator) a random (o almeno questa è l'idea di base) e poi vengono messe nel db
// in modo che poi quando andiamo a scandire questa tabella sappiamo sempre quali sono le coppie di stream che prendiamo in considerazione
class StreamCouple{

    private:
        std::string stream1;
        std::string stream2;
    public:
        StreamCouple(){
            this->stream1 = "";
            this->stream2 = "";
        }

        StreamCouple(std::string stream1, std::string stream2){
            this->stream1 = stream1;
            this->stream2 = stream2;
        }

        ~StreamCouple(){

        }

        std::string getStream1(){
            return this->stream1;
        }

        std::string getStream2(){
            return this->stream2;
        }

        void setStream1(std::string stream1){
            this->stream1 = stream1;
        }

        void setStream2(std::string stream2){
            this->stream2 = stream2;
        }
};

class Average{
    private:
        int temp_start;
        int temp_end;
        int sensore;
        std::string stream;
        double media;
        bool null_window;


    public:

        Average(int temp_start, int temp_end, int sensore, std::string stream, double media, bool null_window){
            this->temp_start = temp_start;
            this->temp_end = temp_end;
            this->sensore = sensore;
            this->stream = stream;
            this->media = media;
            this->null_window = null_window;

        }

        ~Average(){

        }

        int getTempStart(){
            return this->temp_start;
        }

        int getTempEnd(){
            return this->temp_end;
        }

        int getSensor(){
            return this->sensore;
        }

        std::string getStream(){
            return this->stream;
        }

        double getMedia(){
            return this->media;
        }

        bool getNullWindow(){
            return this->null_window;
        }

        void setTempStart(int temp_start){
            this->temp_start = temp_start;
        }

        void setTempEnd(int temp_end){
            this->temp_end = temp_end;
        }

        void setSensor(int sensore){
            this->sensore = sensore;
        }

        void setStream(std::string stream){
            this->stream = stream;
        }

        void setMedia(double media){
            this->media = media;
        }

        void setNullWindow(bool null_window){
            this->null_window = null_window;
        }

        
};

class Covariance{

    private:
        int temp_start;
        int temp_end;
        int sensor1;
        int sensor2;
        double covariance;
        bool null_window;

    public:

        Covariance(int temp_start, int temp_end, int sensor1, int sensor2, double covariance, bool null_window){
            this->temp_start = temp_start;
            this->temp_end = temp_end;
            this->sensor1 = sensor1;
            this->sensor2 = sensor2;
            this->covariance = covariance;
            this->null_window = null_window;
        }

        ~Covariance(){

        }

        int getTempStart(){
            return this->temp_start;
        }

        int getTempEnd(){
            return this->temp_end;
        }

        int getSensor1(){
            return this->sensor1;
        }

        int getSensor2(){
            return this->sensor2;
        }

        double getCovariance(){
            return this->covariance;
        }

        bool getNullWindow(){
            return this->null_window;
        }

        void setTempStart(int temp_start){
            this->temp_start = temp_start;
        }

        void setTempEnd(int temp_end){
            this->temp_end = temp_end;
        }

        void setSensor1(int sensor1){
            this->sensor1 = sensor1;
        }

        void setSensor2(int sensor2){
            this->sensor2 = sensor2;
        }

        void setCovariance(double covariance){
            this->covariance = covariance;
        }

        void setNullWindow(bool null_window){
            this->null_window = null_window;
        }
};

// cosa ci va nelle anomalie?
class Anomaly{

    private:
        int id;
        int timestamp;
        double value;
        std::string sensore;

    public:

        Anomaly(){
            this->id = 0;
            this->timestamp = 0;
            this->value = 0;
            this->sensore = "";
        }

        Anomaly(int id, int timestamp, double value, std::string sensore){
            this->id = id;
            this->timestamp = timestamp;
            this->value = value;
            this->sensore = sensore;
        }

        ~Anomaly(){

        }

        int getId(){
            return this->id;
        }

        int getTimestamp(){
            return this->timestamp;
        }

        double getValue(){
            return this->value;
        }

        std::string getSensor(){
            return this->sensore;
        }

        void setId(int id){
            this->id = id;
        }

        void setTimestamp(int timestamp){
            this->timestamp = timestamp;
        }

        void setValue(double value){
            this->value = value;
        }

        void setSensore(std::string sensore){
            this->sensore = sensore;
        }

};

class Average_anomaly{

    private:
        int temp_start;
        int temp_end;
        int sensor;

    public:

        Average_anomaly(){
            this->temp_start = 0;
            this->temp_end = 0;
            this->sensor = 0;
        }

        Average_anomaly(int temp_start, int temp_end, int sensor){
            this->temp_start = temp_start;
            this->temp_end = temp_end;
            this->sensor = sensor;
        }

        ~Average_anomaly(){

        }

        int getTempStart(){
            return this->temp_start;
        }

        int getTempEnd(){
            return this->temp_end;
        }

        int getSensor(){
            return this->sensor;
        }

        void setTempStart(int temp_start){
            this->temp_start = temp_start;
        }

        void setTempEnd(int temp_end){
            this->temp_end = temp_end;
        }

        void setSensor(int sensor){
            this->sensor = sensor;
        }

};

class Covariance_anomaly{
    
        private:
            int temp_start;
            int temp_end;
            int sensor1;
            int sensor2;
    
        public:
    
            Covariance_anomaly(){
                this->temp_start = 0;
                this->temp_end = 0;
                this->sensor1 = 0;
                this->sensor2 = 0;
            }
    
            Covariance_anomaly(int temp_start, int temp_end, int sensor1, int sensor2){
                this->temp_start = temp_start;
                this->temp_end = temp_end;
                this->sensor1 = sensor1;
                this->sensor2 = sensor2;
            }
    
            ~Covariance_anomaly(){
    
            }
    
            int getTempStart(){
                return this->temp_start;
            }
    
            int getTempEnd(){
                return this->temp_end;
            }
    
            int getSensor1(){
                return this->sensor1;
            }
    
            int getSensor2(){
                return this->sensor2;
            }
    
            void setTempStart(int temp_start){
                this->temp_start = temp_start;
            }
    
            void setTempEnd(int temp_end){
                this->temp_end = temp_end;
            }
    
            void setSensor1(int sensor1){
                this->sensor1 = sensor1;
            }
    
            void setSensor2(int sensor2){
                this->sensor2 = sensor2;
            }

};
