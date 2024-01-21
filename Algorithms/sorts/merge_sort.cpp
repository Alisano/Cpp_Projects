#include <iostream>
#include <ctime>

void merge(int arr[], int leftArr[], int rightArr[], int n) {
    int leftN = n/2;
    int rightN = n - leftN;
    int i = 0, j = 0, k = 0;
    while (j < leftN && k < rightN) {
        if (leftArr[j] < rightArr[k]) {
            arr[i] = leftArr[j];
            i++;
            j++;
        } else {
            arr[i] = rightArr[k];
            i++;
            k++;
        }
    }
    while (j < leftN) {
        arr[i] = leftArr[j];
        i++;
        j++;
    }
    while (k < rightN) {
        arr[i] = rightArr[k];
        i++;
        k++;
    }
}

void mergeSort(int arr[], int n) {
    if (n <= 1) return;
    int leftArr[n/2];
    int rightArr[n-n/2];
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (i < n/2) {
            leftArr[i] = arr[i];
        } else {
            rightArr[j] = arr[i];
            j++;            
        }
    }

    mergeSort(leftArr, n/2);
    mergeSort(rightArr, n-n/2);
    merge(arr, leftArr, rightArr, n);
}

int main() {
    unsigned int start_time =  clock();
    // int array[] = {1, 3, 6, 9, 2, 4, 8, 10, 11, 13, 22, 18, 12, 7};
    // int n = sizeof(array) / sizeof(array[0]);
    
    const int n = 1000;
    int array[n];
    srand(time(0));
    for (int i = 0; i < n; i++) {
        array[i] = rand() % 100;
    }
 
    mergeSort(array, n);
    for (int i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;
    unsigned int end_time = clock(); 
    std::cout << end_time - start_time << std::endl;
    return 0;
}