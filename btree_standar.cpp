#include <iostream>

using namespace std;

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
        // Inserción en el primer lugar disponible (primero el hijo izquierdo, luego el derecho)
        if (node->left == nullptr) {
            node->left = new Node(value);
        } else if (node->right == nullptr) {
            node->right = new Node(value);
        } else {
            // Si ambos hijos están ocupados, intentamos insertar recursivamente en el subárbol izquierdo
            auxInsert(node->left, value);
        }
    }

    Node* auxRemove(Node* current, T value) {
        if (current == nullptr) return current;
        // En un árbol binario normal, la lógica de búsqueda puede cambiar
        if (current->left != nullptr) {
            current->left = auxRemove(current->left, value);
        }
        if (current->right != nullptr) {
            current->right = auxRemove(current->right, value);
        }
        if (current->data == value) {
            // no children
            if (current->left == nullptr && current->right == nullptr) {
                delete current;
                return nullptr;
            }
                // one child
            else if (current->left == nullptr) {
                Node* temp = current->right;
                delete current;
                return temp;
            } else if (current->right == nullptr) {
                Node* temp = current->left;
                delete current;
                return temp;
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
            }
            current = current->left != nullptr ? current->left : current->right;
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

    void preOrderTraversal() const {
        preOrder(root);
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

    // Array con los elementos que queremos insertar en el árbol
    int elements[] = {10, 20, 30, 40, 50, 60, 70};
    int size = sizeof(elements) / sizeof(elements[0]);

    // Insertamos cada elemento del array en el árbol binario
    for (int i = 0; i < size; i++) {
        tree.insert(elements[i]);
    }

    // Mostramos los elementos del árbol en recorrido in-order
    cout << "Recorrido In-Order del árbol binario:" << endl;
    tree.inOrderTraversal();
    cout<<endl;

    tree.draw();

    return 0;
}

