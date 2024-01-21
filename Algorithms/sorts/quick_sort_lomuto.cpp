//Lomuto realisation

#include <iostream>
#include <ctime>

int partition(int arr[], int start, int end) {
    int pivot = arr[end];
    int i = start - 1;
    for (int j = start; j <= end; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    i++;
    std::swap(arr[i], arr[end]);

    return i;
}

void quickSort(int arr[], int start, int end) {
    if (end - start <= 0) return;
    int pivot_idx = partition(arr, start, end);
    quickSort(arr, start, pivot_idx - 1);
    quickSort(arr, pivot_idx + 1, end);
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
 
    quickSort(array, 0, n - 1);
    for (int i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;
    unsigned int end_time = clock(); 
    std::cout << end_time - start_time << std::endl;
    return 0;
}