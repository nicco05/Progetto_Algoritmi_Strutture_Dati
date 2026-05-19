//caida: all-path 
//vogliamo fare una struttura dati tale che dati 2 nodi del grafo quale è il peso minimo 
//di un cammino(minimo massimo del peso di un arco nel cammino)
// che collega dei 2 nodi e quali sono i cammini di peso minimo di questi 2 nodi e qaunto è lungo?
#include<fstream>
#include<iostream>
#include<string>
using namespace std;

int main(int arg,char **argv){
    if (argc <2){
        cout << "specifica un file" <<endl;
        return 0;
    }
    cout << argv[0] << endl;
    cout << argv[1] << end;
    ifstream file(argv[1]);
    string line;
    getline(file, line);
    cout << line << endl;
}
