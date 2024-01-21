#include <iostream>
#include <ctime>

void countingSort(int arr[], int n, int m) {
    int count_arr[m];
    for (int i = 0; i < m; i++) count_arr[i] = 0;
    for (int i = 0; i < n; i++) count_arr[arr[i]]++;
    int k = 0;
    for (int i = 0; i < m; i++) {
        for(int j = 0; j < count_arr[i]; j++) {
            arr[k] = i;
            k++;
        }
    }
}

int main() {
    unsigned int start_time =  clock();
    // int array[] = {1, 3, 6, 9, 2, 4, 8, 10, 11, 13, 22, 18, 12};
    // int n = sizeof(array) / sizeof(array[0]);
    
    const int n = 1000;
    const int m = 100;
    int array[n];
    srand(time(0));
    for (int i = 0; i < n; i++) {
        array[i] = rand() % m;
    }

    countingSort(array, n, m);
    for (int i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;
    unsigned int end_time = clock(); 
    std::cout << end_time - start_time << std::endl;
    return 0;
}