#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Function to compute the failure function
vector<int> computeFailFunction(const string& pattern) {
    vector<int> fail(pattern.size());
    fail[0] = 0;  // The first position is always 0
    int j = 0;    // Length of the previous longest prefix suffix
    int i = 1;    // Current position in the pattern

    while (i < pattern.size()) {
        if (pattern[j] == pattern[i]) {
            fail[i] = j + 1;  // Found a match
            i++;
            j++;
        } else if (j > 0) {
            j = fail[j - 1];  // Use the failure function
        } else {
            fail[i] = 0;  // No match
            i++;
        }
    }
    return fail;
}

// Function to perform KMP pattern matching
int KMPmatch(const string& text, const string& pattern) {
    int n = text.size();
    int m = pattern.size();
    vector<int> fail = computeFailFunction(pattern);
    int i = 0;  // Text index
    int j = 0;  // Pattern index

    while (i < n) {
        if (pattern[j] == text[i]) {
            if (j == m - 1) {
                return i - m + 1;  // Match found, return the starting index
            }
            i++;
            j++;
        } else if (j > 0) {
            j = fail[j - 1];  // Use the failure function to skip comparisons
        } else {
            i++;
        }
    }
    return -1;  // No match
}

int main() {
    string text = "ababcabcabababd";
    string pattern = "ababd";

    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl;

    int position = KMPmatch(text, pattern);

    if (position != -1) {
        cout << "Pattern found at index " << position << endl;
    } else {
        cout << "Pattern not found" << endl;
    }

    return 0;
}
