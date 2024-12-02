#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For seeding rand()
using namespace std;

// Function to swap two elements
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

// Partition function with a random pivot
int randomizedPartition(int arr[], int low, int high) {
    int randomIndex = low + rand() % (high - low + 1);
    swap(arr[randomIndex], arr[high]); // Move the random pivot to the end
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Randomized Select function to find the i-th smallest element
int randomizedSelect(int arr[], int low, int high, int i) {
    if (low == high) {
        return arr[low];
    }

    int pivotIndex = randomizedPartition(arr, low, high);
    int k = pivotIndex - low + 1; // Number of elements in the left partition including pivot

    if (i == k) {
        return arr[pivotIndex]; // Pivot is the i-th smallest element
    } else if (i < k) {
        return randomizedSelect(arr, low, pivotIndex - 1, i);
    } else {
        return randomizedSelect(arr, pivotIndex + 1, high, i - k);
    }
}

int main() {
    srand(time(0)); // Seed the random number generator

    int n, i;
    cout << "Enter the number of elements in the array: ";
    cin >> n;

    int arr[n];
    cout << "Enter the elements of the array:\n";
    for (int j = 0; j < n; j++) {
        cin >> arr[j];
    }

    cout << "Enter the value of i (1-based index for the i-th smallest element): ";
    cin >> i;

    if (i < 1 || i > n) {
        cout << "Invalid input for i. It should be between 1 and " << n << "." << endl;
        return 1;
    }

    int result = randomizedSelect(arr, 0, n - 1, i);
    cout << "The " << i << "-th smallest element is: " << result << endl;

    return 0;
}
