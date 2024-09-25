struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

#include <iostream>

using namespace std;


template <typename T>
class BinaryTree {
public:
    TreeNode* root;

    template <typename key_type, typename value_type>
    struct Pair {
        key_type key;
        value_type value;

        Pair() : key(key_type()), value(value_type()) {}

        Pair(const key_type &key, const value_type &value) : key(key), value(value) {}

        void setValue(const value_type &newValue) {
            value = newValue;
        }

        bool operator==(const Pair &other) const {
            return (key == other.key) && (value == other.value);
        }

        bool operator!=(const Pair &other) const {
            return !(*this == other);
        }
    };

    void auxInsert(TreeNode* node, T value) {
        if (value < node->val) {
            if (node->left == nullptr) {
                node->left = new TreeNode(value);
            } else {
                auxInsert(node->left, value);
            }
        } else if (value > node->val) {
            if (node->right == nullptr) {
                node->right = new TreeNode(value);
            } else {
                auxInsert(node->right, value);
            }
        }
    }

    TreeNode *auxRemove(TreeNode *current, T value) {
        if (current == nullptr)
            return current;
        // search
        if (value < current->val) {

            current->left = auxRemove(current->left, value);
        } else if (value > current->val) {
            current->right = auxRemove(current->right, value);
        }
            // current->val == value
        else {
            // no children
            if (current->left == nullptr && current->right == nullptr) {
                delete current;
                return nullptr;
            }
                // one child
            else if (current->left == nullptr) {
                TreeNode *temp = current->right;
                delete current;
                return temp;
            } else if (current->right == nullptr) {
                TreeNode *temp = current->left;
                delete current;
                return temp;
            }
                // two children
            else {
                TreeNode *temp_parent = current;
                TreeNode *temp = current->left;
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

    void auxClear(TreeNode* node) {
        if (node != nullptr) {
            auxClear(node->left);
            auxClear(node->right);
            delete node;
        }
    }

    int auxHeight(TreeNode* node) {
        if (node == nullptr) return -1;
        int left = auxHeight(node->left);
        int right = auxHeight(node->right);
        return 1 + max(left, right);
    }

    int auxpeso(TreeNode* node) const{
        if (node == nullptr) return 0;
        int left = auxpeso(node->left);
        int right = auxpeso(node->right);
        return left +right+1;
    }

    void preOrder(TreeNode* node) const {
        if (node == nullptr) return;
        cout << node->val << " ";
        preOrder(node->left);
        preOrder(node->right);
    }

    void inOrder(TreeNode* node) const {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->val << " ";
        inOrder(node->right);
    }



    void postOrder(TreeNode* node) const {
        if (node == nullptr) return;
        postOrder(node->left);
        postOrder(node->right);
        cout << node->val << " ";
    }

    Pair<TreeNode*, int> auxproblema(TreeNode* node) {
        if (node == nullptr) return {node, 0};
        if (node->left == nullptr && node->right  == nullptr){
            return {node, auxHeight(node)};
        }
        if (node->left == nullptr){
            return auxproblema(node->right);
        }
        if (node->right == nullptr){
            return auxproblema(node->left);
        }
        Pair bleft = auxproblema(node->left);
        Pair bright = auxproblema(node->right);
        if (bleft.value > bright.value ){
            cout << "a" << bleft.key->val << node->val << endl;
            return bleft;
        }
        else if(bleft.value < bright.value ){
            cout << "a" << bright.key->val << node->val << endl;
            return bright;
        }
        else if(bleft.value == bright.value ){
            cout << bleft.key->val << bleft.value << "-"<< bleft.key->val << bleft.value << endl;
            return bright;
        }
        return bleft;
    }

//    int auxproblema(TreeNode* node){
//        if (node == nullptr) return auxHeight(node);
//        else if (auxHeight(node)>auxHeight(node))
//            return false;
//        int bleft = auxproblema(node->left);
//        int bright = auxproblema(node->right);
//        return ;
//    }

    bool aux_balanced(TreeNode* node) {
        if (node == nullptr) return true;
        else if (abs(auxHeight(node->right) - auxHeight(node->left))>1)
            return false;
        bool bleft = aux_balanced(node->left);
        bool bright = aux_balanced(node->right);
        return bleft*bright;
    }

    void drawCurrentDepth(TreeNode* node, int depth, int spaces) {
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
            cout << node->val;
            for (int i = 0; i < spaces + 1; i++) cout << " ";
            return;
        }
        drawCurrentDepth(node->left, depth - 1, spaces);
        drawCurrentDepth(node->right, depth - 1, spaces);
    }

    BinaryTree() : root(nullptr) {}
    ~BinaryTree() { auxClear(root); }

    void insert(T value) {
        if (root == nullptr) {
            root = new TreeNode(value);
            return;
        }
        auxInsert(root, value);
    }

    TreeNode* search(T value) {
        TreeNode* current = root;
        while (current != nullptr) {
            if (current->val == value) {
                return current;
            } else if (value < current->val) {
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
    int problema() {
        return auxproblema(root).key->val;
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



class Solution {
public:
    int findBottomLeftValue(TreeNode* root) {
        BinaryTree<int> tree;
        return tree.auxproblema(root).key->val;
    }
};