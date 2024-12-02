#include <iostream>
using namespace std;

// A BTree Node
class BTreeNode {
    int *keys;          // Array of keys
    int t;              // Minimum degree
    BTreeNode **C;      // Array of child pointers
    int n;              // Current number of keys
    bool leaf;          // True if the node is a leaf

public:
    BTreeNode(int _t, bool _leaf);

    // Traverse the tree
    void traverse();

    // Search a key in the subtree rooted at this node
    BTreeNode *search(int k);

    // Insert a new key into a non-full node
    void insertNonFull(int k);

    // Split the child y of this node
    void splitChild(int i, BTreeNode *y);

    // Remove the key k from the subtree rooted at this node
    void remove(int k);

    // Helper function to find the index of the first key >= k
    int findKey(int k);

    // Remove a key from a leaf node
    void removeFromLeaf(int idx);

    // Remove a key from a non-leaf node
    void removeFromNonLeaf(int idx);

    // Get the predecessor of the key
    int getPredecessor(int idx);

    // Get the successor of the key
    int getSuccessor(int idx);

    // Fill the child C[idx] if it has fewer than t-1 keys
    void fill(int idx);

    // Borrow a key from the previous child
    void borrowFromPrev(int idx);

    // Borrow a key from the next child
    void borrowFromNext(int idx);

    // Merge C[idx] with C[idx+1]
    void merge(int idx);

    friend class BTree;
};

// A BTree
class BTree {
    BTreeNode *root; // Pointer to the root node
    int t;           // Minimum degree

public:
    BTree(int _t) {
        root = nullptr;
        t = _t;
    }

    // Traverse the tree
    void traverse() {
        if (root != nullptr)
            root->traverse();
    }

    // Search a key in the tree
    BTreeNode *search(int k) {
        return (root == nullptr) ? nullptr : root->search(k);
    }

    // Insert a new key into the B-Tree
    void insert(int k);

    // Remove a key from the B-Tree
    void remove(int k);
};

// Constructor for BTreeNode
BTreeNode::BTreeNode(int _t, bool _leaf) {
    t = _t;
    leaf = _leaf;

    // Allocate memory for keys and child pointers
    keys = new int[2 * t - 1];
    C = new BTreeNode *[2 * t];

    n = 0;
}

// Function to traverse all nodes in a subtree rooted at this node
void BTreeNode::traverse() {
    int i;
    for (i = 0; i < n; i++) {
        if (!leaf)
            C[i]->traverse();
        cout << " " << keys[i];
    }

    if (!leaf)
        C[i]->traverse();
}

// Function to search a key in the subtree rooted at this node
BTreeNode *BTreeNode::search(int k) {
    int i = 0;
    while (i < n && k > keys[i])
        i++;

    if (keys[i] == k)
        return this;

    if (leaf)
        return nullptr;

    return C[i]->search(k);
}

// Function to insert a new key into the B-Tree
void BTree::insert(int k) {
    if (root == nullptr) {
        root = new BTreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    } else {
        if (root->n == 2 * t - 1) {
            BTreeNode *s = new BTreeNode(t, false);
            s->C[0] = root;
            s->splitChild(0, root);

            int i = (s->keys[0] < k) ? 1 : 0;
            s->C[i]->insertNonFull(k);

            root = s;
        } else {
            root->insertNonFull(k);
        }
    }
}

// Function to insert a key into a non-full node
void BTreeNode::insertNonFull(int k) {
    int i = n - 1;

    if (leaf) {
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        n = n + 1;
    } else {
        while (i >= 0 && keys[i] > k)
            i--;

        if (C[i + 1]->n == 2 * t - 1) {
            splitChild(i + 1, C[i + 1]);
            if (keys[i + 1] < k)
                i++;
        }
        C[i + 1]->insertNonFull(k);
    }
}

// Function to split the child y of this node
void BTreeNode::splitChild(int i, BTreeNode *y) {
    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    if (!y->leaf) {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }

    y->n = t - 1;

    for (int j = n; j >= i + 1; j--)
        C[j + 1] = C[j];

    C[i + 1] = z;

    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = y->keys[t - 1];
    n = n + 1;
}

// Function to remove a key from the B-Tree
void BTree::remove(int k) {
    if (!root) {
        cout << "The tree is empty\n";
        return;
    }

    root->remove(k);

    if (root->n == 0) {
        BTreeNode *tmp = root;
        root = (root->leaf) ? nullptr : root->C[0];
        delete tmp;
    }
}

// Function to find the index of the first key >= k
int BTreeNode::findKey(int k) {
    int idx = 0;
    while (idx < n && keys[idx] < k)
        ++idx;
    return idx;
}

// Function to remove a key from the subtree rooted at this node
void BTreeNode::remove(int k) {
    int idx = findKey(k);

    if (idx < n && keys[idx] == k) {
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    } else {
        if (leaf) {
            cout << "The key " << k << " is not in the tree\n";
            return;
        }

        bool flag = (idx == n);
        if (C[idx]->n < t)
            fill(idx);

        if (flag && idx > n)
            C[idx - 1]->remove(k);
        else
            C[idx]->remove(k);
    }
}

// Remove a key from a leaf node
void BTreeNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];
    n--;
}

// Remove a key from a non-leaf node
void BTreeNode::removeFromNonLeaf(int idx) {
    int k = keys[idx];

    if (C[idx]->n >= t) {
        int pred = getPredecessor(idx);
        keys[idx] = pred;
        C[idx]->remove(pred);
    } else if (C[idx + 1]->n >= t) {
        int succ = getSuccessor(idx);
        keys[idx] = succ;
        C[idx + 1]->remove(succ);
    } else {
        merge(idx);
        C[idx]->remove(k);
    }
}

// Get the predecessor of the key
int BTreeNode::getPredecessor(int idx) {
    BTreeNode *cur = C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];
    return cur->keys[cur->n - 1];
}

// Get the successor of the key
int BTreeNode::getSuccessor(int idx) {
    BTreeNode *cur = C[idx + 1];
    while (!cur->leaf)
        cur = cur->C[0];
    return cur->keys[0];
}

// Fill the child C[idx] if it has fewer than t-1 keys
void BTreeNode::fill(int idx) {
    if (idx != 0 && C[idx - 1]->n >= t)
        borrowFromPrev(idx);
    else if (idx != n && C[idx + 1]->n >= t)
        borrowFromNext(idx);
    else {
        if (idx != n)
            merge(idx);
        else
            merge(idx - 1);
    }
}

// Borrow a key from the previous child
void BTreeNode::borrowFromPrev(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx - 1];

    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    if (!child->leaf) {
        for (int i = child->n; i >= 0; --i)
            child->C[i + 1] = child->C[i];
    }

    child->keys[0] = keys[idx - 1];

    if (!child->leaf)
        child->C[0] = sibling->C[sibling->n];

    keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;
}

// Borrow a key from the next child
void BTreeNode::borrowFromNext(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];

    child->keys[child->n] = keys[idx];

    if (!child->leaf)
        child->C[child->n + 1] = sibling->C[0];

    keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->n; ++i)
        sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->C[i - 1] = sibling->C[i];
    }

    child->n += 1;
    sibling->n -= 1;
}

// Merge C[idx] with C[idx+1]
void BTreeNode::merge(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];

    child->keys[t - 1] = keys[idx];

    for (int i = 0; i < sibling->n; ++i)
        child->keys[i + t] = sibling->keys[i];

    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; ++i)
            child->C[i + t] = sibling->C[i];
    }

    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];

    for (int i = idx + 2; i <= n; ++i)
        C[i - 1] = C[i];

    child->n += sibling->n + 1;
    n--;

    delete sibling;
}

// Main driver code
int main() {
    BTree t(3);

    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);

    cout << "Traversal of the constructed tree is\n";
    t.traverse();
    cout << endl;

    t.remove(6);
    cout << "Traversal after removing 6\n";
    t.traverse();
    cout << endl;

    t.remove(13);
    cout << "Traversal after removing 13 (not in tree)\n";
    t.traverse();
    cout << endl;

    t.remove(7);
    cout << "Traversal after removing 7\n";
    t.traverse();
    cout << endl;

    t.remove(4);
    cout << "Traversal after removing 4 (not in tree)\n";
    t.traverse();
    cout << endl;

    t.remove(10);
    cout << "Traversal after removing 10\n";
    t.traverse();
    cout << endl;

    t.remove(20);
    cout << "Traversal after removing 20\n";
    t.traverse();
    cout << endl;

    return 0;
}
