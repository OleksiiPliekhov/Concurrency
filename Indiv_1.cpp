#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <condition_variable>

using namespace std;

class Client {
public:
    string fio;
    atomic<double> balance;
    Client(const string& name, double initialBalance) : fio(name), balance(initialBalance) {}
};

class Cashier {
public:
    atomic<double> moneyInCashDesc;
    atomic<int> waitingClients;
    atomic<int> activeClients;
    std::mutex mtx;

    Cashier(double initialMoney) : moneyInCashDesc(initialMoney), waitingClients(0), activeClients(0) {}

    bool depositMoney(Client& client, double amount) {
        double currentBalance = client.balance.load();
        double newBalance = currentBalance + amount;

        if (client.balance.compare_exchange_weak(currentBalance, newBalance) && activeClients <= 2) {
            std::lock_guard<std::mutex> lock(mtx);
            activeClients ++;
            moneyInCashDesc.store(moneyInCashDesc.load() + amount);
            cout << "Deposited " << amount << " to " << client.fio << "'s account. New balance: " << newBalance <<
                 " Cash in bank: " << moneyInCashDesc << endl;
            return true;
        }

        return false;
    }

    bool withdrawMoney(Client& client, double amount) {
        double currentBalance = client.balance.load();

        if (moneyInCashDesc < amount)
            waitingClients.fetch_add(1);
        while (moneyInCashDesc < amount) {
            this_thread::sleep_for(chrono::milliseconds(100));
            currentBalance = client.balance.load();
        }

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (moneyInCashDesc >= amount && activeClients < 2) {
                activeClients++;
                if (waitingClients)
                    waitingClients.fetch_sub(1);
                if (client.balance.compare_exchange_weak(currentBalance, currentBalance - amount)) {
                    moneyInCashDesc.store(moneyInCashDesc.load() - amount);
                    cout << "Withdrawn " << amount << " from " << client.fio << "'s account. New balance: " << currentBalance - amount <<
                         " Cash in bank: " << moneyInCashDesc << endl;
                    activeClients--;
                    return true;
                }
            }
            else{
                cout << "Not enough seats at the cash register" << endl;
                return false;
            }
        }
        return false;
    }


    void replenishCash(double amount) {
        moneyInCashDesc.store(moneyInCashDesc.load() + amount);
    }

    void autoReplenish() {
        while (true) {
            this_thread::sleep_for(chrono::seconds(10));
            replenishCash(500.0);
            mtx.lock();
            cout << "Cashier balance after replenishment: " << moneyInCashDesc << endl;
            mtx.unlock();
        }
    }
};

int main() {
    std::mutex mtx;
    // Створення клієнтів та каси
    Client client1("John Doe", 501.0);
    Client client2("Alice Smith", 800.0);
    Client client3("Tom Shelbi", 400.0);
    Cashier cashier(200.0);

    auto testWithdrawal1 = [&]() {
        bool success1 = cashier.withdrawMoney(client1, 1000.0);
        mtx.lock();
        if (!success1)
            cout << "Withdrawal failed. Insufficient funds." << endl;
        mtx.unlock();
    };

    auto testWithdrawal2 = [&]() {
        bool success1 = cashier.withdrawMoney(client1, 1.0);
        mtx.lock();
        if (!success1)
            cout << "Withdrawal failed. Insufficient funds." << endl;
        mtx.unlock();
    };

    auto testWithdrawal3 = [&]() {
        bool success1 = cashier.withdrawMoney(client2, 784.0);
        mtx.lock();
        if (!success1)
            cout << "Withdrawal failed. Insufficient funds." << endl;
        mtx.unlock();
    };

    auto testWithdrawal4 = [&]() {
        bool success1 = cashier.depositMoney(client1, 600.0);
        mtx.lock();
        if (!success1)
            cout << "Deposit failed. Insufficient funds." << endl;
        mtx.unlock();
    };

    auto testWithdrawal5 = [&]() {
        bool success1 = cashier.withdrawMoney(client3, 50.0);
        mtx.lock();
        if (!success1)
            cout << "Withdrawal failed. Insufficient funds." << endl;
        mtx.unlock();
    };

    thread t1(testWithdrawal1);
    thread t3(testWithdrawal3);
    thread t2(testWithdrawal2);
    thread t4(testWithdrawal4);
    thread t5(testWithdrawal5);

    thread replenishThread(&Cashier::autoReplenish, &cashier);

    t1.join();
    t3.join();
    t2.join();
    t5.join();
    t4.join();
    replenishThread.join();

    return 0;
}
