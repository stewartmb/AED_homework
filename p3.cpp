#include <iostream>
#include <vector>

using namespace std;

template <typename edgeValueType>
class Grafo {
private:
    struct Pair {
        int key;
        edgeValueType value;

        Pair() : key(int()), value(edgeValueType()) {}
        Pair(const int& key, const edgeValueType& value) : key(key), value(value) {}

        void setValue(const edgeValueType& newValue) {
            value = newValue;
        }
    };

    bool isDirected;
    int numVertices;
    vector<vector<Pair>> listaAdyacencia;

    void auxDFS(int vertice, vector<bool>& visitado) {
        visitado[vertice] = true;
        cout << vertice << " ";

        for (Pair vecino : listaAdyacencia[vertice]) {
            if (!visitado[vecino.key]) {
                auxDFS(vecino.key, visitado);
            }
        }
    }
/*
    // Función para inicializar distancias y predecesores
    void initializeSingleSource(vector<edgeValueType>& dist, vector<int>& predecesor, int inicio) {
        for (int i = 0; i < numVertices; i++) {
            dist[i] = numeric_limits<edgeValueType>::max();
            predecesor[i] = -1; // -1 indica que no tiene predecesor
        }
        dist[inicio] = 0;
    }

    // Función de relajación
    void relax(int u, int v, edgeValueType peso, vector<edgeValueType>& dist, vector<int>& predecesor) {
        if (dist[u] != numeric_limits<edgeValueType>::max() && dist[u] + peso < dist[v]) {
            dist[v] = dist[u] + peso;
            predecesor[v] = u;
        }
    }
*/
    //initializeSingleSource modificada
    void initializeSingleSource(vector<edgeValueType>& prob, int inicio) {
        for (int i = 0; i < numVertices; i++) {
            prob[i] = 0;
        }
        prob[inicio] = 1;
    }

    // relax modificada
    void relax(int u, int v, edgeValueType peso, vector<edgeValueType>& prob, bool& updated) {
        if (prob[u] * peso > prob[v]) {
            prob[v] = prob[u] * peso;
            updated = true;
        }
    }

public:
    Grafo(int vertices, bool dirigido = false) {
        numVertices = vertices;
        isDirected = dirigido;
        listaAdyacencia.resize(numVertices);
    }

    void agregarArista(int origen, int destino, edgeValueType value) {
        if (origen < 0 || origen >= numVertices || destino < 0 || destino >= numVertices) {
            cout << "Error: Vertices fuera de rango" << endl;
            return;
        }
        if (isDirected) { // Dirigido
            listaAdyacencia[origen].push_back(Pair(destino, value));
        } else { // No dirigido
            listaAdyacencia[origen].push_back(Pair(destino, value));
            listaAdyacencia[destino].push_back(Pair(origen, value));
        }
    }

    void DFS(int inicio) {
        vector<bool> visitado(numVertices, false);
        auxDFS(inicio, visitado);
    }

    void imprimirGrafo() {
        for (int i = 0; i < numVertices; i++) {
            cout << "Vertice " << i << ":";
            for (Pair arista : listaAdyacencia[i]) {
                cout << " -> " << arista.key << ":(" << arista.value << ")";
            }
            cout << endl;
        }
    }

// bellmanford modificado para el ejercicio
    double bellmanFord(int inicio , int end_node ) {
        vector<edgeValueType> dist(numVertices);
        //vector<int> predecesor(numVertices);

        initializeSingleSource(dist, inicio);

        for (int i = 0; i < numVertices - 1; i++) {
            bool updated = false;
            for (int u = 0; u < numVertices; u++) {
                for (Pair arista : listaAdyacencia[u]) {
                    int v = arista.key;
                    edgeValueType peso = arista.value;
                    relax(u, v, peso, dist, updated);
                }
            } if (!updated) {break;}
        }

        for (int u = 0; u < numVertices; u++) {
            for (Pair arista : listaAdyacencia[u]) {
                int v = arista.key;
                edgeValueType peso = arista.value;
                if (dist[u] != 0 && dist[u] * peso > dist[v]) {
                    cout << "El grafo contiene un ciclo negativo." << endl;
                    return 0;
                }
            }
        }
        return dist[end_node];
    }
};

class Solution {
public:
    double maxProbability(int n, vector<vector<int>>& edges, vector<double>& succProb, int start_node, int end_node) {
        Grafo<double> grafo(n,false);
        for (int i = 0; i < edges.size(); i++) {
            int origen = edges[i][0];
            int destino = edges[i][1];
            double value = succProb[i];
            grafo.agregarArista(origen, destino, value);
        }
        return grafo.bellmanFord(start_node, end_node);
    }
};