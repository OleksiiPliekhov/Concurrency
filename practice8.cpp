#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

// Задача 1: Введення користувачем рядка з клавіатури та обробка його в іншому потоці

std::mutex mtx;
std::condition_variable cv;
std::string userInput;
bool dataReady = false;

void inputThread() {
    std::cout << "Please enter a string: ";
    std::getline(std::cin, userInput);

    // Повідомляємо про готовність даних та будимо потік обробки
    {
        std::lock_guard<std::mutex> lock(mtx);
        dataReady = true;
    }
    cv.notify_one();
}

void processData() {
    // Очікуємо на готовність даних
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return dataReady; });

    // Обробка даних
    std::cout << "Processed input data: " << userInput << std::endl;
}

// Задача 2: Робота зі спільним буфером, уникаючи колізій

const int bufferSize = 10;
int buffer[bufferSize];
int count = 0;
std::mutex bufferMtx;
std::condition_variable bufferCV;

void producer() {
    for (int i = 0; i < 20; ++i) {
        std::unique_lock<std::mutex> block(bufferMtx);
        bufferCV.wait(block, [] { return count < bufferSize; });

        // Додавання даних в буфер
        buffer[count++] = i;
        std::cout << "Produced: " << i << std::endl;

        block.unlock();
        bufferCV.notify_all();
    }
}

void consumer() {
    for (int i = 0; i < 20; ++i) {
        std::unique_lock<std::mutex> block(bufferMtx);
        bufferCV.wait(block, [] { return count > 0; });

        // Читання даних з буфера
        int data = buffer[--count];
        std::cout << "Consumed: " << data << std::endl;

        block.unlock();
        bufferCV.notify_all();
    }
}

int main() {
    // Задача 1
    std::thread inputThread1(inputThread);
    std::thread processDataThread(processData);

    inputThread1.join();
    processDataThread.join();

    // Задача 2
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    return 0;
}
