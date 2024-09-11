#include <iostream>

template <typename T>
class CircularDoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(T val = T()) : data(val), prev(nullptr), next(nullptr) {}
    };

    Node* sentinel;
    int list_size;

public:
    CircularDoublyLinkedList() {
        sentinel = new Node();  // Nodo centinela
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        list_size = 0;
    }

    // Retorna el primer elemento
    T front() {
        if (!empty()) {
            return sentinel->next->data;
        }
        throw std::out_of_range("List is empty");
    }

    // Retorna el último elemento
    T back() {
        if (!empty()) {
            return sentinel->prev->data;
        }
        throw std::out_of_range("List is empty");
    }

    // Agrega un elemento al comienzo
    void push_front(T value) {
        Node* new_node = new Node(value);
        Node* first = sentinel->next;

        new_node->next = first;
        new_node->prev = sentinel;
        sentinel->next = new_node;
        first->prev = new_node;

        list_size++;
    }

    // Agrega un elemento al final
    void push_back(T value) {
        Node* new_node = new Node(value);
        Node* last = sentinel->prev;

        new_node->next = sentinel;
        new_node->prev = last;
        last->next = new_node;
        sentinel->prev = new_node;

        list_size++;
    }

    // Remueve el primer elemento
    T pop_front() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        Node* first = sentinel->next;
        T value = first->data;

        sentinel->next = first->next;
        first->next->prev = sentinel;

        delete first;
        list_size--;

        return value;
    }

    // Remueve el último elemento
    T pop_back() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        Node* last = sentinel->prev;
        T value = last->data;

        sentinel->prev = last->prev;
        last->prev->next = sentinel;

        delete last;
        list_size--;

        return value;
    }

    // Inserta un elemento en una posición específica
    void insert(T value, int position) {
        if (position < 0 || position > list_size) {
            throw std::out_of_range("Invalid position");
        }

        Node* new_node = new Node(value);
        Node* current = sentinel;

        for (int i = 0; i <= position; ++i) {
            current = current->next;
        }

        new_node->next = current;
        new_node->prev = current->prev;
        current->prev->next = new_node;
        current->prev = new_node;

        list_size++;
    }

    // Remueve un elemento en una posición específica
    void remove(int position) {
        if (position < 0 || position >= list_size) {
            throw std::out_of_range("Invalid position");
        }

        Node* current = sentinel->next;
        for (int i = 0; i < position; ++i) {
            current = current->next;
        }

        current->prev->next = current->next;
        current->next->prev = current->prev;

        delete current;
        list_size--;
    }

    // Retorna el tamaño de la lista
    int size() const {
        return list_size;
    }

    // Verifica si la lista está vacía
    bool empty() const {
        return list_size == 0;
    }

    // Limpia la lista
    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    // Revierte la lista
    void reverse() {
        if (empty()) return;

        Node* current = sentinel;
        do {
            Node* temp = current->next;
            current->next = current->prev;
            current->prev = temp;
            current = temp;
        } while (current != sentinel);
    }

    // Método para imprimir todos los elementos de la lista
    void print() const {
        if (empty()) {
            std::cout << "List is empty" << std::endl;
            return;
        }

        Node* current = sentinel->next;
        while (current != sentinel) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // Destructor
    ~CircularDoublyLinkedList() {
        clear();
        delete sentinel;
    }
};

int main() {
    CircularDoublyLinkedList<int> list;

    list.push_back(6);
    list.push_back(2);
    list.push_back(9);
    list.push_back(3);


    list.print();
    list.pop_front();
    list.print();
    list.remove(0);
    list.remove(0);
    list.remove(0);

    list.print();
    list.push_back(3);
    list.print();
    list.push_back(6);
    list.push_back(2);
    list.push_back(9);
    list.push_back(3);
    list.print();

    list.reverse();
    list.print();

    return 0;
}
