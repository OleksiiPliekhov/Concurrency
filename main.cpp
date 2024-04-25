#include <iostream>
#include <thread>
#include <random>
#include <vector>
#include <cmath>


/*void task1(std::string threadName);
void run1_1(std::string threadName, bool flag);
void run1_2(std::string threadName, bool flag);
void run2_2(std::string threadName, bool flag);
void task3(std::vector<double> vctr1, std::vector<double> vctr2);
void helpDotProduct(const std::vector<int>& v1, const std::vector<int>& v2, size_t start, size_t end, long long& result);
long long dotProduct(const std::vector<int>& v1, const std::vector<int>& v2);*/

const int CONSTANT = 5;
void multiplyNumber(int number, int CONSTANT);
void myThread(std::string threadName);

const int NX = 10;
const int NY = 15;
const double DX0 = 0;
const double DX1 = 25;
const double DY0 = -10;
const double DY1 = 10;

double* generateRandomDoubleArray(double min, double max, int arraySize);
double firstFunction(double* array, int arraySize, int threadNum);
void  helpFirstFunction(int start, int end, double* array ,double & result);
void  helpSecondFunction(int start, int end, double* array ,double & result);
double secondFunction(double* array, int arraySize, int threadNum);

/*
int main() {

    /*std::thread t1(task1, "first 1");
    std::thread f1(task1, "secind 1");

    //task 2.1

    std::thread t2(run1_1, "first 2", true);
    std::thread f2(run1_2, "second 2" , true);
    std::thread k2(run1_1, "third 2" , true);


    //task2.2

    std::thread t22(run2_2, "first 22", true);
    std::thread f22(run2_2, "second 22", true);
    std::thread k22(run2_2, "third 22", true);

    t1.join();
    f1.join();


    t2.join();
    f2.join();
    k2.join();

    t22.join();
    f22.join();
    k22.join();#1#

    //task 3
    /*std::vector<double> vctr1 = {1, 3, 5, 6};
    std::vector<double> vctr2 = {-2, 4, 8, 1};
    std::thread t(task3, vctr1, vctr2);
    t.join();#1#


    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {6, 7, 8, 9, 10};
    int result = dotProduct(std::ref(v1), std::ref(v2));
    std::cout << "Vector product: " << result << std::endl;
    #1#*/


/*
    double* randomArray1 = generateRandomDoubleArray(DX0, DX1, NX);
    double* randomArray2 = generateRandomDoubleArray(DY0, DY1, NY);



    std::thread t2_1(myThread, "first");
    t2_1.detach();
    std::thread t2_2(myThread, "second");
    t2_2.join();
    std::thread t2_3(myThread, "third");


    std::thread t3_1(firstFunction, std::ref(randomArray1), NX, 2);
    t3_1.join();

    std::thread t3_2(secondFunction, std::ref(randomArray2), NY, 3);
    t3_2.join();

    std::thread t1(multiplyNumber, 5, CONSTANT);
    t1.join();


    t2_3.join();
    delete[] randomArray1;
    delete[] randomArray2;
    return 0;
}*/


/*
//task #1
void task1(std::string threadName) {
    std::cout << "Thread " << threadName << ": " << std::this_thread::get_id() <<" Random number: " << 1 + rand() % 10 << "\n" << std::endl;
}

//task #2.1
void run1_1(std::string threadName, const bool flag) {
    for (int i = 0; i < 10; i++) {
        if(flag) {
            std::cout << std::this_thread::get_id();
            std::string out = threadName + " " + std::to_string(i) + "\n";
            std::cout<<out;
        }
    }
}

void run1_2(std::string threadName, bool flag) {
    std::cout << std::this_thread::get_id() <<  threadName;
    flag != flag;
}

//task #2.2

void run2_2(std::string threadName, bool flag) {
    for (int i = 0; i < 10; i++) {
        if(flag) {
            std::cout << std::this_thread::get_id();
            std::string out = threadName + " " + std::to_string(i) + "\n";
            std::cout<<out;
        }
    }
    flag != flag;
}


void helpDotProduct(const std::vector<int>& v1, const std::vector<int>& v2, size_t start, size_t end, long long & result) {
    long long partialSum = 0;
    for (size_t i = start; i < end; ++i) {
        partialSum += v1[i] * v2[i];
    }
    result += partialSum;
}

long long dotProduct(const std::vector<int>& v1, const std::vector<int>& v2) {
    if (v1.size() != v2.size()) {
        std::cout << "vectors with different sizes" << std::endl;
        return 0;
    }

    const size_t numThreads = std::thread::hardware_concurrency();

    // Розмір кожного підмасиву
    const size_t chunkSize = v1.size() / numThreads;

    // Вектор для збереження результатів
    std::vector<long long> results(numThreads);

    // Створення та запуск потоків
    std::vector<std::thread> threads;
    for (size_t i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? v1.size() : start + chunkSize;
        threads.emplace_back(helpDotProduct, std::ref(v1), std::ref(v2), start, end, std::ref(results[i]));
    }

    // Очікування завершення потоків
    for (auto& thread : threads) {
        thread.join();
    }

    long long res = 0;
    for (auto result: results) {
        res += result;
    }
    return res;
}
*/


void multiplyNumber(int number,  int CONSTANT) {
    std::cout << "Start calculation..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int result = number * CONSTANT;
    std::cout << "Result: " << result << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Calculation end." << std::endl;
}


void myThread(std::string threadName) {
    std::cout << "Thred "  << threadName <<" start." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Thred "  << threadName <<" end." << std::endl;
}


double* generateRandomDoubleArray(double min, double max, int arraySize) {
    if (min > max) {
        std::swap(min, max); // Ensure min is always less than or equal to max
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min, max);

    double* resArray = new double[arraySize];  // Allocate memory for arraySize elements

    // Generate and assign random values to the array
    for (int i = 0; i < arraySize; ++i) {
        resArray[i] = dist(gen);
    }

    return resArray;
}


double firstFunction(double* array, int arraySize, int threadNum) {
    // Розмір кожного підмасиву
    size_t chunkSize = arraySize / threadNum;

    // Вектор для збереження результатів
    std::vector<double> results(threadNum);

    // Створення та запуск потоків
    std::vector<std::thread> threads;
    for (size_t i = 0; i < threadNum; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == threadNum - 1) ? arraySize : start + chunkSize;
        threads.emplace_back(helpFirstFunction, start, end, std::ref(array), std::ref(results[i]));
    }

    // Очікування завершення потоків
    for (auto& thread : threads) {
        thread.join();
    }

    double res = 0;
    for (auto result: results) {
        res += result;
    }

    std::cout << "Result of the function: " << res << std::endl;
    return res;
}

void  helpFirstFunction(int start, int end, double* array ,double & result) {
    for(int i = start; i < end; i++) {
        result += std::cbrt(array[i] * std::sin(std::pow(array[i], 2)));
    }
}

double secondFunction(double* array, int arraySize, int threadNum) {
    // Розмір кожного підмасиву
    size_t chunkSize = arraySize / threadNum;

    // Вектор для збереження результатів
    std::vector<double> results(threadNum);

    // Створення та запуск потоків
    std::vector<std::thread> threads;
    for (size_t i = 0; i < threadNum; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == threadNum - 1) ? arraySize : start + chunkSize;
        threads.emplace_back(helpSecondFunction, start, end, std::ref(array), std::ref(results[i]));
    }

    // Очікування завершення потоків
    for (auto& thread : threads) {
        thread.join();
    }

    double res = 0;
    for (auto result: results) {
        res += result;
    }

    std::cout << "Result of the function: " << res << std::endl;
    return res;
}

void  helpSecondFunction(int start, int end, double* array ,double & result) {
    for(int i = start; i < end; i++) {
        result += std::cbrt(array[i] * std::cos(std::pow(array[i], 2)));
    }

}