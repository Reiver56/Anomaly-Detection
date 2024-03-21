# Anomaly-Detection

Progetto per il corso di Ingegneria del Software, del corso di Laurea in Informatica della Sapienza(a.a. 2023/24).

# Come configurare tutto prima di avviare il programma:

***
  * Modificare i permessi di accesso hai file .sh presenti nella directory principale:

  ```
  $ chmod +x run_services.sh
  ```

  ```
  $ chmod +x setup.sh
  ```

  ```
  $ chmod +x start_server.sh
  ```

  ```
  $ chmod +x stop_services.sh
  ```


  * Avviare nella directory principale setup.sh, che permette l'installazione delle due librerie fondamentali del programma hiredis e pqxx (postgres) per C++ (richiede che postgres e redis siano installati per funzionare correttamente).

  ```
  $ ./setup.sh
  ```
* Entrare nel file DB_credentials.csv e modificare i dati di accesso di sql, mettendo i propri:
  ```
  DB_USER=inserireilpropriousername
  DB_PASSWORD=inserirelapropriapassword
  ```
* Una volta seguiti questi passaggi entrando in ./Test-Generator/src/main.cpp, si potranno aggiungere le coppie di sensori che si desiderano nel vettore targetSensors e modificare la variabile tempWindow (finestra temporale configurabile).


Una volta seguiti questi passaggi si può passare all'esecuzione.
  
***

# Come avviare il programma:

***
* Eseguire prima il seguente comando:
  
  ```
  $ make server
  ```
  
  Questo comando permette di inizializzare il server di redis e postgres. Inoltre, una volta connesso al database crea le tabelle necessarie per il programma.
  
* Succesivamente eseguire:
  
  ```
  $ make run
  ```
  Per avviare il programma. Questo farà apparire a schermo la seguente scritta: "verifico anomalie attendo se vuoi chiudere e vedere le anomalie(10 sec)" , con le relative anomalie se ci sono.
  
* Per chiudere i server di postgres e redis:
  
  ```
  $ make stop 
  ```
  
  Questo permetterà di chiudere tutti i server sia per redis e postgres

* Per stoppare l'esecuzione del programma fare ctrl+z

* Una volta fermata l'esecuzione del programma, si possono produrre i due scatter plot relativi a covarianze e medie

  ```
  $ python3 scatter-plots.py
  ```
* E con l'altro script python verranno prodotti 4 file csv, che contengono tutti i dati delle anomalie, medie e covarianze

  ```
  $ python3 make_csv.py
  ```


***


