/*Розробити багатопоточну програму для роботи з двоспрямованими списками.
Повинні бути реалізовані наступні операції:
виведення елементів списку в обох напрямках +
Видалення заданого елемента+
Вставка елемента перед заданим.+
Вставка елементу після заданого+
Перевірка на симетрію*/


#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

template<typename T>
struct Node {
    T data;
    std::atomic<Node<T>*> next;
    std::atomic<Node<T>*> prev;

    Node(T val) : data(val), next(nullptr), prev(nullptr) {}
};

template<typename T>
class DoublyLinkedList {
private:
    std::atomic<Node<T>*> head;
    std::atomic<Node<T>*> tail;

public:

    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    void printForward() {
        Node<T>* current = head.load();
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next.load();
        }
        std::cout << std::endl;
    }

    void printBackward() {
        Node<T>* current = tail.load();
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->prev.load();
        }
        std::cout << std::endl;
    }

    void pushBack(T val) {
        Node<T>* newNode = new Node<T>(val);
        newNode->next.store(nullptr);
        Node<T>* prevTail = tail.exchange(newNode);
        if (prevTail != nullptr) {
            prevTail->next.store(newNode);
            newNode->prev.store(prevTail);
        } else {
            head.store(newNode);
        }
    }

    void pushFront(T val) {
        Node<T>* newNode = new Node<T>(val);
        newNode->prev.store(nullptr);
        Node<T>* prevHead = head.exchange(newNode);
        if (prevHead != nullptr) {
            prevHead->prev.store(newNode);
            newNode->next.store(prevHead);
        } else {
            tail.store(newNode);
        }
    }

    void insertBefore(T val, T beforeVal) {
        Node<T>* current = head.load();
        while (current != nullptr) {
            if (current->data == beforeVal) {
                Node<T>* newNode = new Node<T>(val);
                if (current->prev.load() != nullptr) {
                    current->prev.load()->next.store(newNode);
                    newNode->prev.store(current->prev.load());
                } else {
                    head.store(newNode);
                }
                newNode->next.store(current);
                current->prev.store(newNode);
                return;
            }
            current = current->next.load();
        }
    }

    void insertAfter(T val, T afterVal) {
        Node<T>* current = head.load();
        while (current != nullptr) {
            if (current->data == afterVal) {
                Node<T>* newNode = new Node<T>(val);
                if (current->next.load() != nullptr) {
                    current->next.load()->prev.store(newNode);
                    newNode->next.store(current->next.load());
                } else {
                    tail.store(newNode);
                }
                newNode->prev.store(current);
                current->next.store(newNode);
                return;
            }
            current = current->next.load();
        }
    }

    bool isSymmetric() {
        Node<T>* forward = head.load();
        Node<T>* backward = tail.load();
        while (forward != nullptr && backward != nullptr) {
            if (forward->data != backward->data)
                return false;
            forward = forward->next.load();
            backward = backward->prev.load();
        }
        return true;
    }

    void deleteNode(T val) {
        Node<T>* current = head.load();
        while (current != nullptr) {
            if (current->data == val) {
                if (current == head.load()) {
                    head.store(current->next.load());
                    if (head.load() != nullptr)
                        head.load()->prev.store(nullptr);
                } else if (current == tail.load()) {
                    tail.store(current->prev.load());
                    if (tail.load() != nullptr)
                        tail.load()->next.store(nullptr);
                } else {
                    current->prev.load()->next.store(current->next.load());
                    current->next.load()->prev.store(current->prev.load());
                }
                delete current;
                return;
            }
            current = current->next.load();
        }
    }
    void deleteAllNodes(T val) {
        Node<T>* current = head.load();
        while (current != nullptr) {
            if (current->data == val) {
                if (current == head.load()) {
                    head.store(current->next.load());
                    if (head.load() != nullptr)
                        head.load()->prev.store(nullptr);
                } else if (current == tail.load()) {
                    tail.store(current->prev.load());
                    if (tail.load() != nullptr)
                        tail.load()->next.store(nullptr);
                } else {
                    current->prev.load()->next.store(current->next.load());
                    current->next.load()->prev.store(current->prev.load());
                }
                Node<T>* temp = current;
                current = current->next.load();
                delete temp;
            } else {
                current = current->next.load();
            }
        }
    }
};

void insertElements(DoublyLinkedList<int>& list, int start, int end) {
    for (int i = start; i <= end; ++i) {
        list.pushBack(i);
    }
}

void deleteElements(DoublyLinkedList<int>& list, int val) {
    list.deleteAllNodes(val);
}

int main() {
    DoublyLinkedList<int> list;

    std::thread t1(insertElements, std::ref(list), 1, 5);
    std::thread t2(insertElements, std::ref(list), 6, 10);
    std::thread t3(insertElements, std::ref(list), 14, 19);

    std::vector<std::thread> threads;
    for (int i = 3; i < 6; ++i) {
        threads.emplace_back([&list, i] {
            for (int j = 1; j <= 5; ++j) {
                list.pushFront(i * 10 + j);
            }
        });
    }

    std::cout << "List after concurrent insertions: ";
    list.printForward();
    list.printBackward();

    std::thread t5(deleteElements, std::ref(list), 6);
    std::cout << "List after concurrent deletions 6: ";
    list.printForward();
    std::thread t6(deleteElements, std::ref(list), 14);
    std::cout << "List after concurrent deletions 14: ";
    list.printForward();


    t1.join();
    t2.join();
    t3.join();

    for (auto& thread : threads) {
        thread.join();
    }

    t5.join();
    t6.join();


    return 0;
}

