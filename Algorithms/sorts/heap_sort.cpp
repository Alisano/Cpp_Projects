#include <iostream>
#include <ctime>

void heapSort(int arr[], int n) {
    for (int i = 0; i < (n - 1); i++) {
        for (int j = (n - i) / 2 - 1; j >= 0; j--) {
            int largest = j;
            if  (((2 * j + 1) < (n - i)) && (arr[largest] < arr[2 * j + 1])) largest = 2 * j + 1;
            if (((2 * j + 2) < (n - i)) && (arr[largest] < arr[2 * j + 2])) largest = 2 * j + 2;
            if (largest != j) std::swap(arr[j], arr[largest]);
        }
        std::swap(arr[0], arr[n - i - 1]);
    }
}

int main() {
    unsigned int start_time =  clock();
    // int array[] = {1, 3, 6, 9, 2, 4, 8, 10, 11, 13, 22, 18, 12};
    // int n = sizeof(array) / sizeof(array[0]);
    
    const int n = 1000;
    int array[n];
    srand(time(0));
    for (int i = 0; i < n; i++) {
        array[i] = rand() % 100;
    }
 
    heapSort(array, n);
    for (int i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;
    unsigned int end_time = clock(); 
    std::cout << end_time - start_time << std::endl;
    return 0;
}