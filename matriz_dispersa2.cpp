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
            : data(value), pos_row(row), pos_col(col), next_row(nullptr), next_col(nullptr) {}
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
            Node<T>* current = rows[i];
            while (current) {
                Node<T>* temp = current;
                current = current->next_col;
                delete temp;
            }
        }
    }

    // Insertar valor
    void insert(T value, int row, int col) {
        if (row >= n_rows || col >= n_cols) {
            throw out_of_range("Posición fuera de los límites.");
        }
        Node<T>* newNode = new Node<T>(value, row, col);

        // Insertar en la fila
        if (!rows[row] || rows[row]->pos_col > col) {
            newNode->next_col = rows[row];
            rows[row] = newNode;
        } else {
            Node<T>* temp = rows[row];
            while (temp->next_col && temp->next_col->pos_col < col) {
                temp = temp->next_col;
            }
            newNode->next_col = temp->next_col;
            temp->next_col = newNode;
        }

        // Insertar en la columna
        if (!cols[col] || cols[col]->pos_row > row) {
            newNode->next_row = cols[col];
            cols[col] = newNode;
        } else {
            Node<T>* temp = cols[col];
            while (temp->next_row && temp->next_row->pos_row < row) {
                temp = temp->next_row;
            }
            newNode->next_row = temp->next_row;
            temp->next_row = newNode;
        }
    }

    // Eliminar un valor
    void remove(int row, int col) {
        if (row >= n_rows || col >= n_cols) return;

        // Eliminar de la fila
        Node<T>** current = &rows[row];
        while (*current && (*current)->pos_col != col) {
            current = &((*current)->next_col);
        }
        if (*current) {
            Node<T>* temp = *current;
            *current = (*current)->next_col;
            delete temp;
        }

        // Eliminar de la columna
        current = &cols[col];
        while (*current && (*current)->pos_row != row) {
            current = &((*current)->next_row);
        }
        if (*current) {
            Node<T>* temp = *current;
            *current = (*current)->next_row;
            delete temp;
        }
    }

    // Obtener un valor con sobrecarga
    T operator()(int row, int col) const {
        Node<T>* current = rows[row];
        while (current && current->pos_col != col) {
            current = current->next_col;
        }
        return current ? current->data : T();
    }

    // Comparar dos matrices
    bool operator==(const SparseMatrix<T>& other) const {
        if (n_rows != other.n_rows || n_cols != other.n_cols) return false;
        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current1 = rows[i];
            Node<T>* current2 = other.rows[i];
            while (current1 && current2) {
                if (current1->pos_col != current2->pos_col || current1->data != current2->data) {
                    return false;
                }
                current1 = current1->next_col;
                current2 = current2->next_col;
            }
            if (current1 || current2) return false;
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

    // Transponer matriz
    SparseMatrix<T> transpose() const {
        SparseMatrix<T> transposed(n_cols, n_rows);
        for (int i = 0; i < n_rows; ++i) {
            Node<T>* current = rows[i];
            while (current) {
                transposed.insert(current->data, current->pos_col, current->pos_row);
                current = current->next_col;
            }
        }
        return transposed;
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
                while (rowNode) {
                    Node<T>* colNode = other.cols[j];
                    while (colNode && colNode->pos_row < rowNode->pos_col) {
                        colNode = colNode->next_row;
                    }
                    if (colNode && colNode->pos_row == rowNode->pos_col) {
                        sum += rowNode->data * colNode->data;
                    }
                    rowNode = rowNode->next_col;
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
        for (int i = 0; i < n_rows; i++) {
            Node<T>* temp = rows[i];
            for (int j = 0; j < n_cols; j++) {
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
    sm1.display();
    sm2.display();

    SparseMatrix<int> sum = sm1 * sm2;
    sum.display();

    return 0;
}
