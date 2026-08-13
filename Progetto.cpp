#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //per trasformare la riga in un flusso di dati
#include<algorithm> //per la funzione replace
using namespace std;


int main(int argc,char **argv){
    // Verifica che sia stato passato un file come argomento
    if (argc <2){
        cout << "Specifica un file" <<endl;
        return 0;
    }
    cout << "Programma: " << argv[0] << endl;
    cout << "File: " << argv[1] << endl;

    // Apertura del file specificato come argomento
    ifstream file(argv[1]);

    // Gestione di eventuali errori nell'apertura del file
    if (!file.is_open()){
        cout << "Errore nell'apertura del file" << endl;
        return 1;
    }

    string line; // Variabile per memorizzare le righe lette dal file

    // Lettura del file riga per riga (va avanti finche ci sono righe da leggere)
    while (getline(file, line)) { 
        stringstream ss(line); // trasforma la riga in un flusso di dati
        string prima_parola, sequenza_as; // Variabili per memorizzare le prime 2 parole lette dalla riga

        //l'operatore >> salta automaticamente gli spazi bianchi e legge le parole
        if(ss >> prima_parola >> sequenza_as) { // Estrae le prime due parole dal flusso, if serve per verificare che ci siano almeno due parole nella riga
            replace(sequenza_as.begin(), sequenza_as.end(), '|', ' '); // Sostituisce '|' con ' ' nella sequenza

            //creiamo un nuovo flusso per leggere i numeri della sequenza_as modificata (con gli spazi al posto dei '|')
            stringstream ss_numeri(sequenza_as);

            int number; // Variabile per memorizzare i numeri letti dalla riga

            while(ss_numeri >> number) { // Estrae i numeri dal flusso saltando gli spazi bianchi
                cout << number << " "; //per ora lo stampo
            }
            cout << endl; //dopo ogni riga vado a capo
        }
    }
    file.close(); // Chiusura del file dopo la lettura

    return 0;
}
