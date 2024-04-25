#include <iostream>
#include <mutex>
#include <thread>


std::mutex mtx;

class Account {
private:
    mutable std::mutex acc_mutex;
    double balance;

public:
    Account(double input_balance) : balance(input_balance) {}

    void deposit(double amount) {
        std::unique_lock<std::mutex> lock(acc_mutex);
        balance += amount;
        lock.unlock();
    }


    bool withdraw(double amount) {
        std::unique_lock<std::mutex> lock(acc_mutex);
        if (amount <= balance) {
            balance -= amount;
            lock.unlock();
            return true;
        }
        lock.unlock();
        return false;
    }


    void transfer(Account& to, double amount) {
        std::unique_lock<std::mutex> lock_from(acc_mutex, std::defer_lock);
        std::unique_lock<std::mutex> lock_to(to.acc_mutex, std::defer_lock);
        std::lock(lock_from, lock_to);

        if (balance >= amount) {
            balance -= amount;
            to.balance += amount;
            mtx.lock();
            std::cout<<"Transfer successful\n";
            mtx.unlock();
            return;
        }
        mtx.lock();
        std::cout<<"Transfer Not successful\n";
        mtx.unlock();

    }

    double get_balance() const {
        return balance;
    }

    std::string toString() const {
        return "Balance: " + std::to_string(balance);
    }
};

int main() {
    Account bankAccount1(0);
    Account bankAccount2(0);
    std::thread thread1 ([&](){bankAccount1.deposit(250);});

    std::thread thread2 ([&](){bankAccount1.transfer(bankAccount2,220);});
    std::thread thread3 ([&](){bankAccount1.transfer(bankAccount2,100);});

    std::thread thread4 ([&](){bankAccount1.deposit(120);});
    std::thread thread5 ([&](){bankAccount1.transfer(bankAccount2,100);});
    std::thread thread6 ([&](){bankAccount2.transfer(bankAccount1,0);});
    std::thread thread7 ([&](){bankAccount1.withdraw(5);});
    std::thread thread8 ([&](){bankAccount2.withdraw(5);});
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    std::cout<<"Account 1: " << bankAccount1.toString() << "\n";
    std::cout<<"Account 2: " << bankAccount2.toString() << "\n";
    return 0;
}
