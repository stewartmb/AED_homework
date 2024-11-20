#include <iostream>
#include <vector>

using namespace std;

class Heap {
private:
    pair<long, long>* heap;
    int heapSize;
    int capacity;

    void heapifyDown(int index) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int extremeIndex = index;

        if (leftChild < heapSize && heap[leftChild].first < heap[extremeIndex].first) {
            extremeIndex = leftChild;
        }
        if (rightChild < heapSize && heap[rightChild].first < heap[extremeIndex].first) {
            extremeIndex = rightChild;
        }

        if (extremeIndex != index) {
            pair<long, long> temp = heap[index];
            heap[index] = heap[extremeIndex];
            heap[extremeIndex] = temp;
            heapifyDown(extremeIndex);
        }
    }

    void heapifyUp(int index) {
        int parentIndex = (index - 1) / 2;
        if (parentIndex >= 0) {
            if (heap[index] < heap[parentIndex]) {
                pair<long, long>  temp = heap[index];
                heap[index] = heap[parentIndex];
                heap[parentIndex] = temp;
                heapifyUp(parentIndex);
            }
        }
    }

public:
    Heap(int cap) : heapSize(0), capacity(cap) {
        heap = new pair<long, long>[capacity];
    }

    ~Heap() {
        delete[] heap;
    }

    void insert(pair<long, long> value) {
        if (heapSize >= capacity) {
            throw runtime_error("Heap is full");
        }
        heap[heapSize] = value;
        heapifyUp(heapSize);
        heapSize++;
    }

    pair<long, long> extractRoot() {
        if (heapSize == 0) {
            return {0, -1};
        }
        pair<int, int> rootValue = heap[0];
        heap[0] = heap[heapSize - 1];
        heapSize--;
        heapifyDown(0);
        return rootValue;
    }

    pair<long, long> getRoot() {
        if (heapSize == 0) {
            return {0, -1};
        }
        return heap[0];
    }

    void printHeap() {
        for (int i = 0; i < heapSize; i++) {
            cout << "(" << heap[i].first << ", " << heap[i].second << ") ";
        }
        cout << endl;
    }
};


class Solution {
public:
    int countPaths(int n, vector<vector<int>>& roads) {
        int start = 0;
        int end = n - 1;

        // crear grafo
        vector<vector<pair<int, int>>> graph(n + 1);
        for (const auto& road : roads) {
            int u = road[0], v = road[1], w = road[2];

            graph[u].emplace_back(v, w);
            graph[v].emplace_back(u, w);
        }

        // dijkstra modificado
        vector<long> dist(n + 1, LONG_MAX);
        vector<int> count(n + 1, 0);
        vector<bool> S(n + 1, false);
        dist[start] = 0;
        count[start] = 1;

        Heap minHeap(n);
        minHeap.insert({0, start}); // dist, index

        while (minHeap.getRoot().second != -1) {
            minHeap.printHeap();
            pair<long, long> root = minHeap.extractRoot();
            long d = root.first;
            int u = root.second;

            if (S[u]) continue;
            S[u] = true;

            if (u == end) {
                return count[u] % 1000000007;
            }

            for (const auto& [v, w] : graph[u]) {
                cout << "dist[" << u << "] = " << dist[u] << endl;
                cout << "count[" << u << "] = " << count[u] << endl;
                cout << "dist[" << v << "] = " << dist[v] << endl;
                cout << "count[" << v << "] = " << count[v] << endl;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    count[v] = count[u];
                    minHeap.insert({dist[v], v});
                    cout << "dist[" << v << "]a = " << dist[v] << endl;
                    cout << "count[" << v << "]a = " << count[v] << endl;
                } else if (dist[u] + w == dist[v]) {
                    count[v] = (count[v]% 1000000007) + (count[u]%1000000007);
                    cout << "dist[" << v << "]b = " << dist[v] << endl;
                    cout << "count[" << v << "]b = " << count[v] << endl;
                }
            }
        }
        return 0;
    }
};