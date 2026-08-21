#include<fstream>
#include<vector>
#include<iostream>
#include<string> 
#include<sstream> //per trasformare la riga in un flusso di dati
#include<algorithm> //per la funzione replace
#include<random> //per generare numeri casuali
#include<limits> //per la costante INF
#include<queue> //per la coda di priorità

using namespace std;

const int INF=numeric_limits<int>::max(); //costante per rappresentare l'infinito

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
        long long hash (long long x) { return (a*x+b)%p; } // funzione di hash universale

        UniversalHash(long long _p) // costruttore della classe UniversalHash
            : p(_p), a(rand() %  (_p-1) +1), b(rand()% _p) {} 

        void rehash(){ 
            a=rand() %  (p-1) +1;
            b=rand()% p;
        }
};

// Struttura che rappresenta la tripla
struct InfoCammino{
    int nodo_id; //id del nodo
    int costo; //costo cammino da S a U (peso del massimo arco nel cammmino)
    int predecessore; //nodo precedente nel cammino
};

// Tabella hash per tracciare Dijkstra
class TabellaTracciamento {
private:
    int m; // dimensione della tabella hash
    vector<vector<InfoCammino>> table; // vettore di vettori di InfoCammino
    UniversalHash hasher; 

    // funzione per calcolare l'hash di un nodo
    int calcolaHash(int x) { 
        int h = hasher.hash(x) % m;
        if (h < 0) h += m;
        return h;
    }

public:
    // costruttore della classe TabellaTracciamento
    TabellaTracciamento(int size, long long p = 998244353)
        : m(size), table(size), hasher(p) {};

    // ritorna le info del nodo, se non esistono le crea con peso infinito e le aggiunge alla tabella hash
    InfoCammino& get_info(int nodo_id) { // nodo_id è l'id del nodo che vogliamo ottenere
        int h = calcolaHash(nodo_id);
        
        for (auto& info : table[h]) { // scorre tutti gli InfoCammino nella posizione h della tabella hash
            // se trovo l'InfoCammino con l'id nodo_id lo ritorno
            if (info.nodo_id == nodo_id) { 
                return info; 
            }
        }
        
        //altrimenti creo un nuovo InfoCammino con l'id nodo_id e lo aggiungo alla tabella hash
        table[h].push_back({nodo_id, INF, -1}); // -1 perche non ha predecessore inizialmente
        return table[h].back();
    }
};

// Hash table per rappresentare il grafo
class GrafoHashTable {
private:
    int m; // dimensione della tabella hash
    vector<vector<Nodo>> table; // vettore di vettori di nodi, rappresenta la tabella hash 
                                //quindi ogni posizione della tabella può contenere più nodi (gestione delle collisioni)
    UniversalHash hasher; 

    // funzione per calcolare l'hash di un nodo
    int calcolaHash(int x) { 
        int h = hasher.hash(x) % m;
        if (h < 0) h += m;
        return h;
    }

    // funzione per ottenere un nodo dalla tabella hash, se non esiste lo crea
    Nodo& get_nodo(int nodo_id) { // nodo_id è l'id del nodo che vogliamo ottenere
        int h = calcolaHash(nodo_id);
        
        for (auto& nodo : table[h]) { // scorre tutti i nodi nella posizione h della tabella hash
            // se trovo il nodo con l'id nodo_id lo ritorno
            if (nodo.id == nodo_id) { 
                return nodo; 
            }
        }
        
        //altrimenti creo un nuovo nodo con l'id nodo_id e lo aggiungo alla tabella hash
        table[h].push_back({nodo_id, vector<Arco>()});
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
                return; // Uscita dalla funzione dopo aver incrementato la frequenza
            }
        }
        
        // Se v non è presente nelle adiacenze di u, aggiungiamo un nuovo arco con frequenza 1
        nodo_u.adiacenze.push_back({v, 1});
    }
    
    // =========================================================================
    // MODULO ALGORITMICO: DIJKSTRA MINIMAX
    // =========================================================================
    
    vector<int> calcolaCamminoMinimax(int nodo_sorgente, int nodo_destinazione) {
        
        TabellaTracciamento taccuino(10000);// Creiamo la tabella di tracciamento per memorizzare i costi e i predecessori dei nodi
        
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // Coda di Priorità, salva coppie <Costo_Corrente, AS_ID>
        // primo parametro: tipo di dato della coppia, secondo parametro: come viene salvato nella RAM, terzo parametro: come viene ordinato (min-heap)

        
        InfoCammino& info_sorgente = taccuino.get_info(nodo_sorgente);// inseriamo la sorgente in pq
        info_sorgente.costo = 0;
        info_sorgente.predecessore = nodo_sorgente; // La radice ha come predecessore se stessa
        pq.push({0, nodo_sorgente});
        
        // Estrazione iterativa
        while (!pq.empty()) {
            int costo_attuale = pq.top().first; // Costo del nodo estratto
            int u = pq.top().second; // AS_ID del nodo estratto
            pq.pop(); // rimuove il primo elemento dalla coda di priorità
            
            // Se estraiamo la destinazione abbiamo finito
            if (u == nodo_destinazione) {
                break;
            }
        
            InfoCammino& info_u = taccuino.get_info(u);// Recuperiamo le info del nodo estratto dal taccuino o le creiamo se non esistono ancora
            if (costo_attuale > info_u.costo) continue;// Evitiamo di processare percorsi vecchi o peggiori rimasti in coda di priorità

            // Recuperiamo il nodo u dalla tabella hash del grafo
            Nodo& nodo_u = get_nodo(u);
            
            // Fase di aggiornamento (Minimax)
            for (const auto& arco : nodo_u.adiacenze) { // fin tanto che ci sono nodi nella lista di adiacenza di u
                int v = arco.destinazione; // chiamo v il nodo adiacente a u
                int peso_arco = arco.frequenza;
                InfoCammino& info_v = taccuino.get_info(v);// Recuperiamo le info del nodo v dal taccuino o le creiamo se non esistono ancora

                int candidato_peso = max(info_u.costo, peso_arco); // costo del cammino minimax dalla sorgente alla destinazione passando per u
                
                if (candidato_peso < info_v.costo) { //se conviene passare per u aggiorniamo il costo ed il predecessore di v
                    info_v.costo = candidato_peso;
                    info_v.predecessore = u;
                    pq.push({candidato_peso, v}); // lo reinseriamo nella coda di priorità con il nuovo costo (lazy dijkstra)
                }
            }
        }
        
        // Ricostruzione del cammino minimax dalla destinazione alla sorgente usando i predecessori
        vector<int> percorso_finale; // Vettore per memorizzare il percorso finale dalla sorgente alla destinazione
        InfoCammino& info_dest = taccuino.get_info(nodo_destinazione); // Recuperiamo le info del nodo destinazione dal taccuino o le creiamo se non esistono ancora
        
        // Se il predecessore è rimasto -1, la destinazione è irraggiungibile
        if (info_dest.predecessore == -1) {
            return percorso_finale; // Ritorna vettore vuoto
        }
        
        // il predecessore della destinazione non è -1, quindi esiste un cammino
        int nodo_corrente = nodo_destinazione;
        while (nodo_corrente != nodo_sorgente) { // Risaliamo l'albero dei predecessori fino a raggiungere la sorgente
            percorso_finale.push_back(nodo_corrente); // Aggiungiamo il nodo corrente al percorso finale
            nodo_corrente = taccuino.get_info(nodo_corrente).predecessore; // passi al predecessore del nodo corrente
        }
        percorso_finale.push_back(nodo_sorgente); // Aggiungiamo la sorgente al percorso finale
        
        // Rovesciamo per avere l'ordine corretto
        reverse(percorso_finale.begin(), percorso_finale.end());
        
        return percorso_finale;
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
    cout << "Grafo caricato correttamente." << endl;

    int nodo_sorgente, nodo_destinazione;

    while (true) {
    cout << "Inserisci il nodo sorgente, oppure -1 per uscire: ";
    cin >> nodo_sorgente;
    if (nodo_sorgente == -1) {
        cout << "Uscita dal programma." << endl;
        break;
    }
    cout << "Inserisci il nodo destinazione: ";
    cin >> nodo_destinazione;

    cout << "\nCalcolo del cammino Minimax da " << nodo_sorgente << " a " << nodo_destinazione << endl;
    vector<int> cammino = grafo.calcolaCamminoMinimax(nodo_sorgente, nodo_destinazione);

    // Stampa del risultato
    if (cammino.empty()) {
        cout << "Nessun percorso trovato tra i due nodi (oppure i nodi non esistono)." << endl;
    } else {
        cout << "Percorso trovato: ";
        for (size_t i = 0; i < cammino.size(); ++i) {
            cout << cammino[i];
            if (i < cammino.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
    }

    return 0;
}


