#include <iostream>
#include <ctime>

bool correct(int arr[], int n) {
    for (int i = 0; i < (n - 1); i++) 
        if (arr[i] > arr[i + 1]) 
            return false;
    return true;
}

void shuffle(int arr[], int n) {
    for (int i = 0; i < (n - 1); i++) {
        std::swap(arr[i], arr[rand() % n]);
    }
}

void bogoSort(int arr[], int n) {
    while (!correct(arr, n)) {
        shuffle(arr, n);
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

    bogoSort(array, n);
    for (int i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;
    unsigned int end_time = clock(); 
    std::cout << end_time - start_time << std::endl;
    return 0;
}