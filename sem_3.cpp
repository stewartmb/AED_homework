#include <iostream>

// Estructura del Nodo
template<typename T>
struct Node {
    T data;
    Node *next;
    Node *prev;

};

// Clase de la Lista
template<typename T>
class List {
private:
    Node<T> *tail;  // cola
    Node<T> *head;  // cabeza
public:
    // Constructor
    List() : head(nullptr), tail(nullptr) {}

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
        Node<T> *newNode = new Node<T>();
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
    void insert(T value, int position) {
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
    void remove(int position) {

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
    bool empty(){
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
    void print() const {
        Node<T> *temp = head;
        while (temp != nullptr) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }
        std::cout << std::endl;
    }

    // Destructor para liberar memoria
    ~List() {
        while (head != nullptr) {
            Node<T> *temp = head;
            head = head->next;
            delete temp;
        }
    }
};


int main() {
    List<int> myList;
    std::cout << "Foward List: " << std::endl;
    myList.push_back(1);
    myList.push_back(2);
    myList.push_back(3);
    myList.push_back(4);
    myList.print();


    myList.pop_front();
    std::cout << "Pop_front: " << std::endl;
    myList.print();
    std::cout << "Pop_back: " << std::endl;
    myList.pop_back();
    myList.print();
    std::cout << "Insert(1,0): " << std::endl;
    myList.insert(1, 0);
    myList.print();
    std::cout << "Insert(4,3): " << std::endl;
    myList.insert(4, 3);
    myList.print();
    std::cout << "Remove(3): " << std::endl;
    myList.remove(2);
    myList.print();
    std::cout << "Insert(4,3): " << std::endl;
    myList.insert(3, 2);
    myList.print();
    std::cout << "Retornar elemento de la posicion 3: \n"<<myList.operator[](3) << std::endl;
    std::cout << "La lista es vacia? \n"<<myList.empty()<< std::endl;
    myList.remove(0);
    myList.remove(0);
    myList.remove(0);
    myList.remove(0);
    myList.print();

    std::cout << "La lista es vacia? \n"<<myList.empty()<< std::endl;
    std::cout << "tamaño de la lista: \n"<<myList.size()<< std::endl;
    myList.push_back(1);
    myList.print();
    List<int> myList1;
    myList1.push_back(1);
    myList1.push_back(2);
    myList1.push_back(3);
    myList1.print();
    std::cout <<myList1.back()<<std::endl;


    std::cout << "tamaño de la lista: \n"<<myList.size()<< std::endl;


    std::cout << "Foward List: " << std::endl;
    myList.push_back(2);
    myList.push_back(3);
    myList.push_back(4);
    myList.print();
    myList.reverse();
    myList.print();




    myList.~List();
}