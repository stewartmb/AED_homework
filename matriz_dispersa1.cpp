#include <iostream>
#include <vector>
using namespace std;

// Nodo para la matriz dispersa
template <class T>
class Node {
public:
    T data;
    int pos_row;
    int pos_col;
    Node<T>* next_row;
    Node<T>* next_col;

    Node(T value, int row, int col)
            : data(value), pos_row(row), pos_col(col), next_row(this), next_col(this) {}
};

// Clase Matriz Dispersa
template <class T>
class SparseMatrix {
private:
    vector<Node<T>*> rows;
    vector<Node<T>*> cols;
    int n_rows, n_cols;

public:
    // Constructor
    SparseMatrix(int rows, int cols) : n_rows(rows), n_cols(cols) {
        this->rows.resize(rows, nullptr);
        this->cols.resize(cols, nullptr);
    }

    // Destructor
    ~SparseMatrix() {
        for (int i = 0; i < n_rows; ++i) {
            if (rows[i]) {
                Node<T>* current = rows[i];
                Node<T>* start = rows[i];
                do {
                    Node<T>* temp = current;
                    current = current->next_col;
                    delete temp;
                } while (current != start);
            }
        }
    }

    // Insertar valor
    void insert(T value, int row, int col) {
        if (row >= n_rows || col >= n_cols) {
            throw out_of_range("Posición fuera de los límites.");
        }
        Node<T>* newNode = new Node<T>(value, row, col);

        // Insertar en la fila (circular)
        if (!rows[row]) {
            rows[row] = newNode;
            newNode->next_col = newNode; // Circular
        } else {
            Node<T>* temp = rows[row];
            Node<T>* prev = nullptr;
            do {
                if (temp->pos_col > col) break;
                prev = temp;
                temp = temp->next_col;
            } while (temp != rows[row]);

            if (!prev || temp == rows[row] && col < rows[row]->pos_col) {
                newNode->next_col = rows[row];
                rows[row] = newNode;
                if (prev) prev->next_col = newNode;
                Node<T>* last = rows[row];
                while (last->next_col != temp) last = last->next_col;
                last->next_col = rows[row];
            } else {
                newNode->next_col = temp;
                prev->next_col = newNode;
            }
        }

        // Insertar en la columna (circular)
        if (!cols[col]) {
            cols[col] = newNode;
            newNode->next_row = newNode; // Circular
        } else {
            Node<T>* temp = cols[col];
            Node<T>* prev = nullptr;
            do {
                if (temp->pos_row > row) break;
                prev = temp;
                temp = temp->next_row;
            } while (temp != cols[col]);

            if (!prev || temp == cols[col] && row < cols[col]->pos_row) {
                newNode->next_row = cols[col];
                cols[col] = newNode;
                if (prev) prev->next_row = newNode;
                Node<T>* last = cols[col];
                while (last->next_row != temp) last = last->next_row;
                last->next_row = cols[col];
            } else {
                newNode->next_row = temp;
                prev->next_row = newNode;
            }
        }
    }

    // Obtener un valor con sobrecarga
    T operator()(int row, int col) const {
        if (row >= n_rows || col >= n_cols) {
            throw out_of_range("Posición fuera de los límites.");
        }
        if (!rows[row]) return T();

        Node<T>* current = rows[row];
        do {
            if (current->pos_col == col) return current->data;
            current = current->next_col;
        } while (current != rows[row]);
        return T();
    }

    // Comparar dos matrices
    bool operator==(const SparseMatrix<T>& other) const {
        if (n_rows != other.n_rows || n_cols != other.n_cols) return false;

        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current1 = rows[i];
            Node<T>* current2 = other.rows[i];

            if ((!current1 && current2) || (current1 && !current2)) return false;

            if (current1) {
                do {
                    if (!current2 || current1->pos_col != current2->pos_col || current1->data != current2->data) {
                        return false;
                    }
                    current1 = current1->next_col;
                    current2 = current2->next_col;
                } while (current1 != rows[i]);
            }
        }
        return true;
    }

    // Sumar matrices
    SparseMatrix<T> operator+(const SparseMatrix<T>& other) const {
        if (n_rows != other.n_rows || n_cols != other.n_cols) {
            throw invalid_argument("Dimensiones no compatibles para la suma.");
        }

        SparseMatrix<T> result(n_rows, n_cols);
        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current1 = rows[i];
            Node<T>* current2 = other.rows[i];

            while (current1 || current2) {
                if (current1 && (!current2 || current1->pos_col < current2->pos_col)) {
                    result.insert(current1->data, current1->pos_row, current1->pos_col);
                    current1 = current1->next_col;
                } else if (current2 && (!current1 || current2->pos_col < current1->pos_col)) {
                    result.insert(current2->data, current2->pos_row, current2->pos_col);
                    current2 = current2->next_col;
                } else {
                    result.insert(current1->data + current2->data, current1->pos_row, current1->pos_col);
                    current1 = current1->next_col;
                    current2 = current2->next_col;
                }
            }
        }
        return result;
    }

    // Multiplicar matrices
    SparseMatrix<T> operator*(const SparseMatrix<T>& other) const {
        if (n_cols != other.n_rows) {
            throw invalid_argument("Dimensiones no compatibles para la multiplicación.");
        }

        SparseMatrix<T> result(n_rows, other.n_cols);

        for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < other.n_cols; ++j) {
                T sum = T();
                Node<T>* rowNode = rows[i];

                if (rowNode) {
                    do {
                        Node<T>* colNode = other.cols[j];
                        if (colNode) {
                            do {
                                if (colNode->pos_row == rowNode->pos_col) {
                                    sum += rowNode->data * colNode->data;
                                }
                                colNode = colNode->next_row;
                            } while (colNode != other.cols[j]);
                        }
                        rowNode = rowNode->next_col;
                    } while (rowNode != rows[i]);
                }

                if (sum != T()) {
                    result.insert(sum, i, j);
                }
            }
        }
        return result;
    }

    // Mostrar matriz
    void display() {
        for (int i = 0; i < n_rows; ++i) {
            Node<T>* temp = rows[i];
            for (int j = 0; j < n_cols; ++j) {
                if (temp && temp->pos_col == j) {
                    cout << temp->data << " ";
                    temp = temp->next_col;
                } else {
                    cout << "0 ";
                }
            }
            cout << endl;
        }
    }
};

int main() {
    SparseMatrix<int> sm1(3, 3);
    SparseMatrix<int> sm2(3, 3);

    sm1.insert(1, 0, 0);
    sm1.insert(4, 1, 2);
    sm1.insert(2, 1, 1);
    sm1.insert(3, 2, 2);

    sm2.insert(4, 0, 0);
    sm2.insert(5, 1, 1);
    sm2.insert(6, 2, 2);

    cout << "Matriz 1:" << endl;
    sm1.display();

    cout << "Matriz 2:" << endl;
    sm2.display();

    cout << "Suma de matrices:" << endl;
    SparseMatrix<int> sum = sm1 + sm2;
    sum.display();

    cout << "Producto de matrices:" << endl;
    SparseMatrix<int> product = sm1 * sm2;
    product.display();

    return 0;
}
