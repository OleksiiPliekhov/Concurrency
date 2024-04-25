#include <iostream>
#include <thread>
#include <random>
#include <vector>
#include <cmath>

void multiplyByCaonsant(long& value,  int constant);
double multiplication(double& left, double& right);


class Task3 {
public:
    void run(std::string threadName) {
        for (int i = 0; i < 10; i++) {
            std::cout << std::this_thread::get_id();
            std::string out = threadName + "|" + std::to_string(i) + "\n";
            std::cout<<out;
        }
    }

    void multiply(double& left, double& right) {
        left *= right;
    }
};

/*int main() {
    //task 1
    int threadNum = std::thread::hardware_concurrency();
    std::cout << "Thread num : " << threadNum << std::endl;
    long value = 2;
    const int CONSTAT = 2;
    std::vector<std::thread> threads(threadNum);
    for(int i = 0; i < threadNum; i++) {
        threads[i] = std::thread (multiplyByCaonsant, std::ref(value), CONSTAT);
    }
    for(int i = 0; i < threadNum; i++) {
        std::cout << "thread: " << i << " " << value << std::endl;
        threads[i].join();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    //task 2
    double res = 0;
    double left = 1.5;
    double right = 2;
    std::cout << "Task 2:";
    std::thread t2([&res, &left, &right]() { res = multiplication(std::ref(left), std::ref(right));});
    t2.join();
    std::cout << "Result of the second task: " << res << std::endl;

    Task3 tsk;

    //task 3
    std::vector<std::thread>threads2(threadNum);
    for (int i = 0; i< threadNum;i++){
        if(i % 2 == 0)
            threads[i]=  std::thread (&Task3::run,tsk, std::string ("i = "+std::to_string(i)));
        else
            threads[i]=  std::thread (&Task3::multiply,tsk, std::ref(left),std::ref(right));
    }
    for (int i = 0; i<threadNum;i++){
        threads[i].join();
        if(i%2!=0)
            std::cout<<left<<"\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}
*/

void multiplyByCaonsant(long& value,  int constant) {
    value *= constant;
}

double multiplication(double& left, double& right) {
    return left * right;
}

