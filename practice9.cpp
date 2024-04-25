#include <iostream>
#include <vector>
#include <future>
#include <algorithm>

// Функція розбиття списку на дві частини
template<typename T>
size_t partition(std::vector<T>& arr, size_t low, size_t high) {
    T pivot = arr[high];
    size_t i = low - 1;
    for (size_t j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            std::swap(arr[++i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Функція сортування однієї частини списку за допомогою quicksort
template<typename T>
void quicksort(std::vector<T>& arr, size_t low, size_t high) {
    if (low < high) {
        size_t pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

// Паралельний алгоритм швидкого сортування
template<typename T>
void parallel_quicksort(std::vector<T>& arr, size_t low, size_t high, size_t threshold) {
    if (low < high) {
        size_t pi = partition(arr, low, high);

        // Поріг для визначення, коли почати сортування в окремому потоці
        if (high - low > threshold) {
            auto left_future = async_quicksort(arr, low, pi - 1);
            auto right_future = async_quicksort(arr, pi + 1, high);
            left_future.wait();
            right_future.wait();
        } else {
            quicksort(arr, low, pi - 1);
            quicksort(arr, pi + 1, high);
        }
    }
}

// Функція сортування однієї частини списку в окремому потоці
template<typename T>
std::future<void> async_quicksort(std::vector<T>& arr, size_t low, size_t high) {
    return std::async(std::launch::async, [=]() {
        parallel_quicksort(arr, low, high);
    });
}

int main() {
    std::vector<int> arr = {9, 3, 7, 5, 6, 4, 8, 2, 1};
    size_t threshold = 1000; // Поріг для розділення сортування на паралельні задачі

    parallel_quicksort(arr, 0, arr.size() - 1, threshold);

    std::cout << "Sorted array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
