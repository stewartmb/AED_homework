#include <iostream>
#include <string>

using namespace std;
template<typename T2,typename T3>
class Pair {
public:
    T2 first;
    T3 second;
    Pair(T2 f, T3 s) : first(f), second(s) {}
};


// Estructura del Nodo
template<typename T1>
class Node {
public:

    T1 data;
    Node* next;
    Node* prev;

    Node(const T1& value) : data(value), next(nullptr), prev(nullptr) {}

    // Constructor que acepta un objeto Pair
    template<typename T2, typename T3>
    Node(const Pair<T2, T3>& pair) : data(pair), next(nullptr), prev(nullptr) {}

};



// Clase de la Lista
template<typename T>
class Double_List {
private:
    Node<T> *tail;  // cola
    Node<T> *head;  // cabeza
public:
    // Constructor
    Double_List() : head(nullptr), tail(nullptr) {}
    bool empty() const {
        return head == nullptr;
    }

    //Retorna el elemento al comienzo
    T front() {
        return head->data;
    }

    //Retorna el elemento del final
    T back() {
        return tail->data;
    }

    // Método para agregar un elemento al inicio de la lista
    void push_front(T value) {
        Node<T> *newNode = new Node<T>();       // Crear nuevo nodo
        newNode->data = value;                  // Asignar valor al nodo
        if (head == nullptr && tail == nullptr) {
            head = newNode;                     // el nuevo nodo pasa a ser el (heap)=
        } else {
            newNode->next = head;               // El nuevo nodo apuntara a (head)
            head->prev = newNode;               // El (head) apuntara al nuevo nodo
            head = newNode;                     // el nuevo nodo pasa a ser el (heap)
        }
    }

    // Método para agregar un elemento al final de la lista
    void push_back(T value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->data = value;
        if (head == nullptr && tail == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
    }



    // Remueve el elemento al comienzo
    T pop_front() {
        if (head == tail){
            head = nullptr;
            tail = nullptr;
        }
        else {
            Node<T> *temp = head;
            head = temp->next;
            head->prev = nullptr;
            delete temp;
        }
    }

    // Remueve el elemento al final
    T pop_back() {
        if(head ==tail){
            head = nullptr;
            tail = nullptr;
        }
        else {
            Node<T> *temp = tail;
            tail = temp->prev;
            tail->next = nullptr;
            delete temp;
        }
    }

    // Inserta en cualquier posición
    void insert_in_list(T value, int position) {
        Node<T> *newNode = new Node<T>();
        newNode->data = value;
        Node<T> *temp = head;
        if (position == 0) {
            push_front(value);
        } else {
            for (int i = 0; i < position - 1; i++) {
                temp = temp->next;
            }
            if (temp == tail) {
                push_back(value);
            } else {
                temp->next->prev = newNode;
                newNode->prev = temp;
                newNode->next = temp->next;
                temp->next = newNode;
            }

        }
    }

    // Remueve en cualquier posicion
    void remove_in_list(int position) {

        Node<T> *temp = head;
        if (position == 0) {
            pop_front();
        } else {
            for (int i = 0; i < position ; i++) {
                temp = temp->next;
            }
            if (temp == tail) {
                pop_back();
            } else {
                temp->next->prev = temp->prev;
                temp->prev->next = temp->next;
                delete temp;
            }

        }
    }
    //Retorna el elemento en la posición indicada
    T operator[ ](int position){
        Node<T> *temp = head;
        for (int i = 0; i < position ; i++) {
            temp = temp->next;
        }
        return temp->data;
    }

    //Retorna si la lista está vacía o no
    bool empty_in_list(){
        if(head == nullptr && tail == nullptr){
            return true;
        }
        else {
            return false;
        }
    }


    // Retorna el tamaño de la lista
    int size(){
        Node<T>* temp = head;
        int i = 0;
        if (head == nullptr && tail == nullptr){
            return 0;
        }
        else {
            while (temp != tail) {
                temp = temp->next;
                i++;
            }
            return i;
        }
    }
    // Elimina todos los elementos de la lista
    void clear(){
        while (head != nullptr) {
            remove(0);
        }
    }


    // Revierte la lista
    void reverse() {
        Node<T>* temp = head;
        Node<T>* temp1 = nullptr;
        while (temp != nullptr) {
            temp1 = temp->prev;
            temp->prev = temp->next;
            temp->next = temp1;
            temp = temp->prev;
        }
        if (temp1 != nullptr) {
            temp1 = head;
            head = tail;
            tail = temp1;
        }
    }


    // Método para imprimir todos los elementos de la lista
    // Print the list
    void print() const {
        Node<T>* temp = head;
        while (temp) {
            cout << '('<<temp->data.first << ',' << temp->data.second << ") ";
            temp = temp->next;
        }
    }


    // Destructor para liberar memoria
    ~Double_List() {
        while (head != nullptr) {
            Node<T> *temp = head;
            head = head->next;
            delete temp;
        }
    }
};


template<typename T2,typename T3>
class table_hash {
private:
    int MAX_SIZE;
    float MAX_FACTOR;
    int MAX_LIST;
    Double_List<Pair<T2,T3>> **table;

    // Sobrecarga de funcion hash para string
    int hash_function(const string& key) {
        int hash = 0;
        int a = 31;
        for (char c : key) {
            hash = (hash * a + static_cast<int>(c)) % MAX_SIZE;
        }
        return hash;
    }

    // Sobrecarga de funcion hash para int
    int hash_function(int key) {
        return key % MAX_SIZE;
    }

public:
    table_hash(int n, float m, int l) {
        MAX_SIZE = n;
        MAX_FACTOR = m;
        MAX_LIST = l;
        table = new Double_List<Pair<T2,T3>>*[MAX_SIZE];
        for (int i = 0; i < MAX_SIZE; ++i) {
            table[i] = nullptr;
        }
    }

    void insert_in_table(T2 key, T3 value) {
        int index = hash_function(key);
        if (table[index] == nullptr) {
            table[index] = new Double_List<Pair<T2,T3>>();
        }
        table[index]->push_back(Pair<T2,T3>(key,value));
    }

    void print_table() {
        for (int i = 0; i < MAX_SIZE; i++) {
            cout << i << ": ";
            if (table[i] != nullptr) {
                table[i]->print();
            } else {
                cout << "";
            }
            cout << endl;
        }
    }

    ~table_hash() {
        for (int i = 0; i < MAX_SIZE; ++i) {
            delete table[i];
        }
        delete[] table;
    }
};

int main() {
    table_hash<string,string> table(10, 0.75, 5);
    table.insert_in_table("Vicente","1");
    table.insert_in_table("Eva","2");
    table.insert_in_table("Martin","3");
    table.insert_in_table("Sebastian","4");
    table.insert_in_table("Yafaedh","5");
    table.insert_in_table("Nadia","6");
    table.insert_in_table("Yoselin","7");
    table.insert_in_table("Stewart","8");
    table.insert_in_table("Jennifer","9");
    table.insert_in_table("Li","10");
    table.insert_in_table("Luciana","11");
    table.insert_in_table("Leon","19");


    table.print_table();

    return 0;
}