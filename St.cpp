#include <iostream>
#include <string>
using namespace std;

// Structure to represent each node in the suffix tree
struct SuffixTreeNode {
    SuffixTreeNode* children[256];  // Array of pointers to children nodes (ASCII characters)
    bool isLeaf;  // Flag to indicate if the node is a leaf node

    // Constructor to initialize all children as null and mark node as non-leaf
    SuffixTreeNode() {
        for (int i = 0; i < 256; ++i) {
            children[i] = nullptr;
        }
        isLeaf = false;
    }
};

// Function to insert a suffix into the suffix tree
SuffixTreeNode* bruteForceSuffixTrie(const string &T) {
    // Root of the trie
    SuffixTreeNode* root = new SuffixTreeNode();

    // Iterate over all suffixes starting from the largest to smallest
    for (int i = T.size() - 1; i >= 0; --i) {
        SuffixTreeNode* node = root;

        // Traverse through each character in the suffix and insert it into the tree
        for (int j = i; j < T.size(); ++j) {
            char c = T[j];
            if (node->children[c] == nullptr) {
                node->children[c] = new SuffixTreeNode();
            }
            node = node->children[c];
        }

        // Mark the end of the suffix
        node->isLeaf = true;
    }

    cout << "Suffix Trie constructed!" << endl;
    return root;
}

// Function to print all the suffixes of the string using DFS
void printSuffixes(SuffixTreeNode* root, string currentSuffix = "") {
    if (root == nullptr) return;

    // If the node is a leaf, print the accumulated suffix
    if (root->isLeaf) {
        cout << currentSuffix << endl;
    }

    // Recur for each child
    for (int i = 0; i < 256; ++i) {
        if (root->children[i] != nullptr) {
            printSuffixes(root->children[i], currentSuffix + char(i));
        }
    }
}

int main() {
    string T = "banana";
    cout << "Building suffix tree for string: " << T << endl;

    // Build the suffix trie and get its root
    SuffixTreeNode* root = bruteForceSuffixTrie(T);

    // Print all suffixes in the trie
    cout << "\nSuffixes in the trie:\n";
    printSuffixes(root);

    return 0;
}
