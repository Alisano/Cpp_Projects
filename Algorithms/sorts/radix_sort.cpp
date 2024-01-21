#include <iostream>
#include <vector>
#include <random>
#include <ctime>

void radixSort(int arr[], int n) {
    int num_sys = 10;
    std::vector<int> count_arr[num_sys];
    int powerOfTen = 1;

    for (int i = 0; i < num_sys; i++) {
        count_arr[i].reserve(n / num_sys + 1);
    }

    for (int l = 0; l < num_sys; l++) {
        for (int i = 0; i < n; i++) {
            count_arr[(arr[i] / powerOfTen) % num_sys].push_back(arr[i]);
        }
        int k = 0;
        for (int i = 0; i < num_sys; i++) {
            for(int j = 0; j < count_arr[i].size(); j++) {
                arr[k] = count_arr[i][j];
                k++;
            }
            count_arr[i].clear();
        }
        powerOfTen *= num_sys;
    }
}

int main() {
    unsigned int start_time = clock();
    // int array[] = {1, 3, 6, 9, 2, 4, 8, 10, 11, 13, 22, 18, 12};
    // int n = sizeof(array) / sizeof(array[0]);
    
    const int n = 1000;
    int array[n];
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 99);
    for (int i = 0; i < n; i++) {
        array[i] = distribution(gen);
    }

    radixSort(array, n);
    for (int i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;
    unsigned int end_time = clock(); 
    std::cout << end_time - start_time << std::endl;
    return 0;
}