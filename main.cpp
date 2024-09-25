#include <iostream>

using namespace std;

template <typename T>
struct Node {
    T data;
    Node* next;
};

template <typename T>
class List {
private:
    Node<T>* head;

    Node<T>* merge(Node<T>* first, Node<T>* second) {
        if (!first)
            return second;
        if (!second)
            return first;
        if (first->data < second->data) {
            first->next = merge(first->next,second);
            return first;
        } else {
            second->next = merge(first,second->next);
            return second;
        }
    }
    Node<T>* mergeSort(Node<T>* head) {
        if (!head || !head->next)
            return head;
        Node<T>* slow = head;
        Node<T>* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node<T>* temp = slow->next;
        slow->next = NULL;
        return merge(mergeSort(head), mergeSort(temp));
    }

public:
    List(){
        head = nullptr;
    }
    ~List() {
        clear();
    }
    T front(){
        if(head != nullptr){
            return head->data;
        }
        return T();
    }
    T back(){
        if(head != nullptr){
            Node<T>* temp = head;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            return temp->data;
        }
        return T();
    }
    void push_front(T data){
        Node<T>* newNode = new Node<T>();
        newNode->data = data;
        newNode->next = head;
        head = newNode;
    }
    void push_back(T data){
        Node<T>* newNode = new Node<T>();
        newNode->data = data;
        newNode->next = nullptr;
        if(head == nullptr){
            head = newNode;
        }else{
            Node<T>* temp = head;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }
    T pop_front(){
        if(head != nullptr){
            Node<T>* temp = head;
            head = head->next;
            T data = temp->data;
            delete temp;
            return data;
        }
        return T();
    }
    T pop_back(){
        if(head != nullptr){
            if(head->next == nullptr){
                T data = head->data;
                delete head;
                head = nullptr;
                return data;
            }else{
                Node<T>* temp = head;
                while(temp->next->next != nullptr){
                    temp = temp->next;
                }
                T data = temp->next->data;
                delete temp->next;
                temp->next = nullptr;
                return data;
            }
        }
        return T();
    }
    T operator[](int index){
        if(head != nullptr){
            Node<T>* temp = head;
            for(int i = 0; i < index; i++){
                if(temp->next == nullptr){
                    return T();
                }
                temp = temp->next;
            }
            return temp->data;
        }
        return T();
    }
    bool empty(){
        return head == nullptr;
    }

    void insert(int index, T data){
        if(index == 0){
            push_front(data);
        }else{
            Node<T>* temp = head;
            for(int i = 0; i < index - 1; i++){
                if(temp->next == nullptr){
                    return;
                }
                temp = temp->next;
            }
            Node<T>* newNode = new Node<T>();
            newNode->data = data;
            newNode->next = temp->next;
            temp->next = newNode;
        }
    }

    void remove(int index){
        if(head != nullptr){
            if(index == 0){
                pop_front();
            }else{
                Node<T>* temp = head;
                for(int i = 0; i < index - 1; i++){
                    if(temp->next == nullptr){
                        return;
                    }
                    temp = temp->next;
                }
                if(temp->next != nullptr){
                    Node<T>* toDelete = temp->next;
                    temp->next = toDelete->next;
                    delete toDelete;
                }
            }
        }
    }

    int size(){
        int count = 0;
        Node<T>* temp = head;
        while(temp != nullptr){
            temp = temp->next;
            count++;
        }
        return count;
    }

    void clear(){
        while(head != nullptr){
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void sort() {
        head = mergeSort(head);
    }

    void reverse(){
        if(head != nullptr){
            Node<T>* prev = nullptr;
            Node<T>* current = head;
            Node<T>* next = nullptr;
            while(current != nullptr){
                next = current->next;
                current->next = prev;
                prev = current;
                current = next;
            }
            head = prev;
        }
    }
    Node<T> *getHead(){
        return head;
    }
};

template <typename key_type, typename value_type>
class Hashmap {
private:
    struct Pair {
        key_type key;
        value_type value;

        Pair() : key(key_type()), value(value_type()) {}
        Pair(const key_type& key, const value_type& value) : key(key), value(value) {}

        void setValue(const value_type& newValue) {
            value = newValue;
        }

        bool operator==(const Pair& other) const {
            return (key == other.key) && (value == other.value);
        }
        bool operator!=(const Pair& other) const {
            return !(*this == other);
        }
    };

    int MAX_SIZE;
    float MAX_FACTOR;
    int MAX_LIST;
    List<Pair>* arr;

    // Funcion hash para string
    int hash_function(const string& key) {
        int hash = 0;
        int a = 31;
        for (char c : key) {
            hash = (hash*a + static_cast<int>(c)) % MAX_SIZE;
        }
        return hash;
    }
    // Sobrecarga de funcion hash para int
    int hash_function(int key) {
        return key % MAX_SIZE;
    }
public:
    Hashmap(int size, float factor, int list){
        MAX_SIZE = size;
        arr = new List<Pair>[MAX_SIZE];
        MAX_FACTOR = factor;
        MAX_LIST = list;
    }
    ~Hashmap(){
        delete[] arr;
    }
    void insert(key_type key, value_type value){
        int index = hash_function(key);
        Pair p(key, value);
        // si la llave ya existe se actualiza el valor
        Node<Pair>* temp = arr[index].getHead();
        int nodeCount = 0;
        while (temp != nullptr) {
            //si ya existe el key se actualiza el value
            if (temp->data.key == key) {
                temp->data.setValue(value);
                return;
            }
            //se recorre la lista
            temp = temp->next;
            nodeCount++;
            // si la lista es mayor a MAX_LIST se hace rehash
            if (nodeCount > MAX_LIST) {
                rehash(MAX_SIZE * 2);
                insert(key, value);
                return;
            }
        }
        // si no existe se inserta
        arr[index].push_back(p);
    }
    value_type search(key_type key){
        int index = hash_function(key);
        Node<Pair>* temp = arr[index].getHead();
        while (temp != nullptr) {
            if (temp->data.key == key) {
                return temp->data.value;
            }
            temp = temp->next;
        }
        return value_type();
    }
    void remove(key_type key){
        int index = hash_function(key);
        Node<Pair>* temp = arr[index].getHead();
        int i = 0;
        while (temp != nullptr) {
            if (temp->data.key == key) {
                arr[index].remove(i);
                return;
            }
            temp = temp->next;
            i++;
        }
    }
    void clear(){
        for (int i = 0; i < MAX_SIZE; i++) {
            arr[i].clear();
        }
    }
    void rehash(int newSize){
        List<Pair>* temp = arr;
        int oldSize = MAX_SIZE;
        MAX_SIZE = newSize;
        arr = new List<Pair>[MAX_SIZE];
        for (int i = 0; i < oldSize; i++) {
            Node<Pair>* tempNode = temp[i].getHead();
            while (tempNode != nullptr) {
                insert(tempNode->data.key, tempNode->data.value);
                tempNode = tempNode->next;
            }
        }
        delete[] temp;
        cout << "Se hizo un rehash. Nuevo tamanio: " << newSize << endl;
    }

    void print(){
        for (int i = 0; i < MAX_SIZE; i++) {
            Node<Pair>* temp = arr[i].getHead();
            cout << i << ": ";
            while (temp != nullptr) {
                cout << "(" << temp->data.key << ", " << temp->data.value << ") ";
                temp = temp->next;
            }
            cout << endl;
        }

    }
};


template <typename T>
class BinaryTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node() :  data(T()), left(nullptr), right(nullptr) {}
        Node(T value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void auxInsert(Node* node, T value) {
        if (value < node->data) {
            if (node->left == nullptr) {
                node->left = new Node(value);
            } else {
                auxInsert(node->left, value);
            }
        } else if (value > node->data) {
            if (node->right == nullptr) {
                node->right = new Node(value);
            } else {
                auxInsert(node->right, value);
            }
        }
    }

    Node *auxRemove(Node *current, T value) {
        if (current == nullptr)
            return current;
        // search
        if (value < current->data) {

            current->left = auxRemove(current->left, value);
        } else if (value > current->data) {
            current->right = auxRemove(current->right, value);
        }
            // current->data == value
        else {
            // no children
            if (current->left == nullptr && current->right == nullptr) {
                delete current;
                return nullptr;
            }
                // one child
            else if (current->left == nullptr) {
                Node *temp = current->right;
                delete current;
                return temp;
            } else if (current->right == nullptr) {
                Node *temp = current->left;
                delete current;
                return temp;
            }
                // two children
            else {
                Node *temp_parent = current;
                Node *temp = current->left;
                while (temp->right != nullptr) {
                    temp_parent = temp;
                    temp = temp->right;
                }
                if (temp_parent == current) {
                    temp->right = current->right;
                    delete current;
                    return temp;
                } else {
                    temp_parent->right = temp->left;
                    temp->left = current->left;
                    temp->right = current->right;
                    delete current;
                    return temp;
                }
            }
        }
        return current;
    }

    void auxClear(Node* node) {
        if (node != nullptr) {
            auxClear(node->left);
            auxClear(node->right);
            delete node;
        }
    }

    int auxHeight(Node* node) {
        if (node == nullptr) return -1;
        int left = auxHeight(node->left);
        int right = auxHeight(node->right);
        return 1 + max(left, right);
    }

    int auxpeso(Node* node) const{
        if (node == nullptr) return 0;
        int left = auxpeso(node->left);
        int right = auxpeso(node->right);
        return left +right+1;
    }

    void preOrder(Node* node) const {
        if (node == nullptr) return;
        cout << node->data << " ";
        preOrder(node->left);
        preOrder(node->right);
    }

    void inOrder(Node* node) const {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }



    void postOrder(Node* node) const {
        if (node == nullptr) return;
        postOrder(node->left);
        postOrder(node->right);
        cout << node->data << " ";
    }


    bool aux_balanced(Node* node) {
        if (node == nullptr) return true;
        else if (abs(auxHeight(node->right) - auxHeight(node->left))>1)
            return false;
        bool bleft = aux_balanced(node->left);
        bool bright = aux_balanced(node->right);
        return bleft*bright;
    }

    void drawCurrentDepth(Node* node, int depth, int spaces) {
        if (node == nullptr) {
            if (depth == 0) {
                for (int i = 0; i < spaces; i++) cout << " ";
                cout << "X";
                for (int i = 0; i < spaces + 1; i++) cout << " ";
                return;
            }
            drawCurrentDepth(nullptr, depth - 1, spaces);
            drawCurrentDepth(nullptr, depth - 1, spaces);
            return;
        }
        if (depth == 0) {
            for (int i = 0; i < spaces; i++) cout << " ";
            cout << node->data;
            for (int i = 0; i < spaces + 1; i++) cout << " ";
            return;
        }
        drawCurrentDepth(node->left, depth - 1, spaces);
        drawCurrentDepth(node->right, depth - 1, spaces);
    }

public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() { auxClear(root); }

    void insert(T value) {
        if (root == nullptr) {
            root = new Node(value);
            return;
        }
        auxInsert(root, value);
    }

    Node* search(T value) {
        Node* current = root;
        while (current != nullptr) {
            if (current->data == value) {
                return current;
            } else if (value < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }

    void remove(T value) {
        root = auxRemove(root, value);
    }

    void clear() {
        auxClear(root);
        root = nullptr;
    }

    int height() {
        return auxHeight(root);
    }

    int peso() const{
        return auxpeso(root);
    }

    void preOrderTraversal() const {
        preOrder(root);
    }
    bool balanced()  {
        return aux_balanced(root);
    }

    void inOrderTraversal() const {
        inOrder(root);
    }
    void postOrderTraversal() const {
        postOrder(root);
    }

    void draw() {
        for(int i = 0; i <= height(); i++) {
            int prod = 1<<(height() - i + 1);
            drawCurrentDepth(root, i, prod-1);
            cout << "\n";
        }
    }



};



int main() {
    BinaryTree<int> tree;

    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);
    tree.insert(8);

    cout << "Pre-order Traversal: ";
    tree.preOrderTraversal();
    cout << "\n";
    cout << "In-order Traversal: ";
    tree.inOrderTraversal();
    cout << "\n";
    cout << "Post-order Traversal: ";
    tree.postOrderTraversal();
    cout << "\n";
    cout << "Es balanceada: \n" << tree.balanced() <<endl;
    cout << "Peso: \n" << tree.peso() <<endl;
    tree.draw();

    cout << "insert (i)\nremove (r)\nsearch (s)\nheight (h)\ntraverse (t)\nbalanced (b)\nPeso (p)\nexit (e)\n";
    char option;
    while (cin >> option) {
        if (option == 'i') {
            int value;
            cin >> value;
            tree.insert(value);
            tree.draw();
        } else if (option == 'r') {
            int value;
            cin >> value;
            tree.remove(value);
            tree.draw();
        } else if (option == 's') {
            int value;
            cin >> value;
            if (tree.search(value) != nullptr) {
                cout << "Found\n";
            } else {
                cout << "Not found\n";
            }
        } else if (option == 'h') {
            cout << "Height: " << tree.height() << "\n";
        } else if (option == 'b') {
            cout << "esta balanceado?: " << tree.balanced() << "\n";
        } else if (option == 'p') {
            cout << "peso: " << tree.peso() << "\n";
        } else if (option == 't') {
            cout << "Pre-order Traversal: ";
            tree.preOrderTraversal();
            cout << "\n";
            cout << "In-order Traversal: ";
            tree.inOrderTraversal();
            cout << "\n";
            cout << "Post-order Traversal: ";
            tree.postOrderTraversal();
            cout << "\n";
        } else if (option == 'e') {
            break;
        }
        cout << "insert (i)\nremove (r)\nsearch (s)\nheight (h)\ntraverse (t)\nbalanced (b)\nPeso (p)\nexit (e)\n";
    }
    return 0;
}