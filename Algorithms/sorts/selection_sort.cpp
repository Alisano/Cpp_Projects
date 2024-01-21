#include <iostream>
#include <ctime>

void selectionSort(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        int tmp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = tmp;
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

    selectionSort(array, n);
    for (int i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;
    unsigned int end_time = clock(); 
    std::cout << end_time - start_time << std::endl;
    return 0;
}