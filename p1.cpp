#include <iostream>
#include <vector>

using namespace std;

// Clase Queue personalizada
template <typename T>
class Queue {
private:
    T* arr;
    int MAX_SIZE;
    int front;
    int rear;
    int count;
public:
    Queue(int size) : MAX_SIZE(size), front(0), rear(0), count(0) {
        arr = new T[MAX_SIZE];
    }

    ~Queue() {
        delete[] arr;
    }

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == MAX_SIZE;
    }

    void enqueue(T value) {
        if (isFull()) {
            return;
        }
        arr[rear] = value;
        rear = (rear + 1) % MAX_SIZE;
        count++;
    }

    T dequeue() {
        if (isEmpty()) {
            return T();
        }
        T value = arr[front];
        front = (front + 1) % MAX_SIZE;
        count--;
        return value;
    }

    int size() const {
        return count;
    }
};

// Clase Grafo
template <typename edgeValueType>
class Grafo {
private:
    int numVertices;
    vector<vector<pair<int, edgeValueType>>> listaAdyacencia;

public:
    Grafo(int vertices) {
        numVertices = vertices;
        listaAdyacencia.resize(numVertices);
    }

    void agregarArista(int origen, int destino, edgeValueType value) {
        listaAdyacencia[origen].push_back({destino, value});
        listaAdyacencia[destino].push_back({origen, value});
    }

    int BFS(int inicio, int destino) {
        vector<bool> visitado(numVertices, false);
        Queue<int> q(numVertices);
        q.enqueue(inicio);
        visitado[inicio] = true;

        int pasos = 0;

        while (!q.isEmpty()) {
            int nivelSize = q.size();
            for (int i = 0; i < nivelSize; i++) {
                int nodo = q.dequeue();

                if (nodo == destino) {
                    return pasos + 1;
                }

                for (auto& vecino : listaAdyacencia[nodo]) {
                    int siguienteNodo = vecino.first;
                    if (!visitado[siguienteNodo]) {
                        visitado[siguienteNodo] = true;
                        q.enqueue(siguienteNodo);
                    }
                }
            }
            pasos++;
        }

        return -1;
    }
};

class Solution {
public:
    int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
        int n = grid.size();
        if (grid[0][0] == 1 || grid[n - 1][n - 1] == 1) {
            return -1;
        }

        Grafo<int> grafo(n * n);

        int direcciones[8][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0},
                                 {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

        for (int fila = 0; fila < n; fila++) {
            for (int columna = 0; columna < n; columna++) {
                if (grid[fila][columna] == 0) {
                    int nodoActual = fila * n + columna;
                    for (int k = 0; k < 8; k++) {
                        int nuevaFila = fila + direcciones[k][0];
                        int nuevaColumna = columna + direcciones[k][1];

                        if (nuevaFila >= 0 && nuevaFila < n &&
                            nuevaColumna >= 0 && nuevaColumna < n &&
                            grid[nuevaFila][nuevaColumna] == 0) {
                            int nodoVecino = nuevaFila * n + nuevaColumna;
                            grafo.agregarArista(nodoActual, nodoVecino, 1);
                        }
                    }
                }
            }
        }

        int inicio = 0;
        int destino = n * n - 1;
        return grafo.BFS(inicio, destino);
    }
};
