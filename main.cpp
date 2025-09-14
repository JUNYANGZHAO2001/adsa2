#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key;
    Node *left, *right;
    int height;
    Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

class AVL {
public:
    int height(Node* n) {
        return n ? n->height : 0;
    }

    int getBalance(Node* n) {
        return n ? height(n->left) - height(n->right) : 0;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    Node* insert(Node* root, int key) {
        if (!root) return new Node(key);

        if (key < root->key)
            root->left = insert(root->left, key);
        else if (key > root->key)
            root->right = insert(root->right, key);
        else
            return root; // duplicate

        root->height = 1 + max(height(root->left), height(root->right));
        int balance = getBalance(root);

        // LL
        if (balance > 1 && key < root->left->key)
            return rightRotate(root);
        // RR
        if (balance < -1 && key > root->right->key)
            return leftRotate(root);
        // LR
        if (balance > 1 && key > root->left->key) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        // RL
        if (balance < -1 && key < root->right->key) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    Node* maxValueNode(Node* node) {
        Node* current = node;
        while (current->right)
            current = current->right;
        return current;
    }

    Node* deleteNode(Node* root, int key) {
        if (!root) return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            // Node with only one child or no child
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {
                // Node with two children: use **in-order predecessor**
                Node* temp = maxValueNode(root->left);
                root->key = temp->key;
                root->left = deleteNode(root->left, temp->key);
            }
        }

        if (!root) return root;

        root->height = 1 + max(height(root->left), height(root->right));
        int balance = getBalance(root);

        // LL
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);
        // LR
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        // RR
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);
        // RL
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    void preOrder(Node* root, vector<int> &res) {
        if (root) {
            res.push_back(root->key);
            preOrder(root->left, res);
            preOrder(root->right, res);
        }
    }

    void inOrder(Node* root, vector<int> &res) {
        if (root) {
            inOrder(root->left, res);
            res.push_back(root->key);
            inOrder(root->right, res);
        }
    }

    void postOrder(Node* root, vector<int> &res) {
        if (root) {
            postOrder(root->left, res);
            postOrder(root->right, res);
            res.push_back(root->key);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    getline(cin, line);
    stringstream ss(line);
    vector<string> moves;
    string op;
    while (ss >> op) moves.push_back(op);

    AVL tree;
    Node* root = nullptr;

    for (size_t i = 0; i + 1 < moves.size(); i++) {
        string m = moves[i];
        if (m[0] == 'A') {
            int val = stoi(m.substr(1));
            root = tree.insert(root, val);
        } else if (m[0] == 'D') {
            int val = stoi(m.substr(1));
            root = tree.deleteNode(root, val);
        }
    }

    vector<int> res;
    string last = moves.back();
    if (last == "PRE") tree.preOrder(root, res);
    else if (last == "IN") tree.inOrder(root, res);
    else if (last == "POST") tree.postOrder(root, res);

    if (res.empty()) cout << "EMPTY\n";
    else {
        for (size_t i = 0; i < res.size(); i++) {
            if (i) cout << ' ';
            cout << res[i];
        }
        cout << '\n';
    }

    return 0;
}
