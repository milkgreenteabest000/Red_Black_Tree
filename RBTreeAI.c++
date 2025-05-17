#include <iostream>
#include <queue>
#include <iomanip>
using namespace std;

enum Color { RED, BLACK };

struct Node {
    int data;
    bool color;
    Node *left, *right, *parent;

    Node(int data): data(data), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBTree {
private:
    Node* root;

    void rotateLeft(Node*& node) {
        Node* rightChild = node->right;
        node->right = rightChild->left;
        if (rightChild->left != nullptr)
            rightChild->left->parent = node;
        rightChild->parent = node->parent;

        if (node->parent == nullptr)
            root = rightChild;
        else if (node == node->parent->left)
            node->parent->left = rightChild;
        else
            node->parent->right = rightChild;

        rightChild->left = node;
        node->parent = rightChild;
    }

    void rotateRight(Node*& node) {
        Node* leftChild = node->left;
        node->left = leftChild->right;
        if (leftChild->right != nullptr)
            leftChild->right->parent = node;
        leftChild->parent = node->parent;

        if (node->parent == nullptr)
            root = leftChild;
        else if (node == node->parent->right)
            node->parent->right = leftChild;
        else
            node->parent->left = leftChild;

        leftChild->right = node;
        node->parent = leftChild;
    }

    void fixInsert(Node*& node) {
        Node* parent = nullptr;
        Node* grandparent = nullptr;

        while (node != root && node->color == RED && node->parent->color == RED) {
            parent = node->parent;
            grandparent = parent->parent;

            // case A: parent is left child
            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                if (uncle != nullptr && uncle->color == RED) {
                    // Case 1: uncle is RED → recolor
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                }
                else {
                    // Case 2: uncle is BLACK
                    if (node == parent->right) {
                        rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
            else {
                // case B: parent is right child
                Node* uncle = grandparent->left;

                if (uncle != nullptr && uncle->color == RED) {
                    // Mirror Case 1
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                }
                else {
                    if (node == parent->left) {
                        rotateRight(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateLeft(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        root->color = BLACK;
    }

    void fixDelete(Node* node) {
        if (node == nullptr) return;

        Node* sibling = nullptr;
        while (node != root && node->color == BLACK) {
            if (node == node->parent->left) {
                sibling = node->parent->right;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateLeft(node->parent);
                    sibling = node->parent->right;
                }
                if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                    (sibling->right == nullptr || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                } else {
                    if (sibling->right == nullptr || sibling->right->color == BLACK) {
                        if (sibling->left)
                            sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = node->parent->right;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->right)
                        sibling->right->color = BLACK;
                    rotateLeft(node->parent);
                    node = root;
                }
            }
            else {
                sibling = node->parent->left;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateRight(node->parent);
                    sibling = node->parent->left;
                }
                if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                    (sibling->right == nullptr || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                } else {
                    if (sibling->left == nullptr || sibling->left->color == BLACK) {
                        if (sibling->right)
                            sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->left)
                        sibling->left->color = BLACK;
                    rotateRight(node->parent);
                    node = root;
                }
            }
        }
        if (node) node->color = BLACK;
    }

    void transplant(Node* u, Node* v) {
        if (u->parent == nullptr) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v != nullptr) v->parent = u->parent;
    }

    Node* minValue(Node* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    void deleteNode(Node* node, int key) {
        Node* z = root;
        Node* x, *y;
        while (z != nullptr) {
            if (z->data == key) break;
            if (key < z->data) z = z->left;
            else z = z->right;
        }

        if (z == nullptr) return;

        y = z;
        bool yOriginalColor = y->color;
        if (z->left == nullptr) {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == nullptr) {
            x = z->left;
            transplant(z, z->left);
        }
        else {
            y = minValue(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                if (x) x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yOriginalColor == BLACK)
            fixDelete(x);
    }

    void clear(Node* node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    void printTree(Node* root, string indent, bool last) const {
        if (root != nullptr) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "     ";
            } else {
                cout << "L----";
                indent += "|    ";
            }

            string color = root->color ? "RED" : "BLACK";
            cout << root->data << "(" << color << ")" << endl;
            printTree(root->left, indent, false);
            printTree(root->right, indent, true);
        }
    }

    void printTreePreorder(Node* node) const;
public:
    RBTree() : root(nullptr) {}
    ~RBTree() { clear(root); }

    void insert(int data) {
        Node* y = nullptr;
        Node* x = root;

        while (x != nullptr) {
            y = x;
            if (data < x->data)
                x = x->left;
            else if(data > x->data)
                x = x->right;
            else 
                return;
        }

        Node* node = new Node(data);

        node->parent = y;
        if (y == nullptr)
            root = node;
        else if (node->data < y->data)
            y->left = node;
        else
            y->right = node;

        fixInsert(node);
    }

    void remove(int data) {
        deleteNode(root, data);
    }

    void print() const {
        printTreePreorder(this->root);
    }
};

int main(){
    int input;
    cin >> input;

    RBTree tree;
    for(int i = 0; i < 10; i++){
        cout << i << "************" << endl;
        tree.insert(i * 10);
        tree.print();
    }
    for(int i = 5; i >= 0; i--){
        cout << i << "^^^^^^^^^^^^" << endl;
        tree.remove(i * 10);
        tree.print();
    }

    return 0;
}

void RBTree::printTreePreorder(Node* node) const{
    //if the node dose not exist, return directly
    if(node == nullptr) return;

    //print node itself, left(if it exist), right(if it exist)
    cout << "node: " << node->data << ((node->color == RED) ? "R" : "B") << " ";
    if(node->left){
        cout << "left: " << node->left->data << ((node->left->color == RED) ? "R" : "B") << " ";
    }
    else{
        cout << "left: " << "nullptr" << " ";
    }
    if(node->right){
        cout << "right: " << node->right->data << ((node->right->color == RED) ? "R" : "B") << " ";
    }
    else{
        cout << "right: " << "nullptr" << " ";
    }
    cout << endl;

    //if left node exist, print it, then traverse
    if(node->left) printTreePreorder(node->left);

    //if right node exist, print it, then traverse
    if(node->right) printTreePreorder(node->right);
}

