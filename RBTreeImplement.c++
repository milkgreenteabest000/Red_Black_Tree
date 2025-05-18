/*
Built with love for data structures and competitive programming.
Join us at PDAO — we're hiring team members to run the next great contest!
*/

#include <iostream>
using namespace std;

//each node in the Red-Black Tree is either RED or BLACK
enum Color { RED, BLACK };

//all nodes records its data, color, left node, right node, parent node
struct Node{
    Color color;
    int data;
    Node* left;
    Node* right;
    Node* parent;
    // Constructor: new nodes are red by default, and their children are initially nullptr.
    Node(const int newData, Node* newParent = nullptr) 
    : color(RED), data(newData), left(nullptr), right(nullptr), parent(newParent) {}
};

/*
Red-Black Tree Rule
1.the color of each node is either red or black
2.the color of the root is always black
3.the color of the leaves(nullptr) are considered black
4.no two consecutive red nodes are allowed (red nodes should not have red child)
5.Every path from a node to its descendant leaves must contain the same number of black nodes
*/
class RBTree{
    private:
        //root of the tree
        Node* root_;

        //some operation make the insert and remove function easier
        void rightRotation(Node* node);        //right rotations used during balancing
        void leftRotation(Node* node);         //left rotations used during balancing
        void replace(Node* a, Node* b);         //replace node 'a' with node 'b' in the tree structure (but not deleting a)
        Node* findSuccessor(Node* node);        //find the in-order successor (minimum in right subtree)

        //fix some problem and remain the property of BRTree, when we insert or remove nodes
        void fixInsert(Node* node);
        void fixRemove(Node* node, Node* parent);

        //recursive helpers for cleanup, printing, dfs to check the number of black nodes
        void clearTree(Node* node);
        void printTreeInorder(Node* node) const;
        void printTreePreorder(Node* node) const;
        int dfsCheckBRTree(Node* node, bool& isViolated) const;

    public:
        //constructor, destructor, print, checkBRTree interface
        RBTree();
        ~RBTree();
        void print() const;
        bool isValidRBTree() const;

        //insert, remove interface and implement
        void insert(const int newData);
        void remove(const int newData);
};

//some simple test
int main(){
    int input;
    cin >> input;

    RBTree tree;
    tree.isValidRBTree();
    for(int i = 0; i < 10; i++){
        cout << "insert: " << i * 10 << endl;
        tree.insert(i * 10);
        tree.print();
        tree.isValidRBTree();
    }
    for(int i = 5; i >= 0; i--){
        cout << "remove: " << i * 10 << endl;
        tree.remove(i * 10);
        tree.print();
        tree.isValidRBTree();
    }
    cout << "remove: " << 80 << endl;
    tree.remove(80);
    tree.print();
    tree.isValidRBTree();
    cout << "remove: " << 90 << endl;
    tree.remove(90);
    tree.print();
    tree.isValidRBTree();
    cout << "remove: " << 70 << endl;
    tree.remove(70);
    tree.print();
    tree.isValidRBTree();
    cout << "remove: " << 60 << endl;
    tree.remove(60);
    tree.print();
    tree.isValidRBTree();
    return 0;
}

//just set the root initial condition, nullptr
RBTree::RBTree() : root_(nullptr) {}

//use clear function to destructor the whole tree
RBTree::~RBTree(){
    clearTree(this->root_);
    this->root_ = nullptr;
}

//print the entire tree using pre-order traversal
void RBTree::print() const {printTreePreorder(this->root_);}

//check if the tree BRTree
bool RBTree::isValidRBTree() const {
    if (!root_) return true;  //nullptr considered true

    //root should be black
    if (root_->color != BLACK) return false;

    bool isViolated = 0;
    dfsCheckBRTree(this->root_, isViolated);

    if(!isViolated) return true;

    for(int i = 0; i < 10; i++){
        cout << "NOOOOOOOOOOOOOOOO" << endl;
    }
    return false;
}

//dfs to check if the tree BRTree
int RBTree::dfsCheckBRTree(Node* node, bool& isViolated) const{
    //define the black-height of empty tree is one
    if(node == nullptr) return 1;

    //check if consecutive red nodes
    if(node && node->color == RED){
        if(node->right && node->right->color == RED) isViolated = 1;
        if(node->left && node->left->color == RED) isViolated = 1;
    }

    if(isViolated) return -1;

    //check if the two path has different numbers of nodes
    int leftHeight = dfsCheckBRTree(node->left, isViolated);
    int rightHeight = dfsCheckBRTree(node->right, isViolated);

    if(isViolated || leftHeight != rightHeight){
        isViolated = 1; 
        return -1;
    }

    return ( ((node->color == BLACK) ? 1 : 0) + leftHeight );
}

//in-order traversal: left -> node -> right
void RBTree::printTreeInorder(Node* node) const{
    //if the node dose not exist, return directly
    if(node == nullptr) return;
    
    //if we can traverse to left, then first to left
    if(node->left) printTreeInorder(node->left);

    //print node's data and color, left(if it exist)'s data and color, right(if it exist)'s data and color
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

    //last, traverse to right
    if(node->right) printTreeInorder(node->right);
}

//pre-order traversal: node -> left -> right
void RBTree::printTreePreorder(Node* node) const{
    //if the node dose not exist, return directly
    if(node == nullptr) return;

    //print node's data and color, left(if it exist)'s data and color, right(if it exist)'s data and color
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

    //after print node, then traverse to left
    if(node->left) printTreePreorder(node->left);

    //after print node, then traverse to right
    if(node->right) printTreePreorder(node->right);
}

//post-order to delete nodes: left -> right -> node
void RBTree::clearTree(Node* node){
    //if the node does not exist, return directly
    if(node == nullptr) return;

    //traverse to left
    clearTree(node->left);

    //traverse to right
    clearTree(node->right);

    //delete the node itself
    delete node;
}

//perform a right rotation on the given node
void RBTree::rightRotation(Node* node){
    //if the node or its left child does not exist, return directly
    if(!node || !node->left) return;

    Node* child = node->left;

    //step 1: move child's right subtree to node's left
    node->left = child->right;
    if(child->right) child->right->parent = node;

    //step 2: adjust child's parent pointer
    replace(node, child);

    //step 3: adjust the relation between node and child
    child->right = node;
    node->parent = child;
}

//perform a left rotation on the given node
void RBTree::leftRotation(Node* node){
    //if the node or its right child does not exist, return directly
    if(!node || !node->right) return;

    Node* child = node->right;

    //step 1: move child's left subtree to node's right
    node->right = child->left;
    if(child->left) child->left->parent = node;

    //step 2: adjust child's parent pointer
    replace(node, child);

    //step 3: adjust the relation between node and child
    child->left = node;
    node->parent = child;
}

/*
replace node 'a' with node 'b' in the tree structure (but not deleting a)
ONLY adjust parent-child linkage
DOES NOT adjust the information about a and b's children here
*/
void RBTree::replace(Node* a, Node* b){
    //adjust information of a's parent
    if(!a->parent) this->root_ = b;         //no parent -> node is root -> adjust root
    else if(a->parent->left == a) a->parent->left = b;
    else a->parent->right = b;

    //adjust information of b
    if(b) b->parent = a->parent;
}

/*
return the node with the minimum value in the subtree rooted at 'node'
(i.e., the leftmost node in the subtree)
*/
Node* RBTree::findSuccessor(Node* node){
    //keep searching in left subtree
    Node* prev = nullptr;
    while(node != nullptr){
        prev = node;
        node = node->left;
    }

    return prev;
}

/*
fix violations of Red-Black Tree properties caused by insertion.

we only need to consider the two consecutive red nodes
case 1, uncle node is also red -> only need to adjust the color
case 2, uncle node is black & node is in LR or RL path of grandparent -> rotation to make it LL or RR(case 3)
case 3, uncle node is black & node is in LL or RR path of grandparent -> adjust the color + rotation
*/
void RBTree::fixInsert(Node* node){  

    //node is red -> node's parent exist -> check if node and node's parent are consecutive red nodes
    while(node != this->root_ && node && node->color == RED && node->parent->color == RED){ 

        Node* parent = node->parent;
        Node* grandparent = parent->parent;         //grand parent should exist, because node->parent is red

        //case: node is at the left side of grandparent
        if(grandparent->left == parent){
            Node* uncle = grandparent->right;       //uncle is in the right side of grandparent

            //case 1, uncle is red
            if(uncle && uncle->color == RED){
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;

                //up to check grandparent
                node = grandparent;
                continue;
            }

            //check if case 2, node in LR of grandparent
            if(parent->right == node){
                leftRotation(parent);
                node = parent;
                parent = node->parent;
            }
            
            //deal with case 3, node in LL of grandparent
            grandparent->color = RED;
            parent->color = BLACK;
            rightRotation(grandparent);

            //up to check parent
            node = parent;
        }
        //case: node is at the right side of grandparent
        else{
            Node* uncle = grandparent->left;       //uncle is in the left side of grandparent

            //case 1, uncle is red
            if(uncle && uncle->color == RED){
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;

                //up to check grandparent
                node = grandparent;
                continue;
            }

            //check if case 2, node in RL of grandparent
            if(parent->left == node){
                rightRotation(parent);
                node = parent;
                parent = node->parent;
            }
            
            //deal with case 3, node in RR of grandparent
            grandparent->color = RED;
            parent->color = BLACK;
            leftRotation(grandparent);

            //up to check parent
            node = parent;
        }
    }

    //remain the root black, directly adjusting root to black won't violate anything
    if(this->root_) this->root_->color = BLACK;

}

//insert the new node just like BST, remain RBTree properties additionally
void RBTree::insert(const int newData){
    //traverse to find the correct insertion point
    Node* node = this->root_;
    Node* prev = nullptr;
    while(node != nullptr){
        prev = node;

        if(newData > node->data) node = node->right;       //turn right
        else if(newData < node->data) node = node->left;        //turn left
        else return;        //return due to duplication
    }

    //generate new node with necessary information
    Node* newNode = new Node(newData, prev);

    //adjust information of prev
    if(prev == nullptr) this->root_ = newNode;
    else if(newNode->data > prev->data) prev->right = newNode;
    else prev->left = newNode;

    //remain RBTree properties
    fixInsert(newNode);
}

/*
fix violations of Red-Black Tree properties caused by removal.

we only need to consider we remove black node, 
because that means we decrease the number of black nodes in one subtree, 
which violate the properties of RBTree

case 1, sibling node is red -> adjust the color + rotation -> adjust sibling and continue to case 234 
case 2, sibling node is black & its left and right children are black -> adjust the color -> to deal with upper layer of tree
case 3, (sibling in parent's right and siblingLeft is red) or (sibling in parent's left and siblingRight is red)
        -> adjust the color + rotation to case4 -> adjust sibling and continue to case 4
case 4, (sibling in parent's right and siblingRight is red) or (sibling in parent's left and siblingLeft is red)
        -> adjust the color + rotation -> complete and jump out the loop
Last, set the node black wherever the node is
*/
void RBTree::fixRemove(Node* node, Node* parent){

    while((!node || node->color == BLACK) && node != root_){       //loop when the node's color is black
        //case: node is in the left side of parent
        if(parent->left == node){
            Node* sibling = parent->right;      //sibling is in the right side of parent
            
            //case 1, sibling is red
            if(sibling && sibling->color == RED){
                sibling->color = BLACK;
                parent->color = RED;
                leftRotation(parent);

                sibling = parent->right;
            }

            //we aren't sure whether sibling exist
            Node* siblingLeft = (sibling) ? sibling->left : nullptr;
            Node* siblingRight = (sibling) ? sibling->right : nullptr;

            //case 2, sibling is black and both of its children are black
            if((!siblingLeft || siblingLeft->color == BLACK) 
            && (!siblingRight || siblingRight->color == BLACK)){
                if(sibling) sibling->color = RED;

                node = parent;
                parent = node->parent;
                continue;
            }

            //case 3, sibling is black and its left child is red
            if(siblingLeft && siblingLeft->color == RED){
                if(siblingLeft) siblingLeft->color = BLACK;
                if(sibling) sibling->color = RED;
                rightRotation(sibling);
                sibling = parent->right;
            }
            
            //sibling may be changed by front case
            siblingLeft = (sibling) ? sibling->left : nullptr;
            siblingRight = (sibling) ? sibling->right : nullptr;

            //csae 4, sibling is black and its right child is red
            if(sibling) sibling->color = parent->color;
            parent->color = BLACK;
            if(siblingRight) siblingRight->color = BLACK;
            leftRotation(parent);
            node = this->root_;
        }
        //case: node is in the right side of parent
        else{
            Node* sibling = parent->left;      //sibling is in the left side of parent
            
            //case 1, sibling is red
            if(sibling && sibling->color == RED){
                if(sibling) sibling->color = BLACK;
                parent->color = RED;
                rightRotation(parent);

                sibling = parent->right;
            }

            //we aren't sure whether sibling exist
            Node* siblingLeft = (sibling) ? sibling->left : nullptr;
            Node* siblingRight = (sibling) ? sibling->right : nullptr;

            //case 2, sibling is black and both of its children are black
            if((!siblingLeft || siblingLeft->color == BLACK) 
            && (!siblingRight || siblingRight->color == BLACK)){
                if(sibling) sibling->color = RED;

                node = parent;
                parent = node->parent;
                continue;
            }

            //case 3, sibling is black and its right child is red
            if(siblingRight && siblingRight->color == RED){
                if(siblingRight) siblingRight->color = BLACK;
                if(sibling) sibling->color = RED;
                leftRotation(sibling);
                sibling = parent->left;
            }

            //sibling may be changed by front case
            siblingLeft = (sibling) ? sibling->left : nullptr;
            siblingRight = (sibling) ? sibling->right : nullptr;

            //case 4, sibling is black and its left child is red
            if(sibling) sibling->color = parent->color;
            parent->color = BLACK;
            if(siblingLeft) siblingLeft->color = BLACK;
            rightRotation(parent);
            node = this->root_;
        }
    }

    if(node) node->color = BLACK;
}

/*
    delete the node just like BST
        case 1, the node has no children
        case 2, the node has only left child
        case 3, the node has only right child
        case 4, the node has both children

    z is the node to be delete originally
    y is the node to be delete actually(happen in case 4)
    x is the node to replace the position which is deleted
    xParent is x's parent after deleting node
*/
void RBTree::remove(const int newData){
    //traverse to find the correct deleting position
    Node* z = this->root_;
    while(z != nullptr){
        if(newData > z->data) z = z->right;
        else if(newData < z->data) z = z->left;
        else break;
    }

    //if data not exist
    if(z == nullptr) return;

    Node* x = nullptr;
    Node* xParent = nullptr;
    Node* y = z;

    Color yOriginalColor = y->color;

    //case 1, no child
    if(!z->left && !z->right){
        if(z->parent == nullptr) this->root_ = nullptr;      //no parent -> root
        else if(z->parent->left == z) z->parent->left = nullptr;   //left child
        else z->parent->right = nullptr;     //right child

        xParent = z->parent;
    }
    //case 2, only has left child
    else if(!z->right){
        x = z->left;
        xParent = z->parent;
        replace(z, z->left);
    }
    //case 3, only has right child
    else if(!z->left){
        x = z->right;
        xParent = z->parent;
        replace(z, z->right);
    }
    //case 4, has both children
    else{
        //first find the successor, the minimum in right subtree
        y = findSuccessor(z->right);
        x = y->right;
        yOriginalColor = y->color;

        //adjust successor from successor's right
        if(y == z->right){      //y is z's right -> adjust x's parent -> the left adjustment will be done later
            if(x) x->parent = y;
            xParent = y;
        }
        else{       //other situation -> x should replace y
            xParent = y->parent;

            replace(y, x);
            
            //adjust the connection between z & z's right
            z->right->parent = y;
            y->right = z->right;

        }

        //replace node from successor
        replace(z, y);

        //adjust the connection between z & z's left
        z->left->parent = y;
        y->left = z->left;
        y->color = z->color;
    }

    delete z;

    //remain the properties of RBTree
    if(yOriginalColor == BLACK){
        fixRemove(x, xParent);
    }

}
