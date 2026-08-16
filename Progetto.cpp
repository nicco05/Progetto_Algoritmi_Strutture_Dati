#include<fstream>
#include<vector>
#include<iostream>
#include<string>
#include<sstream> //per trasformare la riga in un flusso di dati
#include<algorithm> //per la funzione replace



using namespace std;

struct Arco { //struttura per rappresentare un arco
    int destinazione;
    int frequenza;
};

struct Nodo { //struttura per rappresentare un nodo
    int id; // numero identificativo del nodo   
    vector<Arco> adiacenze; //vettore che contiene le adiacenze del nodo con le relative frequenze
};

// Hashing universale
class UniversalHash{
    private:
        long long p,a,b;
    public:
        long long hash (long long x) { return (a*x+b)%p; }

        UniversalHash(long long _p) 
            : p(_p), a(rand() %  (_p-1) +1), b(rand()% _p) {}

        void rehash(){
            a=rand() %  (p-1) +1;
            b=rand()% p;
        }
};

// Hash table per rappresentare il grafo
class GrafoHashTable {
private:
    int m; // dimensione della tabella hash
    vector<vector<Nodo>> table; // vettore di vettori di nodi, rappresenta la tabella hash quindi ogni posizione della tabella può contenere più nodi (gestione delle collisioni)
    UniversalHash hasher; 

    // funzione per calcolare l'hash di un nodo
    int calcolaHash(int x) { 
        int h = hasher.hash(x) % m;
        if (h < 0) h += m;
        return h;
    }

    // funzione per ottenere un nodo dalla tabella hash, se non esiste lo crea
    Nodo& get_nodo(int as_id) { // as_id è l'id del nodo che vogliamo ottenere
        int h = calcolaHash(as_id);
        
        for (auto& nodo : table[h]) { // scorre tutti i nodi nella posizione h della tabella hash
            // se trovo il nodo con l'id as_id lo ritorno
            if (nodo.id == as_id) { 
                return nodo; 
            }
        }
        
        //altrimenti creo un nuovo nodo con l'id as_id e lo aggiungo alla tabella hash
        table[h].push_back({as_id, vector<Arco>()});
        return table[h].back();
    }

public:
    // costruttore della classe GrafoHashTable
    GrafoHashTable(int size, long long p = 998244353)
        : m(size), table(size), hasher(p) {};

    // funzione per aggiungere un arco tra due nodi, se l'arco esiste già incrementa la frequenza
    void aggiungi_arco(int u, int v) {
        Nodo& nodo_u = get_nodo(u); //Troviamo o creiamo il nodo u
        
        // Scorriamo le adiacenze del nodo u per vedere se v è gia presente, nel caso incrementiamo la frequenza dell'arco
        for (auto& arco : nodo_u.adiacenze) {
            if (arco.destinazione == v) {
                arco.frequenza++; 
                return;
            }
        }
        
        // Se v non è presente nelle adiacenze di u, aggiungiamo un nuovo arco con frequenza 1
        nodo_u.adiacenze.push_back({v, 1});
    }
    
};


void caricaGrafoDaFile(const string& nomeFile, GrafoHashTable& grafo) {

// =========================================================================
// MODULO DI CARICAMENTO
// =========================================================================

    // Apertura del file specificato come argomento
    ifstream file(nomeFile); 
    
    // Gestione di eventuali errori nell'apertura del file
    if (!file.is_open()) {
        cout << "Errore nell'apertura del file" << endl;
        return; // Uscita dalla funzione in caso di errore
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

            int u=-1; // Variabile per sapere se siamo al primo numero della sequenza di nodi (per creare un arco servono coppie di nodi)
            int v; // Variabile per memorizzare i numeri letti dalla riga

            while(ss_numeri >> v) { // Estrae i numeri dal flusso saltando gli spazi bianchi
                if(u!=-1 && u!=v){ //se u è diverso da -1 significa che abbiamo già letto un numero e quindi possiamo creare un arco tra u e v
                                   //u!=v serve ad eliminare i self-loop
                    // qua avviene la comunicazione tra il modulo di caricamento e il modulo grafo
                    grafo.aggiungi_arco(u,v); //aggiungiamo l'arco tra u e v
                    
                    // cout << "Aggiunto arco: " << u << " -> " << v << endl; //messaggio di verifica
                    
                }
                u=v; //aggiorniamo u con il valore di v per il prossimo arco
                
            }
        }
    }
    file.close(); // Chiusura del file dopo la lettura
}



int main(int argc,char **argv){
    // Verifica che sia stato passato un file come argomento
    if (argc <2){
        cout << "Specifica un file" <<endl;
        return 0;
    }
    cout << "Programma: " << argv[0] << endl;
    cout << "File: " << argv[1] << endl;

    GrafoHashTable grafo(10000); //  Creazione di un grafo con dimensione della tabella hash m=10000

    caricaGrafoDaFile(argv[1], grafo); // Chiamata alla funzione di caricamento

    return 0;
}
